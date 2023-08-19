#include "pcb.h"
#include <stdio.h>

void pcb_init(pcb* this, int uid, states state, int priority, int cores_required, int time){
    this->uid = uid;
    this->state = state;
    this->priority = priority;
    this->cores_required = cores_required;
    this->time = time;
}

void pcb_status(pcb* this){
    printf("Process UID:         %s\n", this->uid);
    printf("Process status:      %s\n", this->state);
    printf("Process priority:    %s\n", this->priority);
    printf("Process requirement: %s\n", this->cores_required);
}

void update_state(pcb* this, states state){
    this->state = state;
}