//
//  tcp_connection.cpp
//  Luves
//
//  Created by leviathan on 16/6/21.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "tcpconnection.h"

namespace luves {
    
    
    //
    //TCP连接模块
    //
    
    //注册tcp连接为一个channel
    void TcpConnection::Register(EventLoop * loop,int fd,Ip4Addr local,Ip4Addr peer)
    {
        loop_=loop;
        channel_=new Channel(loop_,fd);
        channel_->SetEvent(EVFILT_READ|EVFILT_WRITE);
        loop_->AddChannel(channel_);
        TRACE_LOG(" TCP服务创建成功:%s--%s:%d",local.ToString().c_str(),peer.ToString().c_str(),fd);

        TcpConnectionPtr connection=shared_from_this();
        
        //channel的回调函数将系统缓冲区的内容读至自定义缓冲区buffer中,然后调用tcp连接的回调函数
        connection->channel_->SetReadCb([=]{connection->HandleRead(connection);});
        connection->channel_->SetWriteCb([=]{connection->HandleWrite(connection);});
    }
 
    void TcpConnection::Send(Buffer & msg)
    {
        if (channel_)
        {
            if (channel_->WriteEnable())
            {
                output_.Append(msg);
                output_.WriteImp(channel_->GetFd());
            }
        }
    }
    
    void TcpConnection::Send(const std::string & msg)
    {
        if (channel_) {
            if (channel_->WriteEnable())
            {
                output_.Append(msg);
                output_.WriteImp(channel_->GetFd());
            }
        }
    }
    
    void  TcpConnection::HandleRead(const TcpConnectionPtr & conn)
    {
        int n=input_.ReadImp(channel_->GetFd());
        if (n>0)
        {
            if (readcb_)
            {
                readcb_(shared_from_this());
            }
        }
        else if(n==0)
        {
            Close();
        }
        
    }
    
    void TcpConnection::HandleWrite(const TcpConnectionPtr & conn)
    {
        if (writecb_)
        {
            writecb_(shared_from_this());
        }
    }
    
    void TcpConnection::Close()
    {
        if (channel_) {
            TcpConnectionPtr connection=shared_from_this();
            connection->channel_->Close();
            this->GetInputBuffer().Clear();
        }
    }
}