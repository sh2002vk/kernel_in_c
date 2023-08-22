#include "scheduler.h"
#include "pcb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

scheduler* create_scheduler(int cores, int max_jobs) {
    scheduler* this = malloc(sizeof(scheduler));
    this->queue_length = max_jobs;
    this->cores = cores;
    this->idle_cores = cores;
    this->jobs_in_queue = malloc(max_jobs*sizeof(pcb*));
    this->core_holder = malloc(this->cores*sizeof(pcb*));

    // make sure these arrays are empty
    memset(this->jobs_in_queue, 0, max_jobs*sizeof(pcb*));
    memset(this->core_holder, 0, this->cores*sizeof(pcb*));
    return this;
}

int add_job(scheduler* this, pcb* thread) {
    int start = 0;
    if (this->idle_cores >= thread->cores_required) {
        // can be added to end of queue
        while (start < this->cores && this->core_holder[start] != 0) {
            start += 1;
        }
        for (int i = 0; i < thread->cores_required; i++) {
            this->core_holder[start] = thread;
            states new_state = RUNNING;
            update_state(thread, new_state);
            this->idle_cores -= 1;
        }
        return 1;
    } else {
        int replace_index = -1;  // the index to swap a thread with, if priority is higher
        int max_priority = 6;    // highest priority level available is 5, levels go from 1->5

        while (start < this->cores) {
            if (this->core_holder[start]->priority < thread->priority &&
            this->core_holder[start]->priority < max_priority &&
            this->core_holder[start]->cores_required == thread->cores_required) {
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
            for (int i = replace_index; i < replace_index+thread->cores_required; i++) {
                this->core_holder[i] = NULL;
            }

            // start new job and put it into core_holder, replacing the job above
            states runState = RUNNING;
            update_state(thread, runState);
            for (int i = replace_index; i < replace_index+thread->cores_required; i++) {
                this->core_holder[i] = thread;
            }
        }
    }
    return 1;
}

int add_to_queue(scheduler* this, pcb* thread) {
    int index = 0;

    // Find the appropriate position in the queue for the new PCB based on priority
    while (index < this->queue_length && this->jobs_in_queue[index] && this->jobs_in_queue[index]->priority > thread->priority) {
        index++;
    }

    if (index >= this->queue_length) {
        return 0;
    }

    // Now, shift the remaining jobs to make space for the new PCB
    for (int j = index; this->jobs_in_queue[j]; j++) {
        if (j+thread->cores_required > this->queue_length) {
            return 0;
        }
        this->jobs_in_queue[j + thread->cores_required] = this->jobs_in_queue[j];
    }

    // Insert the new PCB
    for(int i = index; i < index+thread->cores_required; i++){
        this->jobs_in_queue[i] = thread;
    }
    states idle = IDLE;
    update_state(thread, idle);  // waiting in queue

    return 1;  // Return 1 for success
}

int kill_job(scheduler* this, int uid) {
    int startIndex = 0;
    int found = 0;
    while (startIndex < this->cores && this->core_holder[startIndex]) {
        // look for pcb in current running processes
        if (this->core_holder[startIndex]->uid == uid) {
            // kill this process
            states killed = KILLED;
            update_state(this->core_holder[startIndex], killed);
            pcb* temp = this->core_holder[startIndex];
            for (int i = startIndex; i < this->cores - 1; i++) {  // Make sure i doesn't go out of bounds
                this->core_holder[i] = this->core_holder[i+temp->cores_required];
            }
            // Clear the last slot since we shifted everything left
            for (int i = 1; i < 1+temp->cores_required; i++) {
                this->core_holder[this->cores - i] = NULL;
            }
            found = 1;
            this->idle_cores += temp->cores_required;
            break;
        }
        startIndex += 1;
    }

    return found;
}

void print_scheduler_status(scheduler* this){
    printf("\nCURRENT SCHEDULER STATUS: \n");
    printf("Number of Cores:     %d\n", this->cores);
    printf("Free Cores:          %d\n", this->idle_cores);
    printf("Length of job queue: %d\n", this->queue_length);
    if (this->idle_cores != this->cores) {
        printf("\nCURRENT RUNNING PROCESSES:\n");
        for (int i = 0; i < this->cores && this->core_holder[i]; ++i) {
            printf("\n");
            pcb_status(this->core_holder[i]);
            printf("\n");
        }
    } else {
        printf("\nNO PROCESSES CURRENTLY RUNNING\n");
    }
}

void update_scheduler(scheduler* this) {
    // THIS WILL BE IMPLEMENTED LATER
}


void destroy_scheduler(scheduler* this) {
    free(this->core_holder);
    free(this->jobs_in_queue);
    free(this);
    this->core_holder = NULL;
    this->jobs_in_queue = NULL;
    this = NULL;
}