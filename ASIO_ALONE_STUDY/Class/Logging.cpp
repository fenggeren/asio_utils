//
//  Logging.cpp
//  ASIO_ALONE_STUDY
//
//  Created by fenggeren on 2018/7/21.
//  Copyright © 2018年 fenggeren. All rights reserved.
//

#include "Logging.hpp"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <assert.h>
#include <string>

namespace Logging
{
    const char digits[] = "9876543210123456789";
    const char* zero = digits + 9;
    const char digitsHex[] = "0123456789ABCDEF";
    
    // Efficient Integer to String Conversions, by Matthew Wilson.
    template<typename T>
    size_t convert(char* buf, T value)
    {
        T i = value;
        char* p = buf;
        
        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);
        
        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);
        
        return p - buf;
    }
    
    size_t convertHex(char buf[], uintptr_t value)
    {
        uintptr_t i = value;
        char* p = buf;
        
        do
        {
            int lsd = static_cast<int>(i % 16);
            i /= 16;
            *p++ = digitsHex[lsd];
        } while (i != 0);
        
        *p = '\0';
        std::reverse(buf, p);
        
        return p - buf;
    }
    
    
    template <typename T>
    void LogStream::formatInteger(T v)
    {
        if (buffer_.writable() >= kMaxNumericSize) {
            size_t len = convert(buffer_.beginWrite(), v);
            buffer_.add(len);
        }
    }
    
    LogStream& LogStream::operator<<(short v)
    {
        *this << static_cast<int>(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(unsigned short v)
    {
        *this << static_cast<unsigned int>(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(int v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(unsigned int v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(long v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(unsigned long v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(long long v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(unsigned long long v)
    {
        formatInteger(v);
        return *this;
    }
    
    LogStream& LogStream::operator<<(const void* p)
    {
        uintptr_t v = reinterpret_cast<uintptr_t>(p);
        if (buffer_.writable() >= kMaxNumericSize)
        {
            char* buf = buffer_.beginWrite();
            buf[0] = '0';
            buf[1] = 'x';
            size_t len = convertHex(buf+2, v);
            buffer_.add(len+2);
        }
        return *this;
    }
    
    LogStream& LogStream::operator<<(double v)
    {
        if (buffer_.writable() >= kMaxNumericSize)
        {
            int len = snprintf(buffer_.beginWrite(), kMaxNumericSize, "%.12g", v);
            buffer_.add(len);
        }
        return *this;
    }
    
    
    template<typename T>
    Fmt::Fmt(const char *fmt, T val)
    {
        length_ = snprintf(buf_, sizeof(buf_), fmt, val);
    }
    
    
 
    __thread char t_errnobuf[512];
    __thread char t_time[64];
    __thread time_t t_lastSecond;
    
    int strerror_tl(int savedError)
    {
       return strerror_r(savedError, t_errnobuf, sizeof(t_errnobuf));
    }
    
    
    Logger::LogLevel initLogLevel()
    {
        if (getenv("LOG_TRACE"))
        {
            return Logger::TRACE;
        }
        else if (getenv("LOG_DEBUG"))
        {
            return Logger::DEBUg;
        }
        else
        {
            return Logger::INFO;
        }
    }
    
    Logger::LogLevel g_logLevel = initLogLevel();
    
    const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
    {
        "TRACE ",
        "DEBUG ",
        "INFO  ",
        "WARN  ",
        "ERROR ",
        "FATAL "
    };
    
    class T
    {
    public:
        T(const char* str, unsigned len)
        : str_(str)
        , len_(len)
        {
            assert(strlen(str) == len_);
        }
        
        const char* str_;
        const unsigned len_;
    };
    
    inline LogStream& operator<<(LogStream& s, T v)
    {
        s.append(v.str_, v.len_);
        return s;
    }
    
    inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
    {
        s.append(v.data_, v.size_);
        return s;
    }
    
    void defaultOutput(const char* msg, size_t len)
    {
        size_t n = fwrite(msg, 1, len, stdout);
        (void)n;
    }
    
    void defaultFlush()
    {
        fflush(stdout);
    }
    
    Logger::OutputFunc g_output = defaultOutput;
    Logger::FlushFunc g_flush = defaultFlush;
    
    
  
    Logger::Impl::Impl(LogLevel level, int savedErrno,
                       const SourceFile& file, int line)
    : stream_()
    , level_(level)
    , line_(line)
    , basename_(file)
    , time_(Timestamp::now())
    {
        formatTime();
        stream_ << T(LogLevelName[level], 6);
        if (savedErrno != 0)
        {
            stream_ << strerror_tl(savedErrno) << " (errno= " << savedErrno << ") ";
        }
    }
    
    void Logger::Impl::formatTime()
    {
        int64_t microSecondsSinceEpoch = time_.microSecondsSinceEpoch();
        time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecondsPerSecond);
        int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecondsPerSecond);
        
        if (seconds != t_lastSecond)
        {
            t_lastSecond = seconds;
            struct tm tm_time;
            ::localtime_r(&seconds, &tm_time);
            
            int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                               tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                               tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
            assert(len == 17); (void)len;
        }
        
        Fmt us(".%06dZ ", microseconds);
        assert(us.length() == 9);
        stream_ << T(t_time, 17) << T(us.data(), 9);
    }
    
    void Logger::Impl::finish()
    {
        stream_ << " - " << basename_ << ':' << line_ << '\n';
    }
    
    
    Logger::Logger(SourceFile file, int line)
    : impl_(INFO, 0, file, line)
    {
    }
    
    Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line)
    {
        impl_.stream_ << func << ' ';
    }
    
    Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line)
    {
    }
    
    Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort?FATAL:ERROR, errno, file, line)
    {
    }
    
    Logger::~Logger()
    {
        impl_.finish();
        const LogStream::Buffer& buf(stream().buffer());
        g_output(buf.data(), buf.readable());
        if (impl_.level_ == FATAL)
        {
            g_flush();
            abort();
        }
    }
    
    void Logger::setLogLevel(Logger::LogLevel level)
    {
        g_logLevel = level;
    }
    
    void Logger::setOutput(OutputFunc out)
    {
        g_output = out;
    }
    
    void Logger::setFlush(FlushFunc flush)
    {
        g_flush = flush;
    }
    
    AppendFile::AppendFile(const std::string& filename)
    :fp_(::fopen(filename.data(), "ae"))
    , writtenBytes_(0)
    {
        assert(fp_);
        ::setbuffer(fp_, buffer_, sizeof(buffer_));
    }
    
    AppendFile::~AppendFile()
    {
        ::fclose(fp_);
    }
    
    void AppendFile::append(const char *logline, const size_t len)
    {
        size_t n = write(logline, len);
        size_t remain = len - n;
        while (remain > 0)
        {
            size_t x = write(logline + n, remain);
            if (x == 0)
            {
                int err = ferror(fp_);
                if (err)
                {
                    fprintf(stderr, "AppendFile: append() failed %d\n", strerror_tl(err));
                }
                break;
            }
            n += x;
            remain = len - n;
        }
        writtenBytes_ += len;
    }
    
    void AppendFile::flush()
    {
        ::fflush(fp_);
    }
    
    size_t AppendFile::write(const char *logline, size_t len)
    {
        return ::fwrite(logline, 1, len, fp_);
    }
    
    
    
    LogFile::LogFile(const std::string& basename,
                     off_t rollSize,
                     bool threadSafe,
                     int flushInterval,
                     int checkEveryN)
    : basename_(basename),
    rollSize_(rollSize),
    flushInterval_(flushInterval),
    checkEveryN_(checkEveryN),
    count_(0),
    mutex_(threadSafe ? new std::mutex : NULL),
    startOfPeriod_(0),
    lastRoll_(0),
    lastFlush_(0)
    {
        rollFile();
    }
    
    LogFile::~LogFile()
    {
        
    }
    
    void LogFile::append(const char *logline, int len)
    {
        if (mutex_)
        {
            std::lock_guard<std::mutex> lock(*mutex_);
            appendUnlocked(logline, len);
        }
        else
        {
            appendUnlocked(logline, len);
        }
    }
    
    void LogFile::flush()
    {
        if (mutex_)
        {
            std::lock_guard<std::mutex> lock(*mutex_);
            file_->flush();
        }
        else
        {
            file_->flush();
        }
    }
    
    void LogFile::appendUnlocked(const char *logline, int len)
    {
        file_->append(logline, len);
        
        if (file_->writtenBytes() > rollSize_)
        {
            rollFile();
        }
        else
        {
            ++count_;
            if (count_ >= checkEveryN_)
            {
                count_ = 0;
                time_t now = time(NULL);
                time_t thisPeriod = now / kRollPerSeconds_ * kRollPerSeconds_;
                if (thisPeriod != startOfPeriod_)
                {
                    rollFile();
                }
                else if (now - lastFlush_ > flushInterval_)
                {
                    lastFlush_ = now;
                    file_->flush();
                }
            }
        }
    }
    
    bool LogFile::rollFile()
    {
        time_t now = 0;
        std::string filename = getLogFileName(basename_, &now);
        time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
        
        if (now > lastRoll_)
        {
            lastRoll_ = now;
            lastFlush_ = now;
            startOfPeriod_ = start;
            file_.reset(new AppendFile(filename));
            return true;
        }
        return false;
    }
    
    std::string LogFile::getLogFileName(const std::string &basename, time_t *now)
    {
        std::string filename(basename);
        
        char timebuf[32];
        struct tm tm;
        *now = time(NULL);
        
        localtime_r(now, &tm);
        strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
        filename += timebuf;
        
        
        filename += ".log";
        
        return filename;
    }
    
    AsyncLogging::AsyncLogging(const std::string& basename,
                               off_t rollSize,
                               int flushInterval)
    : flushInterval_(flushInterval)
    , running_(false)
    , basename_(basename)
    , rollSize_(rollSize)
    , mutex_()
    , currentBuffer_(new Buffer)
    , nextBuffer_(new Buffer)
    , buffers_()
    {
        
    }
    
    void AsyncLogging::append(const char *logline, int len)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (currentBuffer_->writable() > len)
        {
            currentBuffer_->append(logline, len);
        }
        else
        {
            buffers_.push_back(currentBuffer_);
            if (nextBuffer_)
            {
                // TODO.
                currentBuffer_ = nullptr;
                nextBuffer_.swap(currentBuffer_);
            }
            else
            {
                currentBuffer_.reset(new Buffer);
            }
            currentBuffer_->append(logline, len);
            condition_.notify_one();
        }
    }
    
    void AsyncLogging::start()
    {
        running_ = true;
        thread_ = std::make_shared<std::thread>(std::bind(&AsyncLogging::threadFunc, this));
    }
    
    void AsyncLogging::threadFunc()
    {
        assert(running_ = true);
        LogFile output(basename_, rollSize_, false);
        BufferPtr newBuffer1(new Buffer);
        BufferPtr newBuffer2(new Buffer);
    
        BufferVector buffersToWrite;
        
        while (running_)
        {
            assert(newBuffer1 && newBuffer1->readable() == 0);
            assert(newBuffer2 && newBuffer2->readable() == 0);
            
            {
                std::unique_lock<std::mutex> lock(mutex_);
                if (buffers_.empty())
                {
                    
                    condition_.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::seconds(flushInterval_));
                }
                buffers_.push_back(currentBuffer_);
                buffersToWrite.swap(buffers_);
                currentBuffer_ = nullptr;
                newBuffer1.swap(currentBuffer_);
                if (!nextBuffer_)
                {
                    newBuffer2.swap(nextBuffer_);
                }
            }
            
            assert(!buffersToWrite.empty());
            
            if (buffersToWrite.size() > 25)
            {
                char buf[256];
                snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                         Timestamp::now().toFormattedString().c_str(),
                         buffersToWrite.size()-2);
                fputs(buf, stderr);
                output.append(buf, static_cast<int>(strlen(buf)));
                buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
            }
            
            for (size_t i = 0; i < buffersToWrite.size(); ++i)
            {
                output.append(buffersToWrite[i]->data(),
                              (int)buffersToWrite[i]->readable());
            }
            
            if (buffersToWrite.size() > 2)
            {
                buffersToWrite.resize(2);
            }
            
            if (!newBuffer1)
            {
                assert(!buffersToWrite.empty());
                newBuffer1 = buffersToWrite.back();
                buffersToWrite.pop_back();
                newBuffer1->reset();
            }
            if (!newBuffer2)
            {
                assert(!buffersToWrite.empty());
                newBuffer2 = buffersToWrite.back();
                newBuffer2->reset();
            }
            
            buffersToWrite.clear();
            output.flush();
        }
        
        output.flush();
    }
}













