/*
 * Copyright 2016, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef BENCHMARK_UTILISATION_H
#define BENCHMARK_UTILISATION_H

#include <config.h>
#include <arch/benchmark.h>
#include <benchmark/benchmark_utilisation_types.h>
#include <arch/api/constants.h>
#include <model/statedata.h>

#ifdef CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT
#include <armv/benchmark_irqHandler.h>
#endif /* CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT */

#ifdef CONFIG_BENCHMARK_TRACK_UTILISATION
extern bool_t benchmark_log_utilisation_enabled;
extern timestamp_t ksEnter;
extern timestamp_t benchmark_start_time;
extern timestamp_t benchmark_end_time;

void benchmark_track_utilisation_dump(void);

void benchmark_track_reset_utilisation(void);
/* Calculate and add the utilisation time from when the heir started to run i.e. scheduled
 * and until it's being kicked off
 */
static inline void benchmark_utilisation_switch(tcb_t *heir, tcb_t *next)
{
    /* Add heir thread utilisation */
    if (likely(benchmark_log_utilisation_enabled)) {

        /* Check if an overflow occured while we have been in the kernel */
        if (likely(ksEnter > heir->benchmark.schedule_start_time)) {

            heir->benchmark.utilisation += (ksEnter - heir->benchmark.schedule_start_time);

        } else {
#ifdef CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT
            heir->benchmark.utilisation += (0xFFFFFFFFU - heir->benchmark.schedule_start_time) + ksEnter;
            armv_handleOverflowIRQ();
#endif /* CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT */
        }

        /* Reset next thread utilisation */
        next->benchmark.schedule_start_time = ksEnter;

    }
}

static inline void benchmark_utilisation_kentry_stamp(void)
{
    ksEnter = timestamp();
}

/* Add the time between the last thread got scheduled and when to stop
 * benchmarks
 */
static inline void benchmark_utilisation_finalise(void)
{
    /* Add the time between when NODE_STATE(ksCurThread), and benchmark finalise */
    benchmark_utilisation_switch(NODE_STATE(ksCurThread), NODE_STATE(ksIdleThread));

    benchmark_end_time = ksEnter;
    benchmark_log_utilisation_enabled = false;
}

#endif /* CONFIG_BENCHMARK_TRACK_UTILISATION */
#endif /* BENCHMARK_UTILISATION_H */
