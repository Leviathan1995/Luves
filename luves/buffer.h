//
//  Buffer.hpp
//  Luves
//
//  Created by leviathan on 16/6/1.
//  Copyright © 2016年 leviathan. All rights reserved.
//

#ifndef BUFFER_H_
#define BUFFER_H_

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "logger.h"

namespace luves {

    //
    //buffer
    //
    class Buffer
    {

    public:
        Buffer():read_index_(0),write_index_(0){buffer_.resize(1024);};
        ~Buffer(){};
        
        void Swap(Buffer & rhs)
        {
            buffer_.swap(rhs.buffer_);
            std::swap(read_index_, rhs.read_index_);
            std::swap(write_index_,rhs.write_index_);
        }

        size_t Capacity(){return buffer_.capacity();}

        size_t Size(){return buffer_.size();}
        
        void Append(Buffer & buffer);
        void Append(const std::string & msg);
        
        int  ReadImp(int fd);
        int  WriteImp(int fd);

        void Clear(){buffer_.clear();}
        
        friend std::ostream & operator <<(std::ostream & os,Buffer &buffer);

        std::vector<char> & GetData(){return buffer_;}

        size_t GetWriteIndex(){return write_index_;}
        size_t GetReadIndex() {return read_index_;}
    private:
        size_t read_index_;
        size_t write_index_;
        std::vector<char> buffer_;
    };

}
#endif /* Buffer_h */
