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
    std::map<int, Channel *> KqueueModel::channel_fd_map_;


    void KqueueModel::AddChannel(Channel *channel)
    {
        if (channel->GetIsListen())         //获取listen套接字
            listen_fd_=channel->GetFd();
        channel_fd_map_[channel->GetFd()]=channel;
        EV_SET(&monitor_events[monitor_nums_], channel->GetFd(),channel->GetEvent(), EV_ADD|EV_ENABLE, 0, 0, NULL);
        monitor_nums_++;

    }

    void KqueueModel::DeleteChannel(Channel * channel)
    {
        channel_fd_map_.erase(channel->GetFd());
        EV_SET(&monitor_events[monitor_nums_], channel->GetFd(),channel->GetEvent(), EV_DELETE|EV_ENABLE, 0, 0, NULL);
        monitor_nums_--;
    }

    void KqueueModel::UpdateChannel(Channel * channel)
    {

    }

    void KqueueModel::RunModel(int64_t wait_time)
    {
        //struct timespec time_out={static_cast<__darwin_time_t>(0.001*waittime),0};

        int nev=kevent(kq_,monitor_events,monitor_nums_,trigger_events,1024,NULL);
        trigger_channel_list_.clear();

        for (int i=0; i<nev; i++)
        {
            if (trigger_events[i].flags & EV_ERROR)
            {
                ERROR_LOG("kqueue return error %d %s",errno,strerror(errno)); //kqueue error
                exit(1);
            }
            else if ((trigger_events[i].ident==listen_fd_ )||((trigger_events[i].flags & EVFILT_READ) && (is_hsha_==false)))
                //default or connnect coming
            {
                auto event=&trigger_events[i];
                auto channel=channel_fd_map_.find(int(event->ident))->second;
                channel->SetActiveEvent(event->flags);
                trigger_channel_list_.push_back(channel_fd_map_.find(int(event->ident))->second);
            }
            else if (trigger_events[i].flags & EVFILT_READ && is_hsha_)  //the server of model is hsha
            {
                ThreadsPool::AddTask(int(trigger_events[i].ident));
            }


        }
    }

    ChannelList & KqueueModel::GetTriggerPtr()
    {
        return trigger_channel_list_;
    }
}

#endif /* MAC OS X */
