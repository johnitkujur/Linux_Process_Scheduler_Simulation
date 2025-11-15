#include <bits/stdc++.h>
#include <csignal>
#include <unistd.h>

#include "time_utility.hpp"
#include "interrupt.hpp"
#include "process.hpp"

using namespace std;

//For Windows set this to 1
#define WINDOWS_SYSTEM 1

bool compareArrivalTime(const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
}

void add_process_in_list(Process_Sched& proc_sched, vector<proc>& p_list)
{
        int it = 0;
        sort(p_list.begin(), p_list.end(), compareArrivalTime);
        
        while(it < p_list.size()) {
                int time_diff = p_list[it].arrival_time - get_time_since_start();
                if(time_diff > 0)
                        this_thread::sleep_for(chrono::milliseconds(time_diff));
                proc_sched.add_process(&p_list[it]);
                it++;
        }
}

#if WINDOWS_SYSTEM
void add_interrupt_test(void)
{
        this_thread::sleep_for(chrono::milliseconds(2000));
        interrupt_sched.add_interrupt();
        this_thread::sleep_for(chrono::milliseconds(2000));
        interrupt_sched.add_interrupt();
}
#else
void my_signal_handler(int sig_num) {
        if (sig_num == SIGTSTP )
                interrupt_sched.add_interrupt();
}
#endif

static void thread_init(int total_thread, Process_Sched& proc_sched, vector<proc>& p_list)
{
        vector<thread> threads;
	for(int i = 0; i < total_thread; i++) {
		threads.emplace_back(&Process_Sched::schedule, &proc_sched, i+1);
	}
        thread process_thread(add_process_in_list, std::ref(proc_sched), std::ref(p_list));


#if WINDOWS_SYSTEM
        thread interrupt_thread(add_interrupt_test);
        interrupt_thread.join();
#endif
        process_thread.join();

	for(int i = 0; i < total_thread; i++) {
		threads[i].join();
	}
}

void print_process_table(const vector<proc>& p_list)
{
    cout << left << setw(8) << "PID"
         << setw(12) << "Arrival"
         << setw(12) << "Priority"
         << setw(16) << "Total Time Left"
         << "\n";

    cout << string(70, '-') << "\n";

    for (const auto& p : p_list) {
        cout << left << setw(8) << p.pid
             << setw(12) << p.arrival_time
             << setw(12) << p.priority
             << setw(16) << p.total_time_left
             << "\n";
    }
}

int main()
{
#if !WINDOWS_SYSTEM
        signal(SIGTSTP, my_signal_handler);
#endif

        int total_thread;

        //Enter arrival time in ms
        vector<proc>p_list = 
        {       //pid   arrival_time    completion_time         priority        cpu_time        vruntime        total_time_left
                {1,     0,              0,                      0,              0,              0,              7},
                {2,     400,            0,                      1,              0,              0,              3},
                {3,     3000,           0,                      1,              0,              0,              10},
                {4,     500,            0,                      1,              0,              0,              15},
                {5,     100,            0,                      0,              0,              0,              1},
                {6,     0,              0,                      0,              0,              0,              5},
                {7,     200,            0,                      1,              0,              0,              6},
                {8,     0,              0,                      0,              0,              0,              3}
        };

        Process_Sched proc_sched(p_list.size());

        //NUMBER_OF_PROCESS = p_list.size();

        cout<<"All Process:\n";
        print_process_table(p_list);

	cout<<"Enter number of thread you want to simulate? (Min - 1)\n";
	cin>>total_thread;

	if(total_thread < 1) {
		cout<<"Error: minimum thread enter should be 1\n";
		return -1;
	}
	
	program_start_time = get_time();

        thread_init(total_thread, proc_sched, p_list);

        proc_sched.print_stats();
        
        return 0;
}
