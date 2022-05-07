#pragma once

#include <vector>
#include <deque>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>

#include "FileReader.h"

namespace LineCounter
{
    using TTask = std::packaged_task<std::size_t()>;

    /**
    * @class TaskManager
    * @brief Class is intended for adding tasks into the queue, distributing tasks among threads and 
    * executing task.
    */
    class TaskManager final
    {
    public:
        explicit TaskManager(std::size_t numberThreads);
        ~TaskManager();

        TaskManager(const TaskManager&) = delete;
        TaskManager(TaskManager&&) = delete;
        TaskManager& operator=(const TaskManager&) = delete;
        TaskManager& operator=(TaskManager&&) = delete;

        /**
        * @brief Method is used for adding new task in the queue
        * @param task - The task for execution
        */
        void AddTask(TTask task);

        /**
        * @brief Waiting for all tasks to complete
        */
        void WaitAll() noexcept;

        /**
        * @brief Task Manager is shutting down its job
        */
        void Finish();

    private:
        void Run();

    private:
        std::deque<TTask> _tasks;           ///< queue of tasks
        std::vector<std::thread> _threads;  ///< working threads
        std::mutex _mutex;
        std::condition_variable _cv;
        bool _isFinished;                   ///< flag is true when the task manager is finishing its job, otherwise false
    };
}
