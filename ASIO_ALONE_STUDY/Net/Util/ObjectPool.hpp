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


template <typename T, typename R, typename ...Type>
T* createObject(T* place, Type&& ...values)
{
    return new (place)T(std::forward<Type>(values)...);
}
template <typename T, typename R, typename ...Type>
std::shared_ptr<T> createObject(T* place, Type&& ...values, void(deletor)(T*))
{
    std::shared_ptr<T> obj(new (place)T(std::forward<Type>(values)...),
                           deletor);
    return std::move(obj);
}


template <class T, class R>
class ObjectPool
{
public:
    
    ObjectPool(size_t blockSize = 1024):blockSize_(blockSize){}
    
    template<typename ...Type>
    T*
    createObject(Type&& ...values)
    {
        return new (getFreeObject())T(std::forward<Type>(values)...);
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
        std::shared_ptr<T> obj(new (getFreeObject())T(std::forward<Type>(values)...),
                               std::bind(&ObjectPool::releaseObj, this, std::placeholders::_1));
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
    
    void releaseObject(const std::shared_ptr<T>& obj)
    {
        
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
    
    void releaseObj(T* obj)
    {
        if (obj != NULL)
        {
            objects_.push_back(obj);
        }
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
        
        std::shared_ptr<T> obj(new (mem)T(std::forward<Type>(values)...),
                               std::bind(&ThreadSafeObjectPool::releaseObj, this, std::placeholders::_1));
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
    
    
    void releaseObject(const std::shared_ptr<T>& obj)
    {
        
    }

private:
    T* getFreeObjectMem()
    {
        enableEnoughObjects();
        T* obj = &(*objects_.front());
        objects_.pop_front();
        return obj;
    }
    void releaseObj(T* obj)
    {
        if (obj != NULL)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            objects_.push_back(obj);
        }
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















