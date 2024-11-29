/**
 * \file
 * \author Seungheon Jeon
 * \date 2024 Spring
 * \par CS250 Computer Graphics II
 * \copyright DigiPen Institute of Technology
 */

#include "JobSystem.hpp"

using namespace std;

namespace util
{
#if defined(CAN_USE_THREADS)
    JobSystem::JobSystem()
    {
        unsigned num_hw_threads = std::thread::hardware_concurrency();
        unsigned num_workers    = num_hw_threads > 1;

        for (unsigned i = 0; i < num_workers; i++)
        {
            workers.push_back(std::thread(&JobSystem::work, this));
        }
    }

    JobSystem::~JobSystem()
    {
        is_done = true;
        cv.notify_all();
        for (std::thread& worker : workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void JobSystem::work()
    {
        while (!is_done)
        {
            Job current_job;

            {
                std::lock_guard<std::mutex> lock(m);
                if (!que.empty())
                {
                    current_job = std::move(que.front());
                    que.pop();
                }
            }

            if (current_job)
            {
                current_job();
                --numberOfJobs;
            }
            else
            {
                std::unique_lock<std::mutex> lock(m);
                cv.wait(lock, [this] { return !workers.empty(); });
            }
        }
    }

    void JobSystem::DoJob(JobSystem::Job job)
    {
        ++numberOfJobs;
        {
            std::unique_lock<std::mutex> lock(m);
            que.push(job);
        }
        cv.notify_one();
    }

    void JobSystem::WaitUntilDone()
    {
        while (numberOfJobs > 0)
        {
            Job current_job;

            {
                std::lock_guard<std::mutex> lock(m);

                if (!que.empty())
                {
                    current_job = std::move(que.front());
                    que.pop();
                }
            }

            if (current_job)
            {
                current_job();
                --numberOfJobs;
            }
        }
    }

    bool JobSystem::IsDone() const
    {
        return numberOfJobs == 0;
    }

    void JobSystem::DoJobs(int how_many, JobSystem::ComputeAtIndex compute)
    {
        size_t size               = workers.size();
        int    elements_per_group = how_many / static_cast<int>(size);
        int    remainder          = how_many % static_cast<int>(size);
        int    start_index        = 0;

        for (size_t i = 0; i < size; ++i)
        {
            int task_total = elements_per_group;
            if (remainder != 0)
            {
                task_total += remainder;
                remainder = 0;
            }

            int end_index = start_index + task_total;

            DoJob(
                [=]()
                {
                    for (int j = start_index; j < end_index; ++j)
                    {
                        compute(j);
                    }
                });

            start_index = end_index;
        }
    }


#else

    void JobSystem::DoJob(JobSystem::Job job)
    {
        job();
    }

    void JobSystem::WaitUntilDone()
    {
    }

    void JobSystem::DoJobs(int how_many, JobSystem::ComputeAtIndex compute)
    {
        for (int i = 0; i < how_many; ++i)
            compute(i);
    }

    bool JobSystem::IsDone() const
    {
        return true;
    }
#endif
}
