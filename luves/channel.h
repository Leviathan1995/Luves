//
//  channel.hpp
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "eventhandle.h"

//
//事件通道,接管一个fd
//

namespace luves {

    class EventLoop;
    class Channel
    {
        public:
            Channel(EventLoop * loop,int fd):loop_(loop),fd_(fd),is_listen_(false){};
            ~Channel();

            //处理事件
            void HandleEvent();

            //设置回调函数
            void SetReadCb(const std::function<void ()> & cb){readcb_=cb;}
            void SetWriteCb(const std::function<void ()> & cb){writecb_=cb;}
            //关闭通道
            void Close();

            //可读可写的判断
            bool ReadEnable();
            bool WriteEnable();

            //获取事件描述符
            int GetFd(){return fd_;}
            //获取监听类型
            int GetEvent(){return event_;}
            //设置监听IO类型
            void SetEvent(int event){event_=event;}
            //设置活动事件的IO类型
            void SetActiveEvent(ushort active_event){active_events_ = active_event;}

            //设置is_listen_
            void SetIsListen(bool listen){is_listen_=listen;}
            bool GetIsListen(){return is_listen_;}

        private:
            bool is_listen_;        //是否为listen监听套接字
            EventLoop * loop_;
            int fd_;                //事件描述符
            int event_;             //监听的IO事件类型
            ushort active_events_;  //当前活动事件的IO类型
            std::function<void ()> readcb_,writecb_; //读写回调函数

    };

}
#endif /* channel_hpp */
