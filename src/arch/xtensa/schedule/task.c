// SPDX-License-Identifier: BSD-3-Clause
//
// Copyright(c) 2017 Intel Corporation. All rights reserved.
//
// Author: Liam Girdwood <liam.r.girdwood@linux.intel.com>
//         Tomasz Lauda <tomasz.lauda@linux.intel.com>

/**
 * \file arch/xtensa/task.c
 * \brief Arch task implementation file
 * \authors Liam Girdwood <liam.r.girdwood@linux.intel.com>
 *          Tomasz Lauda <tomasz.lauda@linux.intel.com>
 */

#include <sof/lib/alloc.h>
#include <sof/lib/cache.h>
#include <sof/lib/cpu.h>
#include <sof/lib/wait.h>
#include <sof/schedule/edf_schedule.h>
#include <sof/schedule/schedule.h>
#include <sof/schedule/task.h>
#include <ipc/topology.h>
#include <config.h>
#include <xtensa/corebits.h>
#include <xtensa/xtruntime-frames.h>
#include <xtos-structs.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

enum task_state task_main_slave_core(void *data)
{
#if CONFIG_SMP
	/* main audio processing loop */
	while (1) {
		/* sleep until next IDC or DMA */
		wait_for_interrupt(0);
	}
#endif

	return SOF_TASK_STATE_COMPLETED;
}

struct task **task_main_get(void)
{
	struct core_context *ctx = (struct core_context *)cpu_read_threadptr();

	return &ctx->main_task;
}

volatile void *task_context_get(void)
{
	struct core_context *ctx = (struct core_context *)cpu_read_threadptr();

	return ctx->td.xtos_active_task;
}

void task_context_set(void *task_ctx)
{
	struct core_context *ctx = (struct core_context *)cpu_read_threadptr();

	ctx->td.xtos_active_task = task_ctx;
}

int task_context_alloc(void **task_ctx)
{
	*task_ctx = rzalloc(SOF_MEM_ZONE_SYS_RUNTIME, 0, SOF_MEM_CAPS_RAM,
			    sizeof(xtos_task_context));
	if (!*task_ctx)
		return -ENOMEM;
	return 0;
}

int task_context_init(void *task_ctx, void *entry, void *arg0, void *arg1,
		      int task_core, void *stack, int stack_size)
{
	xtos_task_context *ctx = task_ctx;
	UserFrame *sp;

	/* allocate stack if not provided */
	if (stack) {
		ctx->stack_base = stack;
		ctx->stack_size = stack_size;
	} else {
		ctx->stack_base = rballoc(0, SOF_MEM_CAPS_RAM,
					  SOF_TASK_DEFAULT_STACK_SIZE);
		if (!ctx->stack_base)
			return -ENOMEM;
		ctx->stack_size = SOF_TASK_DEFAULT_STACK_SIZE;
		ctx->flags |= XTOS_TASK_CONTEXT_OWN_STACK;
	}
	bzero(ctx->stack_base, ctx->stack_size);

	/* set initial stack pointer */
	sp = (UserFrame *)((char *)ctx->stack_base + ctx->stack_size -
			   sizeof(UserFrame));

	/* entry point */
	sp->pc = (uint32_t)entry;

	/* a1 is pointer to stack */
	sp->a1 = (uint32_t)sp;

	/* PS_WOECALL4_ABI - window overflow and increment enable
	 * PS_UM - user vector mode enable
	 */
	sp->ps = PS_WOECALL4_ABI | PS_UM;

	/* a6 and a7 are the first parameters */
	sp->a6 = (uint32_t)arg0;
	sp->a7 = (uint32_t)arg1;

	ctx->stack_pointer = sp;

	/* flush for slave core */
	if (cpu_is_slave(task_core))
		task_context_cache(ctx, CACHE_WRITEBACK_INV);

	return 0;
}

void task_context_free(void *task_ctx)
{
	xtos_task_context *ctx = task_ctx;

	if (ctx->flags & XTOS_TASK_CONTEXT_OWN_STACK)
		rfree(ctx->stack_base);

	ctx->stack_size = 0;
	ctx->stack_pointer = NULL;

	rfree(ctx);
}

void task_context_cache(void *task_ctx, int cmd)
{
	xtos_task_context *ctx = task_ctx;

	switch (cmd) {
	case CACHE_WRITEBACK_INV:
		dcache_writeback_invalidate_region(ctx->stack_base,
						   ctx->stack_size);
		dcache_writeback_invalidate_region(ctx, sizeof(*ctx));
		break;
	case CACHE_INVALIDATE:
		dcache_invalidate_region(ctx, sizeof(*ctx));
		dcache_invalidate_region(ctx->stack_base, ctx->stack_size);
		break;
	}
}
