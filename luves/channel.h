//
//  channel.h
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include "eventhandle.h"
#include "net.h"
#include "connection.h"

//
//事件通道,接管一个fd
//

namespace luves {

    class EventLoop;
    class TcpConnection;
    
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    
    class Channel
    {
        public:
            Channel(EventLoop * loop, int fd, bool is_listen);
            ~Channel(){};

            //处理事件
            void HandleEvent();

            //设置回调函数
            void SetReadCb(const std::function<void ()> & cb){read_cb_ = cb;}
            void SetWriteCb(const std::function<void ()> & cb){write_cb_ = cb;}
            //关闭通道
            void Close();

            //获取事件描述符
            int GetFd(){return fd_;}
        
            bool GetIsListen(){return is_listen_;}
        
            TcpConnectionPtr GetConnectionPtr(){return connection_;}
        private:
            bool is_listen_;                            //是否为listen监听套接字的channel
            EventLoop * loop_;
            int fd_;                                    //事件描述符
            std::function<void ()> read_cb_, write_cb_; //读写回调函数
            TcpConnectionPtr connection_;               //channel管理的连接

    };

}
#endif /* channel_hpp */
