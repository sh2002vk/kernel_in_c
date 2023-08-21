#ifndef PCB_H
#define PCB_H

typedef enum {
    IDLE,       // waiting in queue
    RUNNING,    // already running
    KILLED,     // thread that was killed
    NEW         // new thread not added to a queue
} states;

typedef struct{
    int uid;
    states state;
    int priority;
    int cores_required;
    int time;
} pcb;

pcb* pcb_init(int uid, int priority, int cores_required, int time);
void pcb_status(pcb* this);
void update_state(pcb* this, states state);

#endif