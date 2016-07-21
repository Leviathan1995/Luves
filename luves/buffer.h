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

namespace luves {
    
    //
    //缓冲区
    //
    class Buffer
    {
        
    public:
        Buffer():readindex_(0),writeindex_(0){};
        ~Buffer(){};
        void Swap(Buffer & rhs)
        {
            buffer_.swap(rhs.buffer_);
            std::swap(readindex_, rhs.readindex_);
            std::swap(writeindex_,rhs.writeindex_);
        }
        
        size_t Capcity(){return buffer_.size();}
        
        void Append(Buffer & buffer);
        void Append(const std::string & msg);
        int  ReadImp(int fd);
        int  WriteImp(int fd);
        
        void Clear(){buffer_.clear();}
        friend std::ostream & operator <<(std::ostream & os,Buffer &buffer);
        
        std::vector<char> & GetData(){return buffer_;}
        
        
    private:
        size_t readindex_;
        size_t writeindex_;
        std::vector<char> buffer_;
    };
    
}
#endif /* Buffer_h */
