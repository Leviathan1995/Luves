//
//  threads.cpp
//  Luves
//
//  Created by leviathan on 16/5/18.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "threadpool.h"

namespace luves
{
    
    std::queue<int> ThreadsPool::task_;
    //std::vector<int> ThreadsPool::active_;
    
    std::map<int,Channel*>  * ThreadsPool::channel_fd_;
    
    pthread_mutex_t ThreadsPool::pthreadMutex_ =    PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t ThreadsPool::task_mutex_  =    PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t ThreadsPool::pthreadCond_   =    PTHREAD_COND_INITIALIZER;
    
    std::function<void (const TcpConnectionPtr &)> ThreadsPool::readcb_;
    std::function<void (const TcpConnectionPtr &)> ThreadsPool::writecb_;
    
    int ThreadsPool::thread_num_;
    
    ThreadsPool::~ThreadsPool()
    {
        delete [] pthreadId_;
        
    }
    
    //创建线程池
    void ThreadsPool::CreatePool()
    {
        pthreadId_ =new pthread_t[thread_num_];
        for (int i=0;i<thread_num_; i++)
            pthread_create(&pthreadId_[i], NULL, ThreadCallBack, NULL);
    }
    
    void * ThreadsPool::ThreadCallBack(void *data)
    {
        while (1)
        {
            int fd = -1;
            pthread_mutex_lock(&pthreadMutex_);
            while (task_.size()==0)
                pthread_cond_wait(&pthreadCond_,&pthreadMutex_);
            
            //获取已accept的Channel
            pthread_mutex_lock(&task_mutex_);
            fd = task_.front();
            task_.pop();
            pthread_mutex_unlock(&task_mutex_);
            pthread_mutex_unlock(&pthreadMutex_);
            
            if(channel_fd_->find(fd) != channel_fd_->end())
                channel_fd_->find(fd)->second->HandleEvent();
        }
    
    }

}
