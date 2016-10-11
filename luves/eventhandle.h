//
//  event_handle.h
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef EVENT_HANDLE_H_
#define EVENT_HANDLE_H_

#ifdef __linux__
#include <sys/epoll.h>
#include "epoll.h"

#elif  __APPLE__
#include <sys/event.h>
#include "kqueue.h"
#endif


#include <memory>
#include <vector>
#include <utility>
#include <map>
#include <functional>
#include <unistd.h>

#include "timer.h"
#include "logger.h"
#include "channel.h"
#include "threadpool.h"

namespace luves {

#ifdef __linux__
    class EpollModel;
    typedef EpollModel io_model;
#elif __APPLE__
    class KqueueModel;
    typedef KqueueModel io_model;
#endif

    class Channel;

    //
    //事件循环
    //
    class EventLoop
    {
    public:
        typedef std::vector<Channel *> TriggerChannels;
        EventLoop();
        ~EventLoop(){};

        //进入事件循环
        void loop();

        //对通道进行操作
        void AddChannel(Channel * channel);
        void DeleteChannel(Channel *channel);
        void UpdateChannel(Channel *channel);

        //定时事件操作
        TimerId startTimer(int64_t delaytime,const TimerTask & task,int64_t interval=0);
        bool stopTimer(TimerId timerid);
        //获取IO模型指针
        std::shared_ptr<io_model> &  GetIOModel(){return io_model_;}
        
        void SetChannelPtr(std::map<int, Channel*>* channel_fd);

        void Exit();

        void SetHsha(bool hsha);

    private:
        
        std::map<int, Channel*> * channel_fd_;
        bool is_hsha_;
        TriggerChannels trigger_channels_;
        bool looping_;
        bool quit_;
        std::shared_ptr<io_model> io_model_;   //事件循环使用的IO复用模型
        std::shared_ptr<Timer> timer_;          //定时处理单元

    };

//监听事件类型
#ifdef __linux__
    const int read_event = EPOLLIN;
    const int write_event = EPOLLOUT;
#elif __APPLE__
    const int read_event = EVFILT_READ;
    const int write_event = EVFILT_WRITE;
#endif

}

#endif /* event_handle_h */
