/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright(c) 2019 Intel Corporation. All rights reserved.
 *
 * Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *         Ranjani Sridharan <ranjani.sridharan@linux.intel.com>
 */

#ifndef _COMMON_TPLG_H
#define _COMMON_TPLG_H

#include <sound/asoc.h>
#include <ipc/dai.h>
#include <kernel/tokens.h>

#define SOF_DEV 1
#define FUZZER_DEV 2

struct comp_info {
	char *name;
	int id;
	int type;
	int pipeline_id;
};

struct frame_types {
	char *name;
	enum sof_ipc_frame frame;
};

static const struct frame_types sof_frames[] = {
	/* TODO: fix topology to use ALSA formats */
	{"s16le", SOF_IPC_FRAME_S16_LE},
	{"s24le", SOF_IPC_FRAME_S24_4LE},
	{"s32le", SOF_IPC_FRAME_S32_LE},
	{"float", SOF_IPC_FRAME_FLOAT},
	/* ALSA formats */
	{"S16_LE", SOF_IPC_FRAME_S16_LE},
	{"S24_LE", SOF_IPC_FRAME_S24_4LE},
	{"S32_LE", SOF_IPC_FRAME_S32_LE},
	{"FLOAT_LE", SOF_IPC_FRAME_FLOAT},
};

/** \brief Types of processing components */
enum sof_ipc_process_type {
	SOF_PROCESS_NONE = 0,		/**< None */
	SOF_PROCESS_EQFIR,		/**< Intel FIR */
	SOF_PROCESS_EQIIR,		/**< Intel IIR */
	SOF_PROCESS_KEYWORD_DETECT,	/**< Keyword Detection */
	SOF_PROCESS_KPB,		/**< KeyPhrase Buffer Manager */
	SOF_PROCESS_CHAN_SELECTOR,	/**< Channel Selector */
	SOF_PROCESS_MUX,
	SOF_PROCESS_DEMUX,
};

struct sof_topology_token {
	uint32_t token;
	uint32_t type;
	int (*get_token)(void *elem, void *object, uint32_t offset,
			 uint32_t size);
	uint32_t offset;
	uint32_t size;
};

enum sof_ipc_frame find_format(const char *name);

int get_token_uint32_t(void *elem, void *object, uint32_t offset,
		       uint32_t size);

int get_token_comp_format(void *elem, void *object, uint32_t offset,
			  uint32_t size);

/* Buffers */
static const struct sof_topology_token buffer_tokens[] = {
	{SOF_TKN_BUF_SIZE, SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_buffer, size), 0},
	{SOF_TKN_BUF_CAPS, SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_buffer, caps), 0},
};

/* scheduling */
static const struct sof_topology_token sched_tokens[] = {
	{SOF_TKN_SCHED_PERIOD, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, period), 0},
	{SOF_TKN_SCHED_PRIORITY, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, priority), 0},
	{SOF_TKN_SCHED_MIPS, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, period_mips), 0},
	{SOF_TKN_SCHED_CORE, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, core), 0},
	{SOF_TKN_SCHED_FRAMES, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, frames_per_sched), 0},
	{SOF_TKN_SCHED_TIME_DOMAIN, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_pipe_new, time_domain), 0},
};

/* volume */
static const struct sof_topology_token volume_tokens[] = {
	{SOF_TKN_VOLUME_RAMP_STEP_TYPE, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_volume, ramp), 0},
	{SOF_TKN_VOLUME_RAMP_STEP_MS,
		SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_comp_volume, initial_ramp), 0},
};

/* SRC */
static const struct sof_topology_token src_tokens[] = {
	{SOF_TKN_SRC_RATE_IN, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_src, source_rate), 0},
	{SOF_TKN_SRC_RATE_OUT, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_src, sink_rate), 0},
};

/* ASRC */
static const struct sof_topology_token asrc_tokens[] = {
	{SOF_TKN_ASRC_RATE_IN, SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_comp_asrc, source_rate), 0},
	{SOF_TKN_ASRC_RATE_OUT, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_asrc, sink_rate), 0},
	{SOF_TKN_ASRC_ASYNCHRONOUS_MODE, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_asrc, asynchronous_mode), 0},
	{SOF_TKN_ASRC_OPERATION_MODE, SND_SOC_TPLG_TUPLE_TYPE_WORD,
		get_token_uint32_t,
		offsetof(struct sof_ipc_comp_asrc, operation_mode), 0},
};

/* EFFECT */
int get_token_process_type(void *elem, void *object, uint32_t offset,
			   uint32_t size);

static const struct sof_topology_token process_tokens[] = {
	{SOF_TKN_PROCESS_TYPE, SND_SOC_TPLG_TUPLE_TYPE_STRING,
		get_token_process_type,
		offsetof(struct sof_ipc_comp_process, type), 0},
};

/* Tone */
static const struct sof_topology_token tone_tokens[] = {
};

/* Generic components */
static const struct sof_topology_token comp_tokens[] = {
	{SOF_TKN_COMP_PERIOD_SINK_COUNT,
		SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_comp_config, periods_sink), 0},
	{SOF_TKN_COMP_PERIOD_SOURCE_COUNT,
		SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_comp_config, periods_source), 0},
	{SOF_TKN_COMP_FORMAT,
		SND_SOC_TPLG_TUPLE_TYPE_STRING, get_token_comp_format,
		offsetof(struct sof_ipc_comp_config, frame_fmt), 0},
};

/* PCM */
static const struct sof_topology_token pcm_tokens[] = {
	{SOF_TKN_PCM_DMAC_CONFIG, SND_SOC_TPLG_TUPLE_TYPE_WORD,
	 get_token_uint32_t,
	 offsetof(struct sof_ipc_comp_host, dmac_config), 0},
};

/* DAI */
enum sof_ipc_dai_type find_dai(const char *name);

int get_token_dai_type(void *elem, void *object, uint32_t offset,
		       uint32_t size);
static const struct sof_topology_token dai_tokens[] = {
	{SOF_TKN_DAI_TYPE, SND_SOC_TPLG_TUPLE_TYPE_STRING, get_token_dai_type,
		offsetof(struct sof_ipc_comp_dai, type), 0},
	{SOF_TKN_DAI_INDEX, SND_SOC_TPLG_TUPLE_TYPE_WORD, get_token_uint32_t,
		offsetof(struct sof_ipc_comp_dai, dai_index), 0},
	{SOF_TKN_DAI_DIRECTION, SND_SOC_TPLG_TUPLE_TYPE_WORD,
	get_token_uint32_t,
	offsetof(struct sof_ipc_comp_dai, direction), 0},
};

struct sof_dai_types {
	const char *name;
	enum sof_ipc_dai_type type;
};

struct sof_controls {
	struct snd_soc_tplg_mixer_control *mixer_ctl;
	struct snd_soc_tplg_enum_control *enum_ctl;
	struct snd_soc_tplg_bytes_control *bytes_ctl;
	char *priv_data;
};

int sof_parse_tokens(void *object,
		     const struct sof_topology_token *tokens,
		     int count, struct snd_soc_tplg_vendor_array *array,
		     int priv_size);
void sof_parse_string_tokens(void *object,
			     const struct sof_topology_token *tokens,
			     int count,
			     struct snd_soc_tplg_vendor_array *array);
void sof_parse_uuid_tokens(void *object,
			   const struct sof_topology_token *tokens,
			   int count,
			   struct snd_soc_tplg_vendor_array *array);
void sof_parse_word_tokens(void *object,
			   const struct sof_topology_token *tokens,
			   int count,
			   struct snd_soc_tplg_vendor_array *array);
int get_token_dai_type(void *elem, void *object, uint32_t offset,
		       uint32_t size);
enum sof_ipc_dai_type find_dai(const char *name);

int tplg_read_array(struct snd_soc_tplg_vendor_array *array, FILE *file);
int tplg_load_buffer(int comp_id, int pipeline_id, int size,
		     struct sof_ipc_buffer *buffer, FILE *file);
int tplg_load_pcm(int comp_id, int pipeline_id, int size, int dir,
		  struct sof_ipc_comp_host *host, FILE *file);
int tplg_load_dai(int comp_id, int pipeline_id, int size,
		  struct sof_ipc_comp_dai *comp_dai, FILE *file);
int tplg_load_pga(int comp_id, int pipeline_id, int size,
		  struct sof_ipc_comp_volume *volume, FILE *file);
int tplg_load_pipeline(int comp_id, int pipeline_id, int size,
		       struct sof_ipc_pipe_new *pipeline, FILE *file);
int tplg_load_controls(struct sof_controls *ctls, int num_kcontrols,
		       FILE *file);
void tplg_free_controls(struct sof_controls *ctls);
int tplg_load_src(int comp_id, int pipeline_id, int size,
		  struct sof_ipc_comp_src *src, FILE *file);
int tplg_load_asrc(int comp_id, int pipeline_id, int size,
		   struct sof_ipc_comp_asrc *asrc, FILE *file);
int tplg_load_mixer(int comp_id, int pipeline_id, int size,
		    struct sof_ipc_comp_mixer *mixer, FILE *file);
int tplg_load_process(int comp_id, int pipeline_id, int size,
		      struct sof_ipc_comp_process *process, FILE *file);
int tplg_load_graph(int num_comps, int pipeline_id,
		    struct comp_info *temp_comp_list, char *pipeline_string,
		    struct sof_ipc_pipe_comp_connect *connection, FILE *file,
		    int route_num, int count);

int load_pga(void *dev, int comp_id, int pipeline_id,
	     struct snd_soc_tplg_dapm_widget *widget);

int load_aif_in_out(void *dev, int comp_id, int pipeline_id,
		    struct snd_soc_tplg_dapm_widget *widget, int dir, void *tp);
int load_dai_in_out(void *dev, int comp_id, int pipeline_id,
		    struct snd_soc_tplg_dapm_widget *widget, int dir, void *tp);
int load_buffer(void *dev, int comp_id, int pipeline_id,
		struct snd_soc_tplg_dapm_widget *widget);
int load_pipeline(void *dev, int comp_id, int pipeline_id,
		  struct snd_soc_tplg_dapm_widget *widget,
		  int sched_id);
int load_src(void *dev, int comp_id, int pipeline_id,
	     struct snd_soc_tplg_dapm_widget *widget, void *params);
int load_asrc(void *dev, int comp_id, int pipeline_id,
	      struct snd_soc_tplg_dapm_widget *widget, void *params);
int load_mixer(void *dev, int comp_id, int pipeline_id,
	       struct snd_soc_tplg_dapm_widget *widget);
int load_process(void *dev, int comp_id, int pipeline_id,
		 struct snd_soc_tplg_dapm_widget *widget);
int load_widget(void *dev, int dev_type, struct comp_info *temp_comp_list,
		int comp_id, int comp_index, int pipeline_id, void *tp,
		int *sched_id, FILE *file);

void register_comp(int comp_type);
int find_widget(struct comp_info *temp_comp_list, int count, char *name);

#endif
