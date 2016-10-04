//
//  event_handle.cpp
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "eventhandle.h"

namespace luves {

    //
    //Event loop
    //
    EventLoop::EventLoop()
    {

        io_model_=std::make_shared<io_model>();
        timer_=std::make_shared<Timer>();

    }

    void EventLoop::SetHsha(bool hsha)
    {
        is_hsha_=hsha;
        io_model_->SetHsha(is_hsha_);
    }

    void EventLoop::loop()
    {
        looping_=true;
        quit_=false;

        while(!quit_)
        {
            io_model_->RunModel(timer_->GetNextTimeout());
            trigger_channels_= io_model_->GetTriggerPtr();
            for (auto event:trigger_channels_)
                event->HandleEvent();

            //定时事件模块处理超时事件
            timer_->HandleTimeoutEvent();
        }
    }



    void EventLoop::AddChannel(Channel * channel)
    {
        io_model_->AddChannel(channel);
    }

    void EventLoop::UpdateChannel(Channel * channel)
    {

    }

    void EventLoop::DeleteChannel(Channel * channel)
    {
        io_model_->DeleteChannel(channel);
    }

    //定时事件操作
    TimerId EventLoop::startTimer(int64_t delaytime, const TimerTask & task,int64_t interval)
    {
        return timer_->StartTimer(delaytime, task,interval);
    }

    bool EventLoop::stopTimer(TimerId timerid)
    {
        return timer_->StopTimer(timerid);
    }

}
