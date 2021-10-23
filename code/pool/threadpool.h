//
// Created by linfan on 2021/10/19.
//

#ifndef WEBSERVER_THREADPOOL_H
#define WEBSERVER_THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
class ThreadPool{
public:
    //explicit : 无法进行隐式类型转换
    explicit ThreadPool(size_t threadCount = 8): pool_(std::make_shared<Pool>()){
        assert(threadCount>0);
        for(size_t i=0;i<threadCount;i++)
        {
            std::thread([pool=pool_]{
                std::unique_lock<std::mutex> locker(pool->mtx);
                while(1){
                    if(!pool->tasks.empty())
                    {
                        auto task = std::move(pool->tasks.front());
                        pool->tasks.pop();
                        locker.unlock();
                        task();
                        locker.lock();
                    }
                    else if(pool->isClosed) break;
                    else pool->cond.wait(locker);
                }
            }).detach();
        }
    }
    ThreadPool() = default;

    ThreadPool(ThreadPool&&) = default;

    ~ThreadPool(){
        if(static_cast<bool>(pool_)){
            {
                std::lock_guard<std::mutex> locker(pool_->mtx);
                pool_->isClosed = true;
            }
            pool_->cond.notify_all();
        }
    }

    template<class F>
    void AddTask(F&& task){
        {
            std::lock_guard<std::mutex> locker(pool_->mtx);
            pool_->tasks.emplace(std::forward<F>(task));
        }
        pool_->cond.notify_one();
    }

private:
    struct Pool{    //线程池
        std::mutex mtx; //互斥锁
        std::condition_variable cond;   //条件变量
        bool isClosed;  //是否关闭
        std::queue<std::function<void()>> tasks;    //回调任务
    };
    std::shared_ptr<Pool> pool_;
};

#endif //WEBSERVER_THREADPOOL_H