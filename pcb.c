#include "pcb.h"
#include <stdlib.h>
#include <stdio.h>

void print_state(states state) {
    switch (state) {
        case IDLE:
            printf("IDLE");
            break;
        case RUNNING:
            printf("RUNNING");
            break;
        case KILLED:
            printf("KILLED");
            break;
        case NEW:
            printf("NEW");
            break;
        default:
            printf("UNKNOWN STATE");
    }
}


pcb* pcb_init(int uid, int priority, int cores_required, int time){
    pcb* this = malloc(sizeof(pcb));
    this->uid = uid;
    states new = NEW;
    this->state = new;
    this->priority = priority;
    this->cores_required = cores_required;
    this->time = time;
    return this;
}

void pcb_status(pcb* this){
    printf("Process UID:      %d\n", this->uid);
    printf("Process status:   ");
    print_state(this->state);
    printf("\n");
    printf("Process priority: %d\n", this->priority);
    printf("Process cores:    %d\n", this->cores_required);
    printf("Time left:        %d\n", this->time);
}

void update_state(pcb* this, states state){
    this->state = state;
}