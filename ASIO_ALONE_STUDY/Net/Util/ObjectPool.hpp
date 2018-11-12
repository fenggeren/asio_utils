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

template <class T, class R>
class ObjectPool
{
public:
    
    ObjectPool(size_t blockSize = 1024):blockSize_(blockSize){}
    
    template<typename ...Type>
    T*
    createObject(Type&& ...values)
    {
        return new T(std::forward<Type>(values)...);
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
                newBlock = (T*)malloc(blockSize_ * sizeof(T));
            }
            catch (std::bad_alloc)
            {
                return NULL;
            }
            blocks_.push_back(newBlock);
            for(int i = 0; i < blockSize_; i++)
            {
                objects_.push_back(newBlock + i);
            }
        }
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    
private:
    std::list<T*> blocks_;
    std::list<T*> objects_;
    size_t blockSize_;
};

template <class T>
class ObjectPool<T, std::shared_ptr<T>>
{
public:
    ObjectPool(size_t blockSize = 1024)
    :blockSize_(blockSize)
    {
    }
    
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
                newBlock = (T*)malloc(blockSize_ * sizeof(T));
            }
            catch (std::bad_alloc)
            {
                return NULL;
            }
            blocks_.push_back(newBlock);
            for(int i = 0; i < blockSize_; i++)
            {
                objects_.push_back(newBlock + i);
            }
        }
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    
private:
    std::list<T*> blocks_;
    std::list<T*> objects_;
    size_t blockSize_;
};



#include <mutex>
template <class T, class R>
class ThreadSafeObjectPool
{
public:
    
    ThreadSafeObjectPool(size_t blockSize = 1024)
     :blockSize_(blockSize)
     {
     }
    
    template<typename ...Type>
    T*
    createObject(Type&& ...values)
    {
        T* mem = NULL;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            mem = getFreeObjectMem();
            if (!mem)
            {
                return nullptr;
            }
        }
        
        return new (mem)T(std::forward<Type>(values)...);
    }
    
    ~ThreadSafeObjectPool()
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
            std::lock_guard<std::mutex> lock(mutex_);
            objects_.push_back(obj);
            printf("%lu -- %lu \n", blocks_.size(), objects_.size());
        }
    }
private:
    T* getFreeObjectMem()
    {
//        printf("%lu -- %lu \n", blocks_.size(), objects_.size());
        enableEnoughObjects();
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    
    void enableEnoughObjects()
    {
        if (objects_.size() == 0)
        {
            T* newBlock = NULL;
            try
            {
                newBlock = (T*)malloc(blockSize_ * sizeof(T));
            }
            catch (std::bad_alloc)
            {
                throw ;
            }
            blocks_.push_back(newBlock);
            for(int i = 0; i < blockSize_; i++)
            {
                objects_.push_back(newBlock + i);
            }
        }
    }
    
private:
    std::list<T*> blocks_;
    std::list<T*> objects_;
    std::mutex mutex_;
    size_t blockSize_;
};


template <class T>
class ThreadSafeObjectPool<T, std::shared_ptr<T>>
{
public:
    
    ThreadSafeObjectPool(size_t blockSize = 1024)
    :blockSize_(blockSize)
    {
    }
    
    template<typename ...Type>
    std::shared_ptr<T>
    createObject(Type&& ...values)
    {
        T* mem = NULL;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            mem = getFreeObjectMem();
            if (!mem)
            {
                return nullptr;
            }
        }
        
        std::shared_ptr<T> obj(new (mem)T(std::forward<Type>(values)...), std::bind(&ThreadSafeObjectPool::releaseObject, this, std::placeholders::_1));
        return std::move(obj);
    }
    
    ~ThreadSafeObjectPool()
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
            std::lock_guard<std::mutex> lock(mutex_);
            objects_.push_back(obj);
        }
    }
private:
    T* getFreeObjectMem()
    {
        enableEnoughObjects();
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    
    void enableEnoughObjects()
    {
        if (objects_.size() == 0)
        {
            T* newBlock = NULL;
            try
            {
                newBlock = (T*)malloc(blockSize_ * sizeof(T));
            }
            catch (std::bad_alloc)
            {
                throw ;
            }
            blocks_.push_back(newBlock);
            for(int i = 0; i < blockSize_; i++)
            {
                objects_.push_back(newBlock + i);
            }
        }
    }
    
private:
    std::list<T*> blocks_;
    std::list<T*> objects_;
    std::mutex mutex_;
    size_t blockSize_;
};















