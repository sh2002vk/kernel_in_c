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
    int time;
} pcb;

void pcb_init(pcb* this, int uid, states state, int priority, int cores_required, int time);
void pcb_status(pcb* this);
void update_state(pcb* this, states state);

#endif