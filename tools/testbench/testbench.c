// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2018 Intel Corporation. All rights reserved.
//
// Author: Seppo Ingalsuo <seppo.ingalsuo@linux.intel.com>
//         Ranjani Sridharan <ranjani.sridharan@linux.intel.com>

#include <sof/drivers/ipc.h>
#include <sof/list.h>
#include <getopt.h>
#include <dlfcn.h>
#include "testbench/common_test.h"
#include <tplg_parser/topology.h>
#include "testbench/trace.h"
#include "testbench/file.h"

#define TESTBENCH_NCH 2 /* Stereo */

/* shared library look up table */
struct shared_lib_table lib_table[NUM_WIDGETS_SUPPORTED] = {
	{"file", "", SOF_COMP_HOST, 0, NULL}, /* File must be first */
	{"volume", "libsof_volume.so", SOF_COMP_VOLUME, 0, NULL},
	{"src", "libsof_src.so", SOF_COMP_SRC, 0, NULL},
	{"asrc", "libsof_asrc.so", SOF_COMP_ASRC, 0, NULL},
	{"eq-fir", "libsof_eq-fir.so", SOF_COMP_EQ_FIR, 0, NULL},
	{"eq-iir", "libsof_eq-iir.so", SOF_COMP_EQ_IIR, 0, NULL},
};

/* main firmware context */
static struct sof sof;

/* compatible variables, not used */
intptr_t _comp_init_start, _comp_init_end;

struct sof *sof_get()
{
	return &sof;
}

/*
 * Parse shared library from user input
 * Currently only handles volume and src comp
 * This function takes in the libraries to be used as an input in the format:
 * "vol=libsof_volume.so,src=libsof_src.so,..."
 * The function parses the above string to identify the following:
 * component type and the library name and sets up the library handle
 * for the component and stores it in the shared library table
 */
static int parse_libraries(char *libs)
{
	char *lib_token = NULL;
	char *comp_token = NULL;
	char *token = strtok_r(libs, ",", &lib_token);
	int index;

	while (token) {

		/* get component type */
		char *token1 = strtok_r(token, "=", &comp_token);

		/* get shared library index from library table */
		index = get_index_by_name(token1, lib_table);

		if (index < 0) {
			fprintf(stderr, "error: unsupported comp type\n");
			return -EINVAL;
		}

		/* get shared library name */
		token1 = strtok_r(NULL, "=", &comp_token);
		if (!token1)
			break;

		/* set to new name that may be used while loading */
		strncpy(lib_table[index].library_name, token1,
			MAX_LIB_NAME_LEN - 1);

		/* next library */
		token = strtok_r(NULL, ",", &lib_token);
	}
	return 0;
}

/* print usage for testbench */
static void print_usage(char *executable)
{
	printf("Usage: %s -i <input_file> -o <output_file> ", executable);
	printf("-t <tplg_file> -b <input_format> ");
	printf("-a <comp1=comp1_library,comp2=comp2_library>\n");
	printf("input_format should be S16_LE, S32_LE, S24_LE or FLOAT_LE\n");
	printf("Example Usage:\n");
	printf("%s -i in.txt -o out.txt -t test.tplg ", executable);
	printf("-r 48000 -R 96000 ");
	printf("-b S16_LE -a vol=libsof_volume.so\n");
}

/* free components */
static void free_comps(void)
{
	struct list_item *clist;
	struct list_item *temp;
	struct ipc_comp_dev *icd = NULL;

	list_for_item_safe(clist, temp, &sof.ipc->comp_list) {
		icd = container_of(clist, struct ipc_comp_dev, list);
		switch (icd->type) {
		case COMP_TYPE_COMPONENT:
			comp_free(icd->cd);
			list_item_del(&icd->list);
			rfree(icd);
			break;
		case COMP_TYPE_BUFFER:
			rfree(icd->cb->stream.addr);
			rfree(icd->cb);
			list_item_del(&icd->list);
			rfree(icd);
			break;
		default:
			rfree(icd->pipeline);
			list_item_del(&icd->list);
			rfree(icd);
			break;
		}
	}
}

static void parse_input_args(int argc, char **argv, struct testbench_prm *tp)
{
	int option = 0;
	int ret = 0;

	while ((option = getopt(argc, argv, "hdi:o:t:b:a:r:R:")) != -1) {
		switch (option) {
		/* input sample file */
		case 'i':
			tp->input_file = strdup(optarg);
			break;

		/* output sample file */
		case 'o':
			tp->output_file = strdup(optarg);
			break;

		/* topology file */
		case 't':
			tp->tplg_file = strdup(optarg);
			break;

		/* input samples bit format */
		case 'b':
			tp->bits_in = strdup(optarg);
			tp->frame_fmt = find_format(tp->bits_in);
			break;

		/* override default libraries */
		case 'a':
			ret = parse_libraries(optarg);
			break;

		/* input sample rate */
		case 'r':
			tp->fs_in = atoi(optarg);
			break;

		/* output sample rate */
		case 'R':
			tp->fs_out = atoi(optarg);
			break;

		/* enable debug prints */
		case 'd':
			debug = 1;
			break;

		/* print usage */
		case 'h':
		default:
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}

		if (ret < 0)
			exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	struct testbench_prm tp;
	struct ipc_comp_dev *pcm_dev;
	struct pipeline *p;
	struct sof_ipc_pipe_new *ipc_pipe;
	struct comp_dev *cd;
	struct file_comp_data *frcd, *fwcd;
	char pipeline[DEBUG_MSG_LEN];
	clock_t tic, toc;
	double c_realtime, t_exec;
	int n_in, n_out, ret;
	int i;

	/* initialize input and output sample rates, files, etc. */
	tp.fs_in = 0;
	tp.fs_out = 0;
	tp.bits_in = 0;
	tp.input_file = NULL;
	tp.output_file = NULL;
	tp.channels = TESTBENCH_NCH;

	/* command line arguments*/
	parse_input_args(argc, argv, &tp);

	/* check args */
	if (!tp.tplg_file || !tp.input_file || !tp.output_file || !tp.bits_in) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* initialize ipc and scheduler */
	if (tb_pipeline_setup(&sof) < 0) {
		fprintf(stderr, "error: pipeline init\n");
		exit(EXIT_FAILURE);
	}

	/* parse topology file and create pipeline */
	if (parse_topology(&sof, lib_table, &tp, pipeline) < 0) {
		fprintf(stderr, "error: parsing topology\n");
		exit(EXIT_FAILURE);
	}

	/* Get pointers to fileread and filewrite */
	pcm_dev = ipc_get_comp_by_id(sof.ipc, tp.fw_id);
	fwcd = comp_get_drvdata(pcm_dev->cd);
	pcm_dev = ipc_get_comp_by_id(sof.ipc, tp.fr_id);
	frcd = comp_get_drvdata(pcm_dev->cd);

	/* Run pipeline until EOF from fileread */
	pcm_dev = ipc_get_comp_by_id(sof.ipc, tp.sched_id);
	p = pcm_dev->cd->pipeline;
	ipc_pipe = &p->ipc_pipe;

	/* input and output sample rate */
	if (!tp.fs_in)
		tp.fs_in = ipc_pipe->period * ipc_pipe->frames_per_sched;

	if (!tp.fs_out)
		tp.fs_out = ipc_pipe->period * ipc_pipe->frames_per_sched;

	/* set pipeline params and trigger start */
	if (tb_pipeline_start(sof.ipc, ipc_pipe, &tp) < 0) {
		fprintf(stderr, "error: pipeline params\n");
		exit(EXIT_FAILURE);
	}

	cd = pcm_dev->cd;
	tb_enable_trace(false); /* reduce trace output */
	tic = clock();

	while (frcd->fs.reached_eof == 0)
		pipeline_schedule_copy(p, 0);

	if (!frcd->fs.reached_eof)
		printf("warning: possible pipeline xrun\n");

	/* reset and free pipeline */
	toc = clock();
	tb_enable_trace(true);
	pipeline_trigger(p, cd, COMP_TRIGGER_STOP);
	ret = pipeline_reset(p, cd);
	if (ret < 0) {
		fprintf(stderr, "error: pipeline reset\n");
		exit(EXIT_FAILURE);
	}

	n_in = frcd->fs.n;
	n_out = fwcd->fs.n;
	t_exec = (double)(toc - tic) / CLOCKS_PER_SEC;
	c_realtime = (double)n_out / TESTBENCH_NCH / tp.fs_out / t_exec;

	/* free all components/buffers in pipeline */
	free_comps();

	/* print test summary */
	printf("==========================================================\n");
	printf("		           Test Summary\n");
	printf("==========================================================\n");
	printf("Test Pipeline:\n");
	printf("%s\n", pipeline);
	printf("Input bit format: %s\n", tp.bits_in);
	printf("Input sample rate: %d\n", tp.fs_in);
	printf("Output sample rate: %d\n", tp.fs_out);
	printf("Output written to file: \"%s\"\n", tp.output_file);
	printf("Input sample count: %d\n", n_in);
	printf("Output sample count: %d\n", n_out);
	printf("Total execution time: %.2f us, %.2f x realtime\n",
	       1e3 * t_exec, c_realtime);

	/* free all other data */
	free(tp.bits_in);
	free(tp.input_file);
	free(tp.tplg_file);
	free(tp.output_file);

	/* close shared library objects */
	for (i = 0; i < NUM_WIDGETS_SUPPORTED; i++) {
		if (lib_table[i].handle)
			dlclose(lib_table[i].handle);
	}

	return EXIT_SUCCESS;
}
