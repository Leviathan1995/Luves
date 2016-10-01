//
//  channel.cpp
//  Luves
//
//  Created by hashdata on 16/9/30.
//  Copyright © 2016年 hashdata. All rights reserved.
//

#include "channel.h"

//
//事件通道
//
namespace luves {
    
Channel::~Channel()
{
    Close();
}

//关闭通道
void Channel::Close()
{
    if (fd_>0)
    {
        loop_->GetIOModel()->DeleteChannel(this);
        
        close(fd_);
    }
}

void Channel::HandleEvent()
{
    if (active_events_ & EVFILT_READ)
    {
        if (readcb_)
        {
            readcb_();
        }
    }
    if (active_events_ & EVFILT_WRITE)
    {
        if (writecb_)
        {
            writecb_();
        }
    }
}

bool Channel::ReadEnable()
{
    return event_ & readevent;
}

bool Channel::WriteEnable()
{
    return event_ & writeevent;
}
}
