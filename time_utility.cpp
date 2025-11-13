#include "time_utility.hpp"

using namespace std;

long long program_start_time=0;

long long get_time()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
                ).count();
}

int get_time_since_start()
{
    return (int)(get_time() - program_start_time);
}