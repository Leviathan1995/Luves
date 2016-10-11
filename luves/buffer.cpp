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
        buffer_.insert(buffer_.end(), buffer.buffer_[buffer.GetReadIndex()], buffer.buffer_[0]);
        write_index_ += buffer.GetReadIndex();
        INFO_LOG("Append %d bytes to buffer.", buffer.GetReadIndex() );
    }

    void Buffer::Append(const std::string & msg)
    {
        for (auto s:msg)
        {
            buffer_[read_index_] = s;
            write_index_++;
        }
        INFO_LOG("Append %d bytes to buffer.", msg.length());

    }

    int Buffer::ReadImp(int fd) // in
    {
        buffer_.clear();
        read_index_ = 0;
        int n = int(read(fd, &buffer_[0], 1024));
        INFO_LOG("Receive %d bytes.",n);
        if(n>0)
            read_index_ += n;
      
        return n;
    }

    int Buffer::WriteImp(int fd) // out
    {
        int n = int(write(fd, buffer_.data(), write_index_));
        INFO_LOG("Write %d bytes.", n);
        buffer_.clear();
        write_index_ = 0;
        return n;
    }

    std::ostream & operator <<(std::ostream & os, Buffer & buffer)
    {
        for (auto data:buffer.buffer_)
            os<<data;
        return os;
    }
}
