#include "scheduler.h"
#include "pcb.h"
#include <stdlib.h>
#include <string.h>

scheduler* create_scheduler(scheduler* this, int cores, int max_processes, int max_jobs) {
    this->cores = cores;
    this->idle_cores = cores;
    this->currently_running_processes = malloc(max_processes*sizeof(char*));
    this->jobs_in_queue = malloc(max_jobs*sizeof(char*));
    this->core_holder = malloc(this->cores*sizeof(pcb*));

    // make sure these arrays are empty
    memset(this->currently_running_processes, 0, max_processes*sizeof(char*));
    memset(this->jobs_in_queue, 0, max_jobs*sizeof(char*));
    memset(this->core_holder, 0, this->cores*sizeof(pcb*));
    return this;
}

int add_job(scheduler* this, pcb* thread) {
    int start = 0;
    if (this -> idle_cores > 0) {
        // can be added to end of queue
        while (start < this->cores && this->core_holder[start] != 0) {
            start += 1;
        }
        this->core_holder[start] = thread;
        states new_state = RUNNING;
        update_state(thread, new_state);
        this->idle_cores -= 1;
        return 1;
    } else {
        int replace_index = -1;  // the index to swap a thread with, if priority is higher
        int max_priority = 6;    // highest priority level available is 5, levels go from 1->5

        while (start < this->cores) {
            if (this->core_holder[start]->priority < thread->priority &&
            this->core_holder[start]->priority < max_priority) {
                max_priority = this->core_holder[start]->priority;
                replace_index = start;
            }
            start += 1;
        }

        if (replace_index == -1) {
            // no replacement found, add to job queue
            add_to_queue(this, thread);
        } else {
            // replace with lowest eligible priority running pcb

            // stop lowest job and send to queue
            states holdState = IDLE;
            update_state(this->core_holder[replace_index], holdState);
            add_to_queue(this, this->core_holder[replace_index]);

            // start new job and put it into core_holder, replacing the job above
            states runState = RUNNING;
            this->core_holder[replace_index] = thread;
            update_state(this->core_holder[replace_index], runState);
        }
    }
    return 1;
}

int add_to_queue(scheduler* this, pcb* thread) {
    int index = 0;

    // Find the appropriate position in the queue for the new PCB based on priority
    while (this->jobs_in_queue[index] && this->jobs_in_queue[index]->priority > thread->priority) {
        index++;
    }

    // Now, shift the remaining jobs to make space for the new PCB
    for (int j = index; this->jobs_in_queue[j]; j++) {
        this->jobs_in_queue[j + 1] = this->jobs_in_queue[j];
    }

    // Insert the new PCB
    this->jobs_in_queue[index] = thread;

    return 1;  // Return 1 for success
}
