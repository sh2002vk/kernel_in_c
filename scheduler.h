#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "pcb.h"

typedef struct {
    int cores;
    int idle_cores;
    int queue_length;
    pcb **core_holder;
    pcb **jobs_in_queue;
} scheduler;

scheduler* create_scheduler(int cores, int max_jobs);
int add_job(scheduler* this, pcb* thread);
int add_to_queue(scheduler* this, pcb* thread);
int kill_job(scheduler* this, int uid);
void destroy_scheduler(scheduler* this);
void update_scheduler(scheduler* this);
void print_scheduler_status(scheduler* this);
void print_queue_status(scheduler* this);

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