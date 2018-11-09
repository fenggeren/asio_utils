//
//  ObjectPool.hpp
//  C++Demo
//
//  Created by huanao on 2018/11/9.
//  Copyright © 2018年 fgr. All rights reserved.
//

#pragma once

#include <list>
#include <memory>
#include <type_traits>
namespace fasio
{
template <class T, size_t BLOCK_SIZE = 1024>
class ObjectPool
{
public:
    
    template<typename ...Type>
    std::shared_ptr<T>
    createObject(Type&& ...values)
    {
        std::shared_ptr<T> obj(new (getFreeObject())T(std::forward<Type>(values)...), std::bind(&ObjectPool::releaseObject, this, std::placeholders::_1));
        return std::move(obj);
    }
    
    ~ObjectPool()
    {
        while (!blocks_.empty())
        {
            T* objs = &(*blocks_.front());
            blocks_.pop_front();
            if (objs)
            {
                free(objs);
            }
        }
    }
    
    void releaseObject(T* obj)
    {
        if (obj != NULL)
        {
            objects_.push_back(obj);
        }
    }
    
    T* getFreeObject()
    {
        if (objects_.size() == 0)
        {
            T* newBlock = NULL;
            try
            {
                newBlock = (T*)malloc(BLOCK_SIZE * sizeof(T));
            }
            catch (std::bad_alloc)
            {
                return NULL;
            }
            blocks_.push_back(newBlock);
            for(int i = 0; i < BLOCK_SIZE; i++)
            {
                objects_.push_back(newBlock + i);
            }
        }
        printf("objects size begin: %lu\n", objects_.size());
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    
private:
    std::list<T*> blocks_;
    std::list<T*> objects_;
};


}














