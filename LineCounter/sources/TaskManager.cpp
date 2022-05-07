#include <iostream>

#include "TaskManager.h"

using namespace LineCounter;

TaskManager::TaskManager(std::size_t numberThreads)
    : _isFinished(false)
{
    _threads.reserve(numberThreads);
    for (std::size_t index = 0; index < numberThreads; ++index)
    {
        _threads.emplace_back(&TaskManager::Run, this);
    }
}

TaskManager::~TaskManager()
{
    Finish();
    WaitAll();
}

void TaskManager::AddTask(TTask task)
{
    std::lock_guard locker(_mutex);
    _tasks.emplace_back(std::move(task));
    _cv.notify_all();
}

void TaskManager::WaitAll() noexcept
{
    try
    {
        for (auto& thread : _threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception was occurred during TaskManager::WaitAll(). File: " << __FILE__ << " Line: " << __LINE__
            << " Reason: " << ex.what();
    }
}

void TaskManager::Finish()
{
    std::lock_guard<std::mutex> locker(_mutex);
    _isFinished = true;
    _cv.notify_all();
}

void TaskManager::Run()
{
    while (!(_isFinished && _tasks.empty()))
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]() { return !_tasks.empty() || _isFinished; });
        if (!_tasks.empty())
        {
            auto task = std::move(_tasks.front());
            _tasks.pop_front();

            lock.unlock();
            task();
        }
    }
}
