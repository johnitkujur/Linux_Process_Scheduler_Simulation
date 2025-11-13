#include <bits/stdc++.h>
#include <csignal>
#include <unistd.h>

#include "time_utility.hpp"
#include "interrupt.hpp"
#include "process.hpp"

using namespace std;


static void thread_init(int total_thread, Process_Sched& proc_sched)
{
        vector<thread> threads;
	for(int i = 0; i < total_thread; i++) {
		threads.emplace_back(&Process_Sched::schedule, &proc_sched, i+1);
	}

	for(int i = 0; i < total_thread; i++) {
		threads[i].join();
	}
}


void my_signal_handler(int sig_num) {
        cout<<"Signal handler called\n";
        signal(SIGINT, my_signal_handler); 
        if (sig_num == SIGTSTP )
                interrupt_sched.add_interrupt();
}

void add_interrupt_test()
{
        this_thread::sleep_for(chrono::milliseconds(2000));
        interrupt_sched.add_interrupt();
        this_thread::sleep_for(chrono::milliseconds(2000));
        interrupt_sched.add_interrupt();
}

int main()
{
        Process_Sched proc_sched;
        int total_process = 8, total_thread;
        proc p_list[total_process];

        p_list[0] = {1, 0, 0, 0, 0, 7};
        p_list[1] = {2, 0, 1, 0, 0, 3};
        p_list[2] = {3, 0, 1, 0, 0, 10};
        p_list[3] = {4, 0, 1, 0, 0, 15};
        p_list[4] = {5, 0, 0, 0, 0, 1};
        p_list[5] = {6, 0, 0, 0, 0, 5};
	p_list[6] = {7, 0, 1, 0, 0, 6};
	p_list[7] = {8, 0, 0, 0, 0, 3};

        signal(SIGTSTP, my_signal_handler);

        cout<<"All Process:\n";
        cout<<"pid\t priority\t total_time_left\n";

        for (int i = 0; i < total_process; i++) {
                cout<<p_list[i].pid<<"\t"<<p_list[i].priority<<"\t\t"<<p_list[i].total_time_left<<"\n";
                proc_sched.add_process(&p_list[i]);
        }

	cout<<"Enter number of thread you want to simulate? (Min - 1)\n";
	cin>>total_thread;

	if(total_thread < 1) {
		cout<<"Error: minimum thread enter should be 1\n";
		return -1;
	}
	
	program_start_time = get_time();

        thread newThread(add_interrupt_test); 

	thread_init(total_thread, proc_sched);
        newThread.join(); 

        return 0;
}
