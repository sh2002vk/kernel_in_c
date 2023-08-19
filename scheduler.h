#ifndef scheduler_H
#define scheduler_H
#include "pcb.h"

typedef struct {
    int cores;
    int idle_cores;
    pcb  **core_holder;
    char **currently_running_processes;
    char **jobs_in_queue;
} scheduler;

scheduler* create_scheduler(scheduler* this, int cores, int max_processes, int max_jobs);
int add_job(scheduler* this, pcb* thread);
int add_to_queue(scheduler* this, pcb* thread);

/**
 * ADD JOBS FORMULA:
 * a) check if there are any open cores
 *      A) if open, add pcb to open core
 *      B) if all cores in use, proceed to b)
 * b) check if the curr pcb priority > any running pcb
 *      A) if curr priority > running priority, stop (least priority) running pcb
 *          A1) start curr pcb in the core that was modified
 *          A2) add the stopped pcb back to the job queue, maintaining job queue order
 *      B) if curr priority < all curr running pcbs, add curr pcb to job queue in order
 * **/

#endif