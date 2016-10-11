//
//  tcp.h
//  Luves
//
//  Created by leviathan on 16/5/17.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_


#include <iostream>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <functional>
#include <fcntl.h>
#include <arpa/inet.h>

#include "connection.h"

namespace luves {
    
    //
    //TcpServer
    //
    class TcpServer
    {
    public:
        TcpServer(EventLoop *loop,Ip4Addr & addr):loop_(loop),addr_(addr),listenFd_(Socket::CreateNonBlockSocket()),listenning_(false),ip_(addr.GetIp()),port_(addr.GetPort())
            {
                SocketOp::SetReuseaddr(listenFd_);
                SocketOp::SetNonblock(listenFd_);
                listen_channel_=new Channel(loop,listenFd_,true);
                listen_channel_->SetReadCb([this]{HandleAccept();});
                
                INFO_LOG("Set server ip: %s", addr_.GetIp().c_str());
                INFO_LOG("Set server port: %d", addr_.GetPort());
            };
        virtual ~TcpServer();
        
        void Bind();
        void Listen();
        
    
        void NewChannel(int accept_fd);   //Server新建Channel
        
        void HandleAccept();
        
        void SetReadCb(const TcpCallBack & read_cb){read_cb_ = read_cb;}
        void SetWriteCb(const TcpCallBack & write_cb){write_cb_ = write_cb;}
        
        void  RunServer();
        
        struct sockaddr_in * GetServerAddrPointer(){return &serverAddr_;}
        
        EventLoop * GetLoop(){ return loop_;}
        
        std::map<int, Channel*>* GetChannelPtr() { return &channel_fd_;}
          
        void SetHsha(bool is_hsha)
        {
            is_hsha_ = is_hsha;
            loop_->SetHsha(is_hsha_);
        }
    private:
        
        std::map<int, Channel*> channel_fd_;
        struct sockaddr_in serverAddr_;
        std::string ip_;
        short port_;
        int listenFd_;
        bool listenning_, is_hsha_;
        Ip4Addr addr_;
        EventLoop * loop_;
        Channel * listen_channel_;
        TcpCallBack read_cb_,write_cb_;
    };
    
    
    //
    //Tcp客户端
    //
    class TcpClient
    {
    public:
        TcpClient(EventLoop *loop,Ip4Addr & server_addr):loop_(loop),server_addr_(server_addr),client_fd_(Socket::CreateNonBlockSocket()){};
        virtual ~TcpClient(){};
        
        void Bind();
        void HandleConnect();
        
        //设置回调函数
        void SetReadCb(const TcpCallBack & cb){readcb_=cb;}
        void SetWriteCb(const TcpCallBack & cb){writecb_=cb;}
        
        //启动Client
        void RunClient();
    private:
        short port_;
        int client_fd_;
        Ip4Addr server_addr_;
        EventLoop * loop_;
        Channel * client_channel_;
        TcpCallBack readcb_,writecb_;

    };
}

#endif /* tcp_h */
