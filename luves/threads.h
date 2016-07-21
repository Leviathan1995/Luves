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
#include <pthread.h>
#include <vector>
#include <map>

#include "tcp_connection.h"

namespace luves
{
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
        
        
        static void AddTask(int accept_fd)
        {
            pthread_mutex_lock(&vector_mutex);
            accpet_list_.push_back(accept_fd);
            pthread_mutex_unlock(&vector_mutex);
            pthread_cond_signal(&pthread_cond);
        };
        
        void CreatePool();
        
        static void * ThreadCallBack(void * data);     //pthread_create()回调函数
        
        //static void SetReadCb(std::function<void (const TcpConnPtr &)> & cb){readcb_=cb;}
        //static void SetWriteCb(std::function<void (const TcpConnPtr &)> & cb){writecb_=cb;}
        
        static void SetTcpConnectionFdMapPtr(std::map<int,TcpConnectionPtr> * Tcpconnection_fd_map){Tcpconnection_fd_map_= Tcpconnection_fd_map_;}
        
        static void SetThreadsNum(int threads_num){threads_num_=threads_num;}
        
        static TcpConnectionPtr  GetTcpConnectionPtr(int fd);
    private:
        ThreadsPool(){};
        ~ThreadsPool();
        ThreadsPool & operator=(ThreadsPool const &);
        ThreadsPool(ThreadsPool const &);
        
        static std::map<int,TcpConnectionPtr>  * Tcpconnection_fd_map_;
        static std::vector<int > accpet_list_;                                  //任务队列,存放accept套接字
        static int threads_num_;
        pthread_t * pthread_id_;
        
        static pthread_mutex_t pthread_mutex;
        static pthread_mutex_t vector_mutex;                                    //任务队列锁
        static pthread_cond_t pthread_cond;
        static std::function<void (const TcpConnectionPtr &)> readcb_,writecb_;
    };
}

#endif /* threads_h */

