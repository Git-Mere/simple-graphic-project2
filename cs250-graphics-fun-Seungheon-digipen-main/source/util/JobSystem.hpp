/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */
#pragma once

#include "environment/Environment.hpp"


#if defined(CAN_USE_THREADS)

// TODO - replace with your own JobSystem implementation from CS180
#    include <functional>
#    include <atomic>
#    include <condition_variable>
#    include <functional>
#    include <mutex>
#    include <queue>
#    include <thread>
namespace util
{
    class JobSystem
    {
    public:
        JobSystem();
        ~JobSystem();

        using Job = std::function<void(void)>;
        void DoJob(Job job);
        void WaitUntilDone();
        bool IsDone() const;

        using ComputeAtIndex = std::function<void(int)>;
        void DoJobs(int how_many, ComputeAtIndex compute);


        JobSystem(const JobSystem&)            = delete;
        JobSystem(JobSystem&&)                 = delete;
        JobSystem& operator=(const JobSystem&) = delete;
        JobSystem& operator=(JobSystem&&)      = delete;


    private:
        std::vector<std::thread> workers;
        std::queue<Job>          que;
        std::mutex               m;
        std::condition_variable  cv;
        std::atomic_bool         is_done      = false;
        std::atomic<int>         numberOfJobs = 0;

    private:
        void work();
    };
}

#else
#    include <functional>

namespace util
{

    class JobSystem
    {
    public:
        using Job = std::function<void(void)>;
        void DoJob(Job job);
        void WaitUntilDone();
        bool IsDone() const;

        using ComputeAtIndex = std::function<void(int)>;
        void DoJobs(int how_many, ComputeAtIndex compute);
    };
}

#endif
