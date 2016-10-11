//
//  kqueue.h
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifdef __APPLE__

#ifndef EVENTMODEL_H_
#define EVENTMODEL_H_

#include <vector>
#include <map>
#include "eventhandle.h"
#include "channel.h"


namespace luves {

    class Channel;
    //
    //Event Model-Kqueue
    //
    typedef std::vector<Channel *> ChannelList;
    typedef std::map<int ,Channel*> ChannelMap;
    typedef std::vector<struct kevent*> MonitorEvents;


    class KqueueModel
    {
        public:
            KqueueModel():kq_(kqueue()),monitor_nums_(0){};
            ~KqueueModel(){};

            typedef std::vector<Channel *> ChannelList;
            typedef std::map<int ,Channel*> ChannelMap;

            //启动事件模型
            void RunModel(int64_t wait_time);

            //事件通道操作,只暴露给事件循环EventLoop模块操作
            void AddChannel(Channel * channel);
            void UpdateChannel(Channel * channel);
            void DeleteChannel(Channel * channel);

            ChannelList  & GetTriggerPtr();

            void SetHsha(bool is_hsha){is_hsha_=is_hsha;}

            void SetChannelPtr(std::map<int ,Channel*> * channel_fd){ channel_fd_= channel_fd; }
        private:
        
            bool is_hsha_;
            int monitor_nums_;                  //监听的事件总数
            struct kevent monitor_events[2048];
            struct kevent trigger_events[2048];
            std::map<int ,Channel*> * channel_fd_;      //channel与fd 映射
            ChannelList monitor_channel_;       //监听的channel集合
            ChannelList trigger_channel_;       //触发的channel集合
            int kq_;
            int listen_fd_;                     //监听套接字
    };
}

#endif /* kqueue_h */

#endif /* MAC OS X*/
