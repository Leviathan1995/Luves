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
#include <sys/socket.h>
#include <arpa/inet.h>

#include "net.h"
#include "buffer.h"
#include "eventhandle.h"
#include "channel.h"

namespace luves {

    class TcpConnection;
    class EventLoop;

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::function<void (const TcpConnectionPtr &)> TcpCallBack;

    //
    //TCP连接服务
    //
    class TcpConnection:public std::enable_shared_from_this<TcpConnection>
    {
    public:

        TcpConnection(EventLoop *loop):loop_(loop){};
        ~TcpConnection(){};

        void Register(EventLoop * loop,int fd,Ip4Addr local,Ip4Addr peer);

        //设置回调函数
        void SetReadCb(const TcpCallBack & cb){readcb_=cb;}
        void SetWriteCb(const TcpCallBack & cb){writecb_=cb;}
        void SetCloseCb(const TcpCallBack & cb){closecb_=cb;}

        //处理读写回调
        void HandleRead(const TcpConnectionPtr & con);
        void HandleWrite(const TcpConnectionPtr & con);

        //发送数据
        void Send(Buffer & msg);
        void Send(const std::string & msg);

        //获取读写缓冲区
        Buffer & GetInputBuffer(){return input_;}
        Buffer & GetOutputBuffer(){return output_;}

        //处理断开连接
        void Close();

        //获取事件循环
        EventLoop * GetLoop(){return loop_;};

    private:
        TcpCallBack readcb_,writecb_,closecb_;
        EventLoop * loop_;
        Channel * channel_;
        Buffer input_,output_;
    };


}
#endif /* tcp_connection_h */
