//
//  Logging.hpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/21.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#pragma once

#include <array>
#include <string>
#include <mutex>
#include <vector>
#include <thread>

#include "Timestamp.hpp"

namespace fasio
{


namespace logging
{
    const static int kSmallBuffer = 4000;
    const static int kLargeBuffer = 4000 * 1000;
    
    
    template<int SIZE>
    class SwapBuffer
    {
    public:
        
        SwapBuffer()
        : writeIndex_(0)
        {
        }
        
        void append(const char* buf, size_t len)
        {
            if (writable() > len) {
                std::copy(buf, buf + len, beginWrite());
                writeIndex_ += len;
            }
        }
        
        void reset()
        {
            writeIndex_ = 0;
        }
        
        const char* data() const { return data_.data(); }
        size_t readable() const { return writeIndex_; }
        size_t writable() const { return data_.size() - writeIndex_; }
        
        void add(size_t len) { writeIndex_ += len; }
        
        char* beginWrite() { return data_.data() + writeIndex_;}
        char* beginRead() { return data_.data(); }
        
        const std::string toString() const { return std::string(data_.data(), readable()); }
        
    private:
        
        std::array<char, SIZE> data_;
        size_t writeIndex_{0};
    };
    
    class LogStream
    {
        using self = LogStream;
    public:
        
        using Buffer = SwapBuffer<kSmallBuffer>;
        
        self& operator<<(bool v)
        {
            buffer_.append(v ? "1" : "0", 1);
            return *this;
        }
        self& operator<<(short);
        self& operator<<(unsigned short);
        self& operator<<(int);
        self& operator<<(unsigned int);
        self& operator<<(long);
        self& operator<<(unsigned long);
        self& operator<<(long long);
        self& operator<<(unsigned long long);
        
        self& operator<<(const void*);
        self& operator<<(float v)
        {
            *this << static_cast<double>(v);
            return *this;
        }
        self& operator<<(double);
        
        self& operator<<(char v)
        {
            buffer_.append(&v, 1);
            return *this;
        }
        
        self& operator<<(const char* str)
        {
            if (str)
            {
                buffer_.append(str, strlen(str));
            }
            else
            {
                buffer_.append("(null)", 6);
            }
            return *this;
        }
        
        self& operator<<(const unsigned char* str)
        {
            return operator<<(reinterpret_cast<const char*>(str));
        }
        
        self& operator<<(const std::string& v)
        {
            buffer_.append(v.data(), v.size());
            return *this;
        }
        
        self& operator<<(const Buffer& v)
        {
            *this << v.toString();
            return *this;
        }
        
        void append(const char* data, int len)
        {
            buffer_.append(data, len);
        }
        void append(const std::string& data)
        {
            buffer_.append(data.data(), data.length());
        }
        
        const Buffer& buffer() const { return buffer_; }
        void resetBuffer() { buffer_.reset(); }
        
    private:
        
        template <typename T>
        void formatInteger(T);
        
    private:
        Buffer buffer_;
        
        static const int kMaxNumericSize = 32;
    };
    
    class Fmt
    {
    public:
        template <typename T>
        Fmt(const char* fmt, T val);
        
        const char* data() const { return buf_; }
        int length() const { return length_;}
        
    private:
        char buf_[32];
        int length_;
    };
    
    inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
    {
        s.append(fmt.data(), fmt.length());
        return s;
    }
    
    
    class Logger
    {
    public:
        
        enum LogLevel
        {
            TRACE,
            DEBUg,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };
        
        // __FILE__  获取纯粹的 文件名
        class SourceFile
        {
        public:
            
            template <int N>
            inline SourceFile(const char(&arr)[N])
            : data_(arr)
            , size_(N-1)
            {
                const char* slash = strrchr(data_, '/');
                if (slash)
                {
                    data_ = slash + 1;
                    size_ -= static_cast<int>(data_ - arr);
                }
            }
            explicit SourceFile(const char* filename)
            : data_(filename)
            {
                const char* slash = strrchr(filename, '/');
                if (slash)
                {
                    data_ = slash + 1;
                }
                size_ = static_cast<int>(strlen(data_));
            }
             
            const char* data_;
            int size_;
        };
        
        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, LogLevel level, const char* func);
        Logger(SourceFile file, int line, bool toAbort);
        ~Logger();
        
        LogStream& stream() { return impl_.stream_; }
        
        static LogLevel logLevel();
        static void setLogLevel(LogLevel level);
        
        using OutputFunc = void(*)(const char* msg, size_t len);
        using FlushFunc = void(*)();
        
        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);
        //.
        
    private:
        
        class Impl
        {
        public:
            using LogLevel = Logger::LogLevel;
            Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
            void formatTime();
            void finish();
            
            LogStream stream_;
            LogLevel level_;
            int line_;
            SourceFile basename_;
            Timestamp time_;
        };
        
    private:
        Impl impl_;
    };
    
    extern Logger::LogLevel g_logLevel;
    inline Logger::LogLevel Logger::logLevel()
    {
        return g_logLevel;
    }
 
    
#define LOG_TRACE if(Logger::LogLevel() <= Logger::TRACE) \
    Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if(Logger::LogLevel() <= Logger::DEBUg) \
    Logger(__FILE__, __LINE__, Logger::DEBUg, __func__).stream()
#define LOG_MINFO if(Logger::LogLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__, Logger::INFO, __func__).stream()
#define LOG_WARN  Logger(__FILE__, __LINE__, Logger::WARN, __func__).stream()
#define LOG_ERROR  Logger(__FILE__, __LINE__, Logger::ERROR, __func__).stream()
#define LOG_FETAL  Logger(__FILE__, __LINE__, Logger::FATAL, __func__).stream()
#define LOG_SYSTEM  Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL  Logger(__FILE__, __LINE__,true).stream()

    int strerror_tl(int savedError);
    
#define CHECK_NOTNULL(val) \
    CheckNotNull(__FILE__, __LINE__, "" #val " 'Must be non NULL", (val))
    
    template <typename T>
    T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr)
    {
        if (ptr == NULL)
        {
            Logger(file, line, Logger::FATAL).stream() << names;
        }
        return ptr;
    }
    
    
    class AppendFile
    {
    public:
        
        explicit AppendFile(const std::string& filename);
        ~AppendFile();
        
        void append(const char* logline, const size_t len);
        void flush();
        
        off_t writtenBytes() const { return writtenBytes_; }
        
    private:
        
        size_t write(const char* logline, size_t len);
        
    private:
        FILE* fp_;
        char buffer_[64*1024];
        off_t writtenBytes_;
    };
    
    
    class LogFile
    {
    public:
        
        LogFile(const std::string& basename,
                off_t rollSize,
                bool threadSafe = true,
                int flushInterval = 3,
                int checkEveryN = 1024);
        ~LogFile();
        
        void append(const char* logline, int len);
        void flush();
        bool rollFile();
        
    private:
        
        void appendUnlocked(const char* logline, int len);
        static std::string getLogFileName(const std::string& basename,
                                          time_t* now);
        
        
    private:
        const std::string basename_;
        const off_t rollSize_;
        const int flushInterval_;
        const int checkEveryN_;
        
        int count_;
        
        std::shared_ptr<std::mutex> mutex_;
        time_t startOfPeriod_;
        time_t lastRoll_;
        time_t lastFlush_;
        
        std::shared_ptr<AppendFile> file_;
        
        const static int kRollPerSeconds_ = 60 * 60 * 24;
    };
    
    
    class AsyncLogging
    {
    public:
        
        AsyncLogging(const std::string& basename,
                     off_t rollSize,
                     int flushInterval = 3);
        
        void append(const char* logline, int len);
        
        void start();
        
        ~AsyncLogging()
        {
            if (running_)
            {
                stop();
            }
        }
        
        void stop()
        {
            running_ = true;
            condition_.notify_one();
            thread_->join();
        }
        
    private:
        
        AsyncLogging(const AsyncLogging&);
        void operator=(const AsyncLogging&);
        
        void threadFunc();
        
    private:
        using Buffer = SwapBuffer<kLargeBuffer>;
        using BufferPtr = std::shared_ptr<Buffer>;
        using BufferVector = std::vector<BufferPtr>;
        
        const int flushInterval_;
        bool running_;
        std::string basename_;
        off_t rollSize_;
        
        std::mutex mutex_;
        std::condition_variable condition_;
        BufferPtr currentBuffer_;
        BufferPtr nextBuffer_;
        BufferVector buffers_;
        
        std::shared_ptr<std::thread> thread_;
    };
}













}
