#include<vector>
#include<thread>
#include<iostream>
#include "ThreadSafeQueue.h"

#ifndef THREADPOOL_H
#define THREADPOOL_H

class ThreadPool
{
public:
    ThreadPool(int threads):thread_count(threads){}
    ~ThreadPool() {}
    void start()
    {
        if(threads.size() == 0)
        {
            try
            {
                for(auto i=0;i<thread_count;i++)
                    threads.push_back(std::thread(&ThreadPool::run,this));
            }
            catch(const std::system_error)
            {
                std::cout << "Error in creating thread pool " <<std::endl;
                throw;
            }
        }
    }
    void stop()
    {
        for(auto i=0;i<thread_count;i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
        threads.clear();
    }
    void addWork(std::function<void()> task)
    {
        workQueue.push(task);
    }


private:
    void run()
    {
        while(true)
        {
            std::function<void()> task;
            workQueue.wait_and_pop(task);
            if(task != nullptr)
                task();
        }
    }

    ThreadSafeQueue<std::function<void()>> workQueue;
    std::vector<std::thread> threads;
    const unsigned int thread_count;
};
#endif // THREADPOOL_H
