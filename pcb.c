#include "pcb.h"
#include <stdlib.h>
#include <stdio.h>

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
    printf("Process UID:         %d\n", this->uid);
    printf("Process status:      %d\n", this->state);
    printf("Process priority:    %d\n", this->priority);
    printf("Process requirement: %d\n", this->cores_required);
}

void update_state(pcb* this, states state){
    this->state = state;
}