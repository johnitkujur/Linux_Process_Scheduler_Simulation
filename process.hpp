#ifndef PROCESS_H
#define PROCESS_H

#include <bits/stdc++.h>

#include "time_utility.hpp"
#include "interrupt.hpp"

using namespace std;

#define NICE_VALUE 1024
#define IO_BOUND_TIME_SLICE 2
#define IO_BOUND_WAIT_TIME 2
#define CPU_BOUND_TIME_SLICE 5

/*
IO_BOUND have higher priority than CPU_BOUND
to make system more responsive, higher priority means more 
weight that will increase vrutime slowly
*/
enum process_type {
        CPU_BOUND = 0,
        IO_BOUND,
};

/*
In process cpu time we are storing
how many cpu tick we require
*/
typedef struct Process {
        int pid;
        int arrival_time;
        int completion_time;
        int priority;
        int cpu_time;
        int vruntime;
        int total_time_left;
}proc;

extern int weight[];

struct CompareRuntime {
    bool operator()(const proc* a, const proc* b) const {
        return a->vruntime > b->vruntime;  
    }
};

class Process_Sched {
private:
        priority_queue<proc*, vector<proc*>, CompareRuntime> active_process_list;
        map<int, proc*> suspended_process_list;
        queue<proc*> completed_process_list;
        int curr_time = 0;
        mutex suspended_process_list_mutex;
        mutex active_process_list_mutex;
        int total_process;
public:
        void add_process (proc *p);
        void requeue_process(proc *curr_proc, int runtime, int current_thread);
        proc* get_process_to_run();
        void wait_for_io(proc *curr_proc, int runtime, int current_thread);
        void process (proc *curr_proc, int time_slice, int current_thread);
        void schedule (int current_thread);
        void print_stats(void);

        Process_Sched(int tot_process)
        {
                total_process = tot_process;
        }
};

#endif