//
//  threads.cpp
//  Luves
//
//  Created by leviathan on 16/5/18.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "threads.h"

namespace luves
{
    
    std::vector<int > ThreadsPool::accpet_list_;
    std::map<int,TcpConnectionPtr>  * ThreadsPool::Tcpconnection_fd_map_;
    pthread_mutex_t ThreadsPool::pthread_mutex=PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t ThreadsPool::vector_mutex=PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t ThreadsPool::pthread_cond=PTHREAD_COND_INITIALIZER;
    
    std::function<void (const TcpConnectionPtr &)> ThreadsPool::readcb_;
    std::function<void (const TcpConnectionPtr &)> ThreadsPool::writecb_;
    
    int ThreadsPool::threads_num_;
    
    ThreadsPool::~ThreadsPool()
    {
        delete [] pthread_id_;
        
    }
    
    //创建线程池
    void ThreadsPool::CreatePool()
    {
        pthread_id_ =new pthread_t[threads_num_];
        for (int i=0;i<threads_num_; i++)
            pthread_create(&pthread_id_[i], NULL, ThreadCallBack, NULL);
    }
    
    TcpConnectionPtr ThreadsPool::GetTcpConnectionPtr(int fd)
    {
        return Tcpconnection_fd_map_->find(fd)->second;
    }
    
    void * ThreadsPool::ThreadCallBack(void *data)
    {
        int fd;
        while (1)
        {
            pthread_mutex_lock(&pthread_mutex);
            while (accpet_list_.size()==0)
                pthread_cond_wait(&pthread_cond,&pthread_mutex);
            
            //获取accept套接字
            pthread_mutex_lock(&vector_mutex);
            auto fd_ptr=accpet_list_.begin();
            fd=*fd_ptr;
            if (fd_ptr!=accpet_list_.end())
                accpet_list_.erase(fd_ptr);
            pthread_mutex_unlock(&vector_mutex);
        
            pthread_mutex_unlock(&pthread_mutex);

            //进行回调
            if (Tcpconnection_fd_map_)
            {
                auto tcpconnnection=Tcpconnection_fd_map_->find(fd)->second;
                tcpconnnection->HandleRead(tcpconnnection);
                tcpconnnection->HandleWrite(tcpconnnection);
            }

        }
    
    }

}