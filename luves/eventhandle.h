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
#elif  __APPLE__
#include <sys/event.h>
#endif


#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <unistd.h>

#include "timer.h"
#include "logger.h"
#include "kqueue.h"
#include "channel.h"
#include "threadpool.h"

namespace luves {

#ifdef __linux__
    class EpollModel;
#elif __APPLE__
    class KqueueModel;
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
        std::shared_ptr<KqueueModel> &  GetIOModel(){return io_model_;}

        void Exit();

        void SetHsha(bool hsha);

    private:
        bool is_hsha_;
        TriggerChannels trigger_channels_;
        bool looping_;
        bool quit_;
        std::shared_ptr<KqueueModel> io_model_;   //事件循环使用的IO复用模型
        std::shared_ptr<Timer> timer_;          //定时处理单元

    };

//监听事件类型
#ifdef __linux__
    const int readevent = EPOLLIN;
    const int readevent = EPOLLOUT;
#elif __APPLE__
    const int readevent=EVFILT_READ;
    const int writeevent=EVFILT_WRITE;
#endif

}

#endif /* event_handle_h */
