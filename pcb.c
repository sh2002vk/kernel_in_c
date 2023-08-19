#include "pcb.h"
#include <stdio.h>

void pcb_init(pcb* this, int uid, states state, int priority, int cores_required){
    this->uid = uid;
    this->state = state;
    this->priority = priority;
    this->cores_required = cores_required;
}

void pcb_status(pcb* this){
    printf("Process UID:         %s\n", this->uid);
    printf("Process status:      %s\n", this->state);
    printf("Process priority:    %s\n", this->priority);
    printf("Process requirement: %s\n", this->cores_required);
}