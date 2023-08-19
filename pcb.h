#ifndef pcb_H
#define pcb_H

typedef enum {
    IDLE,
    RUNNING,
    KILLED,
    READY,
    NEW
} states;

typedef struct{
    int uid;
    states state;
    int priority;
    int cores_required;
} pcb;

void pcb_init(pcb* this, int uid, states state, int priority, int cores_required);
void pcb_status(pcb* this);

#endif