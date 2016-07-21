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

#include "tcp_connection.h"

//#include "threads.h"

namespace luves {
    
    //
    //Tcp服务器
    //
    class TcpServer
    {
    public:
        TcpServer(EventLoop *loop,Ip4Addr & addr):loop_(loop),addr_(addr),listen_fd_(Socket::CreateNonBlockSocket()),listenning_(false),host_(addr.GetHost()),port_(addr.GetPort())
            {
                listen_channel_=new Channel(loop,listen_fd_);
                SocketOp::SetReuseaddr(listen_fd_);
                SocketOp::SetNonblock(listen_fd_);

            };
        ~TcpServer(){};
        
        //Socket操作
        void Bind();
        void Listen();
        
        //新建Tcp连接服务,设置CallBack函数
        void NewConnection(int accept_fd,Ip4Addr local,Ip4Addr peer);
        //处理TCP连接请求
        void HandleAccept();
        //设置回调函数
        void SetReadCb(const TcpCallBack & cb){readcb_=cb;}
        void SetWriteCb(const TcpCallBack & cb){writecb_=cb;}
        void SetCloseCb(const TcpCallBack & cb){closecb_=cb;}
        
        //启动Server
        void  RunServer();
        
        struct sockaddr_in * GetServerAddrPointer(){return &server_addr_;}
        
        EventLoop * GetLoop(){ return loop_;}
        //设置HSHA模式
        void SetHsha(bool hsha){is_hsha_=hsha;loop_->SetHsha(is_hsha_);}
        
        std::map<int,TcpConnectionPtr> * GetTcpConnMap(){return &TcpConn_fd_map_;}
    private:
        
        std::map<int,TcpConnectionPtr> TcpConn_fd_map_;
        bool is_hsha_  ;                        //是否为hsha模式
        struct sockaddr_in server_addr_;
        std::string host_;
        short port_;
        int listen_fd_;                         //监听套接字
        bool listenning_;                       //监听状态
        Ip4Addr addr_;
        EventLoop * loop_;
        Channel * listen_channel_;
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
