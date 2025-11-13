#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <bits/stdc++.h>
#include <ctime>

#include "time_utility.hpp"

using namespace std;

//Assuming CPU tick is 10 milliseconds
#define CPU_TICK 100
#define INTERRUPT_TIME_SLICE 2

typedef struct Interrupt {
        int interrupt_id;
        int arrival_time;
}interrupt;

class Interrupt_Sched {
private:
        queue<interrupt*>interrupt_list;
        mutex interrupt_mutex;
        int interrupt_id = 0;
public:
        int check_for_interrupt(void);
        void handle_interrupt(int current_thread);
        void add_interrupt(void);

        Interrupt_Sched()
        {
                cout<<"Constructor called\n";
                interrupt_id = 0;
        }
};

extern Interrupt_Sched interrupt_sched;

#endif