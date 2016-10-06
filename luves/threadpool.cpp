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
    
    std::vector<int > ThreadsPool::connect_fd_;
    std::map<int,TcpConnectionPtr>  * ThreadsPool::tcpConnectionFd_;
    pthread_mutex_t ThreadsPool::pthreadMutex_=PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t ThreadsPool::acceptMutex_=PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t ThreadsPool::pthreadCond_=PTHREAD_COND_INITIALIZER;
    
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
    
    TcpConnectionPtr ThreadsPool::GetTcpConnectionPtr(int fd)
    {
        return tcpConnectionFd_->find(fd)->second;
    }
    
    void * ThreadsPool::ThreadCallBack(void *data)
    {
        int fd;
        while (1)
        {
            pthread_mutex_lock(&pthreadMutex_);
            while (connect_fd_.size()==0)
                pthread_cond_wait(&pthreadCond_,&pthreadMutex_);
            
            //获取accept套接字
            pthread_mutex_lock(&acceptMutex_);
            auto fd_ptr = connect_fd_.begin();
            fd = *fd_ptr;
            if (fd_ptr != connect_fd_.end())
                connect_fd_.erase(fd_ptr);
            pthread_mutex_unlock(&acceptMutex_);
            pthread_mutex_unlock(&pthreadMutex_);

            //进行回调
            if (tcpConnectionFd_)
            {
                auto tcpconnnection=tcpConnectionFd_->find(fd)->second;
                tcpconnnection->HandleRead(tcpconnnection);
                tcpconnnection->HandleWrite(tcpconnnection);
            }

        }
    
    }

}
