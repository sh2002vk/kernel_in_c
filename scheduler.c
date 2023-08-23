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
    // Validate the inputs
    if (!this || !thread) {
        return 0;  // Return failure if inputs are invalid
    }

    // Check if we have enough idle cores to accommodate the new thread
    if (this->idle_cores >= thread->cores_required) {
        int allocated_cores = 0;
        for (int i = 0; i < this->cores && allocated_cores < thread->cores_required; i++) {
            if (this->core_holder[i] == NULL) {
                this->core_holder[i] = thread;
                allocated_cores++;
            }
        }
        states new_state = RUNNING;
        update_state(thread, new_state);
        this->idle_cores -= thread->cores_required;
        return 1;  // Successfully allocated the thread to cores
    }

    // If not enough idle cores, try to find a replaceable thread with the same core requirement and lower priority
    int max_priority = 6;  // Start with a priority higher than the maximum possible
    int replace_index = -1;
    for (int i = 0; i < this->cores; i++) {
        if (this->core_holder[i] &&
            this->core_holder[i]->priority < thread->priority &&
            this->core_holder[i]->priority < max_priority &&
            this->core_holder[i]->cores_required == thread->cores_required) {

            max_priority = this->core_holder[i]->priority;
            replace_index = i;
        }
    }

    if (replace_index != -1) {
        // Replace the thread at replace_index with the new thread
        states holdState = IDLE;
        update_state(this->core_holder[replace_index], holdState);
        add_to_queue(this, this->core_holder[replace_index]);
        for (int i = replace_index; i < replace_index + thread->cores_required; i++) {
            this->core_holder[i] = thread;
        }
        states runState = RUNNING;
        update_state(thread, runState);
        return 1;  // Successfully replaced and allocated the thread to cores
    } else {
        // If no thread is replaceable, add the new thread to the queue
        add_to_queue(this, thread);
        return 1;  // Successfully added the thread to the queue
    }
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

void print_queue_status(scheduler* this) {
    printf("\n");
    int i = 0;
    while (i < this->queue_length && this->jobs_in_queue[i]) {
        printf("\n");
        pcb_status(this->jobs_in_queue[i]);
        printf("\n");
    }
    if (i == 0) {
        printf("\nNO PROCESSES IN QUEUE\n");
    }
    printf("\n");
}

void update_scheduler(scheduler* this) {
    if (!this) return; // Safety check

    // Update running processes and free up completed ones
    for (int i = 0; i < this->cores; i++) {
        pcb* thread = this->core_holder[i];
        if (thread) {
            thread->time -= 1; // Decrease the time for the running process

            // If the thread's time is up, free the cores it occupied and nullify those core slots
            if (thread->time <= 0) {
                for (int j = 0; j < thread->cores_required; j++) {
                    this->core_holder[i + j] = NULL;
                    this->idle_cores++;
                }
                // Move the index by cores_required - 1 since those threads are identical and already checked
                i += thread->cores_required - 1;

                // Update the thread's state as KILLED (or you could have another state like COMPLETED)
                update_state(thread, KILLED);
                free(thread); // Free the memory for the thread
            }
        }
    }

    // Try to move jobs from the queue to the cores if cores are available
    int q_index = 0; // Index to traverse the queue
    while (this->idle_cores > 0 && q_index < this->queue_length && this->jobs_in_queue[q_index]) {
        pcb* thread_from_queue = this->jobs_in_queue[q_index];

        // If idle cores are sufficient for the thread from queue
        if (this->idle_cores >= thread_from_queue->cores_required) {
            // Find the first available core slot
            int core_slot = 0;
            while (core_slot < this->cores && this->core_holder[core_slot]) { // Make sure not to overflow core_holder
                core_slot++;
            }

            // Ensure we don't overflow the core_holder array
            if (core_slot + thread_from_queue->cores_required > this->cores) {
                break;  // Exit the loop if adding the thread would result in overflow
            }

            // Assign the thread to the required number of cores
            for (int j = 0; j < thread_from_queue->cores_required; j++) {
                this->core_holder[core_slot + j] = thread_from_queue;
            }
            update_state(thread_from_queue, RUNNING);
            this->idle_cores -= thread_from_queue->cores_required;

            // Remove the thread from the queue and shift other threads up
            for (int j = q_index; j < this->queue_length - 1; j++) {
                this->jobs_in_queue[j] = this->jobs_in_queue[j + 1];
            }
            this->jobs_in_queue[this->queue_length - 1] = NULL; // Set the last position to NULL
        }
        q_index++;
    }
}


void destroy_scheduler(scheduler* this) {
    free(this->core_holder);
    free(this->jobs_in_queue);
    free(this);
    this->core_holder = NULL;
    this->jobs_in_queue = NULL;
    this = NULL;
}