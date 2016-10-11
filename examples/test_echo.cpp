//
//  echo.cpp
//  Luves
//
//  Created by leviathan on 16/5/19.
//  Copyright © 2016年 leviathan. All rights reserved.
//


#include "luves/luves.h"

using namespace luves;

Buffer GetInput(const TcpConnectionPtr & conn)
{
    return conn->GetInputBuffer();
}

int main()
{
    EventLoop loop;
    Ip4Addr server_addr("0.0.0.0", 6543);
    TcpServer server(&loop, server_addr);
    server.SetReadCb(GetInput);

    /*
    server.SetWriteCb([](const TcpConnectionPtr & conn)
                     {conn->Send("HTTP/1.1 200 OK\r\n"
                                 "Content-Type:text/html;charset=utf-8\r\n"
                                 "Content-Length:18\r\n"
                                 "\r\n"
                                 "Welcome to tinyweb");});
    */
    server.SetWriteCb([](const TcpConnectionPtr & conn)
                      {conn->Send(conn->GetInputBuffer());});

    server.RunServer();
    loop.loop();
}

