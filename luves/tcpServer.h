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

#include "tcpConnection.h"


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
                listenChannel_=new Channel(loop,listenFd_);

            };
        ~TcpServer();
        
        void Bind();
        void Listen();
        
    
        void NewConnection(int accept_fd,Ip4Addr local,Ip4Addr peer);
        
        //handle accept connection
        void HandleAccept();
        
        void SetReadCb(const TcpCallBack & cb){readcb_=cb;}
        void SetWriteCb(const TcpCallBack & cb){writecb_=cb;}
        void SetCloseCb(const TcpCallBack & cb){closecb_=cb;}
        
        void  RunServer();
        
        struct sockaddr_in * GetServerAddrPointer(){return &serverAddr_;}
        
        EventLoop * GetLoop(){ return loop_;}
        
        //set HSHA model
        void SetHSHA(bool hsha){isHSHA_=hsha;loop_->SetHsha(isHSHA_);}
        void SetLF(bool lf){isHF_=lf;}
        std::map<int,TcpConnectionPtr> * GetTcpConnMap(){return &tcpConnectionFd_;}
    private:
        
        std::map<int,TcpConnectionPtr> tcpConnectionFd_;
        bool isHSHA_  ;
        bool isHF_;
        struct sockaddr_in serverAddr_;
        std::string ip_;
        short port_;
        int listenFd_;
        bool listenning_;
        Ip4Addr addr_;
        EventLoop * loop_;
        Channel * listenChannel_;
        TcpCallBack readcb_,writecb_,closecb_;
    };
    
    //
    //Tcp客户端
    //
    class TcpClient
    {
    public:
        TcpClient(EventLoop *loop,Ip4Addr & server_addr):loop_(loop),server_addr_(server_addr),client_fd_(Socket::CreateNonBlockSocket()){};
        ~TcpClient(){};
        
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
