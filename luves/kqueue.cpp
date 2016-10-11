//
//  kqueue.cpp
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifdef __APPLE__

#include "kqueue.h"

namespace luves {

    //
    //IO复用模型-Kqueue
    //


    void KqueueModel::AddChannel(Channel *channel)
    {
        struct kevent ev;
        if (channel->GetIsListen())         //是否为listen套接字
        {
            listen_fd_=channel->GetFd();
            EV_SET(&ev, channel->GetFd(),read_event, EV_ADD|EV_ENABLE, 0, 0, NULL);
            INFO_LOG("Add the listen socket %d of channel to kqueue.", channel->GetFd());
        }
        else
        {
            EV_SET(&ev, channel->GetFd(), read_event|write_event, EV_ADD|EV_ENABLE|EV_ONESHOT, 0, 0, NULL);
            INFO_LOG("Add the EV_ONESHOT socket %d of channel to kqueue.", channel->GetFd());

        }
        //调用kevent(),更改设置
        kevent(kq_, &ev, 1 , NULL, 0, NULL);
        monitor_nums_++;

    }

    void KqueueModel::DeleteChannel(Channel * channel)
    {
        struct kevent ev;
        EV_SET(&ev, channel->GetFd(), read_event|write_event, EV_DELETE, 0, 0, NULL);
        //调用kevent(),更改设置
        kevent(kq_, &ev, 1, NULL, 0, NULL);
        INFO_LOG("Remove the socket %d of channel from kqueue.",channel->GetFd());
        monitor_nums_++;
    }

    // TODO
    void KqueueModel::UpdateChannel(Channel * channel)
    {

    }

    void KqueueModel::RunModel(int64_t wait_time)
    {
        //struct timespec time_out={static_cast<__darwin_time_t>(0.001*waittime),0};

        int nev = kevent(kq_,NULL, 0 ,trigger_events, 2048, NULL);
        trigger_channel_.clear();

        for (int i=0; i<nev; i++)
        {
            if (trigger_events[i].flags & EV_ERROR)
            {
                ERROR_LOG("kqueue return error (%d) %s",errno,strerror(errno)); //kqueue error
                exit(1);
            }
            else if ((trigger_events[i].ident == listen_fd_ )||((trigger_events[i].flags & EVFILT_READ) && (is_hsha_ == false))) //default mode or connnect coming
            {
                /*
                auto event = &trigger_events[i];

                //update channel_fd_: set active event
                if (channel_fd_->find(int(event->ident)) != channel_fd_->end())
                {
                    auto channel = channel_fd_->find(int(event->ident))->second;
                    channel_fd_->insert(std::make_pair(channel->GetFd(),channel));
                
                    //add to trigger_channel_
                 */
                trigger_channel_.push_back(channel_fd_->find(int(trigger_events[i].ident))->second);
    
            }
            else if (trigger_events[i].flags & EVFILT_READ && is_hsha_)  //the server of model is hsha
            {
                ThreadsPool::AddTask(int(trigger_events[i].ident));
            }
        }
    }

    ChannelList & KqueueModel::GetTriggerPtr()
    {
        return trigger_channel_;
    }
}

#endif /* MAC OS X */
