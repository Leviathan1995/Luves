//
//  event_handle.cpp
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "eventHandle.h"
#include "threads.h"
namespace luves {
    
    //
    //Event loop
    //
    EventLoop::EventLoop()
    {
        
        IOModel_=std::make_shared<EventModel>();
        timer_=std::make_shared<Timer>();
        
    }
    
    void EventLoop::SetHsha(bool hsha)
    {
        is_hsha_=hsha;
        IOModel_->SetHsha(is_hsha_);
    }
    
    void EventLoop::loop()
    {
        looping_=true;
        quit_=false;
        
        while(!quit_)
        {
            IOModel_->RunModel(timer_->Getnexttimeout());
            trigger_channels_= IOModel_->GetTriggerPtr();
            for (auto event:trigger_channels_)
                event->HandleEvent();
            
            //定时事件模块处理超时事件
            timer_->HandleTimeoutEvent();
        }
    }
    
    
    
    void EventLoop::AddChannel(Channel * channel)
    {
        IOModel_->AddChannel(channel);
    }
    
    void EventLoop::UpdateChannel(Channel * channel)
    {
        
    }
    
    void EventLoop::DeleteChannel(Channel * channel)
    {
        IOModel_->DeleteChannel(channel);
    }
    
    //定时事件操作
    TimerId EventLoop::startTimer(int64_t delaytime, const TimerTask & task,int64_t interval)
    {
        return timer_->startTimer(delaytime, task,interval);
    }
   
    bool EventLoop::stopTimer(TimerId timerid)
    {
        return timer_->stopTimer(timerid);
    }
    
    //
    //事件通道
    //
    
    Channel::~Channel()
    {
        Close();
    }
    
    //关闭通道
    void Channel::Close()
    {
        if (fd_>0)
        {
            loop_->GetIOModel()->DeleteChannel(this);
            
            close(fd_);
        }
    }
    
    void Channel::HandleEvent()
    {
        if (active_events_ & EVFILT_READ)
        {
            if (readcb_)
            {
                readcb_();
            }
        }
        if (active_events_ & EVFILT_WRITE)
        {
            if (writecb_)
            {
                writecb_();
            }
        }
    }
    
    bool Channel::ReadEnable()
    {
        return event_ & readevent;
    }
    
    bool Channel::WriteEnable()
    {
        return event_ & writeevent;
    }
    
    //
    //IO复用模型
    //
    std::map<int, Channel *> EventModel::channel_fd_map_;
    

    void EventModel::AddChannel(Channel *channel)
    {
        if (channel->GetIsListen())         //获取listen套接字
            listen_fd_=channel->GetFd();
        channel_fd_map_[channel->GetFd()]=channel;
        EV_SET(&monitor_events[monitor_nums_], channel->GetFd(),channel->GetEvent(), EV_ADD|EV_ENABLE, 0, 0, NULL);
        monitor_nums_++;

    }
    
    void EventModel::DeleteChannel(Channel * channel)
    {
        channel_fd_map_.erase(channel->GetFd());
        EV_SET(&monitor_events[monitor_nums_], channel->GetFd(),channel->GetEvent(), EV_DELETE|EV_ENABLE, 0, 0, NULL);
        monitor_nums_--;
    }
    
    void EventModel::UpdateChannel(Channel * channel)
    {
        
    }
    
    void EventModel::RunModel(int64_t waittime)
    {
        struct timespec tmout={static_cast<__darwin_time_t>(0.001*waittime),0};
        
        int nev=kevent(kq_,monitor_events,monitor_nums_,trigger_events,1024,NULL);
        trigger_channel_list_.clear();
        
        for (int i=0; i<nev; i++)
        {
            if (trigger_events[i].flags & EV_ERROR)
            {
                ERROR_LOG("kqueue return error %d %s",errno,strerror(errno)); //kqueue error
            }
            else if ((trigger_events[i].ident==listen_fd_ )||((trigger_events[i].flags & EVFILT_READ)&& isHSHA_==false))       //default or connnect coming
            {
                auto event=&trigger_events[i];
                auto channel=channel_fd_map_.find(int(event->ident))->second;
                channel->SetActiveEvent(event->flags);
                trigger_channel_list_.push_back(channel_fd_map_.find(int(event->ident))->second);
            }
            else if (trigger_events[i].flags & EVFILT_READ &&isHSHA_)  //HSHA
            {
                ThreadsPool::AddTask(int(trigger_events[i].ident));
            }
            
            
        }
    }
    
    ChannelList & EventModel::GetTriggerPtr()
    {
        return trigger_channel_list_;
    }

}