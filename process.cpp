#include "process.hpp"

int weight[] = {1024, 2048};

void Process_Sched::add_process(proc* p)
{
    //updating to get actual arrival time
    p->arrival_time = get_time_since_start();
    active_process_list.push(p);
}

void Process_Sched::requeue_process(proc *curr_proc, int runtime, int current_thread)
{
    curr_proc->cpu_time += runtime;
    curr_proc->total_time_left -= runtime;
    char buffer[256];

    if(!curr_proc->total_time_left) {
        snprintf(buffer, sizeof(buffer), 
                    "[%d][PROC] process %d finished processing\n", 
                    get_time_since_start(), curr_proc->pid);
        curr_proc->completion_time = get_time_since_start();
        completed_process_list.push(curr_proc);
        cout<<buffer;
        return;
    }

    active_process_list_mutex.lock();
    curr_proc->vruntime += curr_proc->cpu_time * (NICE_VALUE / weight[curr_proc->priority]);
    active_process_list.push(curr_proc);
    active_process_list_mutex.unlock();
}
    
proc* Process_Sched::get_process_to_run()
{
    proc *curr_proc = NULL;
    active_process_list_mutex.lock();

    if(!active_process_list.size())
        goto exit;

    curr_proc = active_process_list.top();
    active_process_list.pop();
        
exit:
    active_process_list_mutex.unlock();
    return curr_proc;
}

void Process_Sched::wait_for_io(proc *curr_proc, int runtime, int current_thread)
{
    char buffer[256];
    std::this_thread::sleep_for(chrono::milliseconds(IO_BOUND_WAIT_TIME*CPU_TICK));
    snprintf(buffer, sizeof(buffer), 
                "[%d][SUSPENDED] process %d I/O complete\n", 
                get_time_since_start(), curr_proc->pid);
    cout<<buffer;
    suspended_process_list_mutex.lock();
    suspended_process_list.erase(curr_proc->pid);
    suspended_process_list_mutex.unlock();
    requeue_process(curr_proc, runtime, current_thread);
}

void Process_Sched::process (proc *curr_proc, int time_slice, int current_thread)
{
    int runtime = 0;
    char buffer[256];
    while(runtime < time_slice){
        ++runtime;
        std::this_thread::sleep_for(chrono::milliseconds(CPU_TICK));
        if(interrupt_sched.check_for_interrupt()){
            snprintf(buffer, sizeof(buffer), 
                        "[%d][PROC][%d] Interrupt detected preempting process %d\n", 
                        get_time_since_start(), current_thread, curr_proc->pid);
            cout<<buffer;
            requeue_process(curr_proc, runtime, current_thread);
            interrupt_sched.handle_interrupt(current_thread);
            return;
        }
    }
    if(curr_proc->priority == IO_BOUND) {
        suspended_process_list_mutex.lock();
        suspended_process_list[curr_proc->pid] = curr_proc;
        suspended_process_list_mutex.unlock();
        snprintf(buffer, sizeof(buffer), 
                    "[%d][PROC][%d] Suspending process %d for I/0\n", 
                    get_time_since_start(), current_thread, curr_proc->pid);
        cout<<buffer;
        std::thread(&Process_Sched::wait_for_io, this, curr_proc, runtime, current_thread).detach();
    } else
        requeue_process(curr_proc, runtime, current_thread);
}

void Process_Sched::schedule (int current_thread)
{
    char buffer[256];
    while(completed_process_list.size() < total_process) {
        proc *curr_proc = get_process_to_run();
        if(!curr_proc) {
            snprintf(buffer, sizeof(buffer), 
                        "[%d][PROC][%d] Currently no active process left\n", 
                        get_time_since_start(), current_thread);
            cout<<buffer;
            std::this_thread::sleep_for(chrono::milliseconds(CPU_TICK));
            continue;
        }
        snprintf(buffer, sizeof(buffer), 
                    "[%d][PROC][%d] processing %d, priority %d, total time left %d\n", 
                    get_time_since_start(), current_thread, curr_proc->pid, curr_proc->priority, 
                    curr_proc->total_time_left);
        cout<<buffer;
        if(curr_proc->priority == CPU_BOUND) {
            process(curr_proc,  min(curr_proc->total_time_left, CPU_BOUND_TIME_SLICE), current_thread);
        }
        else {
            process(curr_proc,  min(curr_proc->total_time_left, IO_BOUND_TIME_SLICE), current_thread);
        }
    }

}

void Process_Sched::print_stats()
{
    int total_TAT = 0, total_waiting_time = 0;

    cout << "\n"<< left
         << setw(12) << "PID"
         << setw(12) << "Arrival"
         << setw(15) << "Completion"
         << setw(18) << "TurnAround(TAT)"
         << setw(15) << "WaitingTime"
         << "\n";

    cout << string(72, '-') << "\n";

    while (!completed_process_list.empty()) {
        proc* curr_proc = completed_process_list.front();
        completed_process_list.pop();

        int tat = curr_proc->completion_time - curr_proc->arrival_time;
        int wt  = tat - curr_proc->cpu_time;

        total_TAT += tat;
        total_waiting_time += wt;

        cout << left
             << setw(12) << curr_proc->pid
             << setw(12) << curr_proc->arrival_time
             << setw(15) << curr_proc->completion_time
             << setw(18) << tat
             << setw(15) << wt
             << "\n";
    }

    cout << string(72, '-') << "\n";
    cout << "Average Turn Around Time : " << (total_TAT * 1.0 / total_process) << "\n";
    cout << "Average Waiting Time     : " << (total_waiting_time * 1.0 / total_process) << "\n";
}
