//
//  threads.h
//  Luves
//
//  Created by leviathan on 16/5/18.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef THREADS_H_
#define THREADS_H_

#include <functional>
#include <memory>
#include <pthread.h>
#include <vector>
#include <map>
#include <queue>
#include "connection.h"
#include "logger.h"

namespace luves
{
 
    class TcpConnection;
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void (const TcpConnectionPtr &)> TcpCallBack;
    //
    //
    //线程池模块
    //Singleton模式
    //
    
    class ThreadsPool
    {
    public:
        static ThreadsPool & Instance()
        {
            static ThreadsPool pool;
            return pool;
        }
        
        
        static void AddTask(int fd)
        {
            pthread_mutex_lock(&task_mutex_);
            task_.push(fd);
            pthread_mutex_unlock(&task_mutex_);
            pthread_cond_signal(&pthreadCond_);
        };
        
        void CreatePool();
        
        static void * ThreadCallBack(void * data);     //pthread_create()回调函数
        
        static void SetReadCb(std::function<void (const TcpConnectionPtr &)> & cb){readcb_=cb;}
        static void SetWriteCb(std::function<void (const TcpConnectionPtr &)> & cb){writecb_=cb;}
        
        static void SetChannelPtr(std::map<int, Channel*> * channel_fd)
        {
            channel_fd_ = channel_fd;
        }
    
        static void SetThreadNum(int thread_num)
        {
            thread_num_ = thread_num;
            INFO_LOG("Set the number of thread : %d", thread_num_);
        }
        
    private:
        ThreadsPool(){};
        ~ThreadsPool();
        ThreadsPool & operator=(ThreadsPool const &);
        ThreadsPool(ThreadsPool const &);
           
        static std::map<int, Channel*>  * channel_fd_;
        static std::queue<int> task_;
        static int thread_num_;
        pthread_t * pthreadId_;
        
        static pthread_mutex_t pthreadMutex_;
        static pthread_mutex_t task_mutex_;
        static pthread_cond_t pthreadCond_;
        static std::function<void (const TcpConnectionPtr &)> readcb_,writecb_;
    };
}

#endif /* threads_h */

