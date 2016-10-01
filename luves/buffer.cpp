//
//  Buffer.cpp
//  Luves
//
//  Created by leviathan on 16/6/1.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#include "buffer.h"

namespace luves {

    //
    //Buffer
    //
    void Buffer::Append(Buffer & buffer)
    {
        buffer_.insert(buffer_.end(), buffer.buffer_.begin(),buffer.buffer_.end());
        writeindex_+=buffer_.capacity();
    }

    void Buffer::Append(const std::string & msg)
    {
        for (auto s:msg)
            buffer_.push_back(s);
        writeindex_+=msg.length();
    }

    int Buffer::ReadImp(int fd)
    {
        buffer_.resize(1024);
        int n=int(read(fd,&buffer_[0],1024));
        return n;
    }

    int Buffer::WriteImp(int fd)
    {
        int n=int(write(fd,buffer_.data(),Capcity()));
        return n;
    }

    std::ostream & operator <<(std::ostream & os, Buffer & buffer)
    {
        for (auto data:buffer.buffer_)
        {
            os<<data;
        }
        return os;
    }
}
