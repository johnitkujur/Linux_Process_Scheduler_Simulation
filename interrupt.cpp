#include "interrupt.hpp"
#include "process.hpp"

Interrupt_Sched interrupt_sched;

int Interrupt_Sched::check_for_interrupt(void)
{
    return interrupt_list.size();
}

void Interrupt_Sched::handle_interrupt(int current_thread)
{
    interrupt_mutex.lock();
    char buffer[256];
    while(interrupt_list.size()){
        interrupt curr_interrupt = interrupt_list.front();
        interrupt_list.pop();
        snprintf(buffer, sizeof(buffer), 
                    "[%d][INT][%d] processing Interrupt %d\n", 
                    get_time_since_start(), current_thread, curr_interrupt.interrupt_id);
        cout<<buffer;
        std::this_thread::sleep_for(chrono::milliseconds(INTERRUPT_TIME_SLICE*CPU_TICK));
        snprintf(buffer, sizeof(buffer), 
                    "[%d][INT][%d] Interrupt %d handled\n", 
                    get_time_since_start(), current_thread, curr_interrupt.interrupt_id);
        cout<<buffer;
    }
    interrupt_mutex.unlock();
}

void Interrupt_Sched::add_interrupt(void)
{
    interrupt_mutex.lock();
    interrupt curr_int;
    curr_int.interrupt_id = ++interrupt_id;
    curr_int.arrival_time = get_time_since_start();
    interrupt_list.push(curr_int);
    interrupt_mutex.unlock();
}