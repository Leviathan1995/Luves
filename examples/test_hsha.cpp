//
//  test_hsha.cpp
//  Luves
//
//  Created by leviathan on 16/6/28.
//  Copyright © 2016年 leviathan. All rights reserved.
//

/*
#include "luves.h"

using namespace luves;

Buffer GetInput(const TcpConnectionPtr & conn)
{
    return conn->GetInputBuffer();
}

int main()
{
    EventLoop loop;
    Ip4Addr server_addr("0.0.0.0", 6543);

    HshaServer server(&loop, server_addr, 2);
    server.SetReadCb(GetInput);


    server.SetWriteCb([](const TcpConnectionPtr & conn)
                    {conn->Send(conn->GetInputBuffer());});

    server.RunServer();
    loop.loop();
}
*/
