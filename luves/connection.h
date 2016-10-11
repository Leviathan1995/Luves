//
//  tcp_connection.h
//  Luves
//
//  Created by leviathan on 16/6/21.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <iostream>
#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "net.h"
#include "buffer.h"
#include "eventhandle.h"
#include "channel.h"
#include "logger.h"

namespace luves {

    class TcpConnection;
    class EventLoop;
    class Channel;

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void (const TcpConnectionPtr &)> TcpCallBack;

    //
    //TCP连接服务
    //
    class TcpConnection:public std::enable_shared_from_this<TcpConnection>
    {
    public:

        TcpConnection(EventLoop *loop, int fd):loop_(loop), fd_(fd), is_close_(false){};
        ~TcpConnection(){};

        //设置回调函数
        void SetReadCb(const TcpCallBack & cb){readcb_=cb;}
        void SetWriteCb(const TcpCallBack & cb){writecb_=cb;}
        void SetCloseCb(const TcpCallBack & cb){closecb_=cb;}

        //处理读写回调
        void HandleRead();
        void HandleWrite();
        void HandleClose(const TcpConnectionPtr & conn);
        //发送数据
        void Send(Buffer & msg);
        void Send(const std::string & msg);

        //获取读写缓冲区
        Buffer & GetInputBuffer(){return input_;}
        Buffer & GetOutputBuffer(){return output_;}

        //断开连接
        void Close();

        //获取事件循环
        EventLoop * GetLoop(){return loop_;};

        bool IsClose(){return is_close_;}
    private:
        bool is_close_;     //连接是否已经断开
        int fd_;
        TcpCallBack readcb_,writecb_,closecb_;
        EventLoop * loop_;
        Buffer input_,output_;
    };

}
#endif /* tcp_connection_h */
