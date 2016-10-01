//
//  eventmodel.hpp
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifndef EVENTMODEL_H_
#define EVENTMODEL_H_

#include <vector>
#include <map>
#include "eventhandle.h"
#include "channel.h"
using namespace luves;



namespace luves {


    class Channel;

    //
    //Event Model-Kqueue
    //
    typedef std::vector<Channel *> ChannelList;
    typedef std::map<int ,Channel*> ChannelMap;
    typedef std::vector<struct kevent*> MonitorEvents;

    extern int monitor_nums;


    class KqueueModel
    {
        public:
            KqueueModel():kq_(kqueue()),monitor_nums_(0){};
            ~KqueueModel(){};

            typedef std::vector<Channel *> ChannelList;
            typedef std::map<int ,Channel*> ChannelMap;

            //启动事件模型
            void RunModel(int64_t waittime);

            //事件通道操作,只暴露给事件循环EventLoop模块操作
            void AddChannel(Channel * channel);
            void UpdateChannel(Channel * channel);
            void DeleteChannel(Channel * channel);

            ChannelList  & GetTriggerPtr();

            void SetHsha(bool is_hsha){is_hsha_=is_hsha;}

            //传递线程池指针
            //void PassPoolPtr(ThreadsPool *  pool){pool_=pool;}
        private:
            //ThreadsPool * pool_;
            bool is_hsha_;
            bool isLF_;
            int monitor_nums_;                  //监听的事件总数
            struct kevent monitor_events[1024];
            struct kevent trigger_events[1024];
            static ChannelMap channel_fd_map_;  //channel与fd 映射
            ChannelList channel_list_;          //channel集合数组
            ChannelList trigger_channel_list_;  //触发的Channel数组
            int kq_;
            int listen_fd_;                     //监听套接字
    };
}

#endif /* eventmodel_h */
