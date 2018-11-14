//
//  ObjectIndexPool.hpp
//  ASIO_ALONE_STUDY
//
//  Created by huanao on 2018/11/7.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <list>
#include <vector>
#include <array>

namespace fasio
{
    


#define POOL_INVALID_INDEX 0xFFFFFFFF
#define OB_MAX 10000

template <typename T, size_t MAX_NUM = OB_MAX>
class ObjectIndexPool
{
public:
    
    ObjectIndexPool()
    {
        maxIndex_ = 0;
        size_ = 0;
        invalidIndexs_.push_back(MAX_NUM);
        for(auto iter = objectArray_.begin(); iter != objectArray_.end(); iter++)
        {
            *iter = nullptr;
        }
    }
    
    
    unsigned int addObject(const T& object)
    {
        if (maxIndex_ <= MAX_NUM)
        {
            objectArray_[maxIndex_] = object;
            objects_.push_back(object);
            size_ ++;
            return maxIndex_++;
        }
        else if (invalidIndexs_.size() > 0)
        {
            unsigned int invalidIndex = invalidIndexs_.front();
            {
                if (invalidIndex != POOL_INVALID_INDEX)
                {
                    objectArray_[invalidIndex] = object;
                    objects_.push_back(object);
                    invalidIndexs_.pop_front();
                    size_++;
                    return invalidIndex;
                }
            }
        }
        return POOL_INVALID_INDEX;
    }
    
    void removeObject(unsigned int index)
    {
        if (index <= MAX_NUM)
        {
            size_--;
            objects_.remove(objectArray_[index]);
            objectArray_[index] = nullptr;
            invalidIndexs_.push_back(index);
        }
    }
    T& getObject(unsigned int index)
    {
        assert(index<=MAX_NUM);
        return objectArray_[index];
    }
    
    T& operator[](unsigned int index)
    {
        assert(index<=MAX_NUM);
        return objectArray_[index];
    }
    
    unsigned int getSize() const
    {
        return size_;
    }
    
    bool isFull()
    {
        return size_ == MAX_NUM + 1;
    }
    
    void foreach(const std::function<void(const T&)>& cb)
    {
        for (auto& obj : objects_)
        {
            cb(obj);
        }
    }
    
    
private:
    unsigned int maxIndex_;
    unsigned int size_;
//    std::vector<T> objectMap_;
    std::array<T, MAX_NUM + 1> objectArray_;
    std::list<T> objects_;
    std::list<unsigned int> invalidIndexs_;
};






}
