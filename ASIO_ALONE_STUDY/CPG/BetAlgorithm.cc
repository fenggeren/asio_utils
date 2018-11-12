#include "BetAlgorithm.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <mutex>

#define HOUR_SECONDS	3600

namespace BetCoreAlgo {

std::recursive_mutex  syncmutex_;

unsigned gen_session_key()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
    return rand();
}

unsigned long long                   curGameId_ = 10000;
unsigned long long GetNextGameId()
{
    std::lock_guard<std::recursive_mutex> locker(syncmutex_);
    unsigned long long gameID = curGameId_;
    FILE *fp = fopen("gameid", "r");
    if(fp)
    {
        char szTemp[256] = {0};
        fread(szTemp, 256, 1, fp);
        fclose(fp);

        unsigned long long gid = atol(szTemp);
        gameID = gid+1;
        sprintf(szTemp, "%llu", gameID);
         fp = fopen("gameid", "wb+");
        fwrite(szTemp, strlen(szTemp), 1, fp);
        fclose(fp);
    }
    else
    {
        gameID++;
    }
    curGameId_ = gameID;
    return gameID;
}

unsigned long long client_session_id_ = 1000000;
unsigned long long GetNextSessionId()
{
    client_session_id_++;
    return client_session_id_;
}

int bsearch_similar_number(const std::vector<int>& arr, int val)
{
    const int max_index = arr.size() - 1;
    int left = 0, right = max_index;
    int mid = 0;
    while (left <= right) {
        mid = (left + right) >> 1;
        if (mid == max_index)
            break;
        if (arr[mid] <= val && arr[mid + 1] > val)
            return mid;
        else if (arr[mid] < val)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return mid;
}

int get_similar_number(const std::vector<unsigned>& arr, unsigned val)
{
    int index = 0;
    const int max_index = arr.size() - 1;
	if (val <= arr[0])
		return 0;
    for (; index < max_index; ++index)
        if (arr[index] <= val && arr[index + 1] > val)
            break;
    return index;
}

void convert_http(const std::string& http_addr, std::string& host,
	std::string &service)
{
	std::string::size_type pos = http_addr.find("http://");
	if (pos != http_addr.npos)
		pos = 7;
	else
		pos = 0;
	std::string addr = http_addr.substr(pos);
	//if (inet_addr(addr.c_str()) != INADDR_NONE) { // ip
    if (addr[0] >= '0' && addr[0] <= '9') {
		std::string::size_type pos = addr.find(":");
		if (pos != addr.npos) {
			host = addr.substr(0, pos);
			service = addr.substr(pos + 1);
		} else {
			// error
			host = addr;
			service = "8080";
		}
	} else { // domain
		host = addr;
		service = "http";
	}
}

size_t get_packet_header_size(const char* packet)
{
    for (size_t i = 0; i < 32; i += 8) {
        if (0 == (*packet & 0x80))
            return (i + 8) >> 3;
        packet++;
    }
    return 0;
}

size_t convert_packet(const char* packet, size_t size, char* out_packet)
{
	size_t len = *packet & 0x7F;
	if (0 == (*packet & 0x80)) {
		memcpy(out_packet, packet + 1, size - 1);
	}
	else {
		int res = *(packet + 1) & 0xFF;
		len |= res << 7;
        memcpy(out_packet, packet + 2, size - 2);
	}

	return len;
}

std::string format_current_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	// ignore micseconds
    tm tm_time ={0,0,0,0,0,0,0,0,0,0,0};
	time_t seconds = tv.tv_sec;
    localtime_r(&seconds, &tm_time);
	char buf[16] = {0};
	snprintf(buf, sizeof(buf), "%4d%02d%02d%02d%02d%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1,
        tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min,
		tm_time.tm_sec);
	return buf;
}
int GetCurrentDayInt()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	// ignore micseconds
    struct tm tm_time ={0,0,0,0,0,0,0,0,0,0,0};
	time_t seconds = tv.tv_sec;
    	localtime_r(&seconds, &tm_time);

	int day = (tm_time.tm_year + 1900) * 10000 + (tm_time.tm_mon + 1) * 100 + (tm_time.tm_mday);
	return day;
}

std::string format_expiredate_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	// ignore micseconds
    struct tm tm_time ={0,0,0,0,0,0,0,0,0,0,0};
	// add 12 hours
	time_t seconds = tv.tv_sec + 43200;
    localtime_r(&seconds, &tm_time);
	char buf[16] = {0};
	snprintf(buf, sizeof(buf), "%4d%02d%02d%02d%02d%02d",
		tm_time.tm_year + 1900, tm_time.tm_mon + 1,
        tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min,
		tm_time.tm_sec);
	return buf;
}

void generate_sequence(int type, char sequence[33])
{
	/**
	 * total 32.
	 * type: 4
	 * ServerId: 4
	 * TableId: 4
	 * expire date: 14
	 * Serial Number: 6
	 */
	static int serial_num = 0;
	serial_num++;
	if (serial_num > 999999)
		serial_num = 0;

	int table_id = 0;

	sprintf(sequence, "%04d%04d%04d%s%06d",
		type, 
        1,
		table_id,
		format_expiredate_time().c_str(),
		serial_num);
}

u_int64_t rand64()
{
	return rand() ^ ((u_int64_t)rand() << 15) ^ 
		((u_int64_t)rand() << 30) ^ ((u_int64_t)rand() << 45) ^ 
		((u_int64_t)rand() << 60);
}

unsigned long long rand_between(unsigned long long from, unsigned long long to)
{
	return rand64() % (to - from + 1) + from;
}

std::string getStringFromFile(const std::string& file_name)
{
	std::string ret = "";
	do {
    FILE* fp = fopen(file_name.c_str(), "r");
		if (NULL == fp)
			break;

		long file_size;
		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

    char* data = (char*)malloc(file_size + 1);
		data[file_size] = '\0';

		file_size = fread(data, sizeof(char), file_size, fp);

		ret = std::string(data);

		fclose(fp);
		free(data);
	} while (0);

	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

long long convert_time(const char* str_date)
{
   struct tm mttbegintm ={0,0,0,0,0,0,0,0,0,0,0};
    strptime(str_date, "%Y-%m-%d", &mttbegintm); //将字符串转换为tm时间
    mttbegintm.tm_isdst = -1;
    unsigned long long date_time = mktime(&mttbegintm); //将tm时间转换为秒时间
	return date_time;
}

long long convert_date_to_int(const char* str_date)
{
    struct tm mttbegintm = {0,0,0,0,0,0,0,0,0,0,0};
    strptime(str_date, "%Y/%m/%d", &mttbegintm); //将字符串转换为tm时间
    mttbegintm.tm_isdst = -1;
    unsigned long long date_time = mktime(&mttbegintm); //将tm时间转换为秒时间
    return date_time;
}

long long convert_time_to_int(const char* str_date)
{
    unsigned int hour = 0;
    unsigned int min = 0;
    unsigned int sec = 0;
        sscanf(str_date , "%d:%d:%d" ,  &hour , &min , &sec );
        long long time_int = ((hour) * 60 + min) * 60 + sec;
	return time_int;
}

long long GetCurDateInt()
{
	long long cur_date_int = 0;
	long cur_time = time(NULL);
    struct tm *p = localtime(&cur_time);
    p->tm_sec = 0;
    p->tm_min = 0;
    p->tm_hour = 0;
    cur_date_int = mktime(p);
	return cur_date_int;
}

long long GetCurTimeInt()
{
    long long curTimeint = 0;
    long cur_time = time(NULL);
    struct tm *p = localtime(&cur_time);
    curTimeint = (p->tm_hour * 3600) + (p->tm_min * 60) + p->tm_sec;
    return curTimeint;
}

long long GetCurMonDayFormat()
{
	long long cur_date_int = 0;
	long cur_time = time(NULL);

	tm *p_tm = gmtime(&cur_time);
	if (p_tm)
	{
		cur_date_int = (p_tm->tm_mon + 1)* 100 + p_tm->tm_mday;
	}
	return cur_date_int;
}
long long GetCurDateFormat()
{
	long long cur_date_format = 0;
	long cur_time = time(NULL);
    //long next_time = cur_time + 24 * 60 * 60;
    tm *p_tm = localtime(&cur_time);
	if (p_tm)
	{
		cur_date_format = (p_tm->tm_year + 1900) * 10000 + (p_tm->tm_mon + 1) * 100 + p_tm->tm_mday;
	}

	return cur_date_format;
}

long long GetDateIntFromFormat(long long date_format)
{
    struct tm mttbegintm ={0,0,0,0,0,0,0,0,0,0,0};
    time_t date_int;
    char szBuf[256] = {0};
    sprintf(szBuf, "%lld", date_format);
    strptime(szBuf, "%Y%m%d", &mttbegintm); //将字符串转换为tm时间
    mttbegintm.tm_isdst = -1;
    date_int = mktime(&mttbegintm); //将tm时间转换为秒时间
	return date_int;
}

long long GetDateTimeIntFromFormat(const char *szDateTime)
{
    struct tm mttbegintm ={0,0,0,0,0,0,0,0,0,0,0};
    time_t date_int;
    strptime(szDateTime, "%Y-%m-%d %H:%M:%S", &mttbegintm); //将字符串转换为tm时间
    mttbegintm.tm_isdst = -1;
    date_int = mktime(&mttbegintm); //将tm时间转换为秒时间
    return date_int;
}

std::string getCurrentSystemDate()
{
 auto tt = time(0);
  struct tm* ptm = localtime(&tt);
  char date[60] = {0};
  sprintf(date, "%d-%02d-%02d",
    (int)ptm->tm_year + 1900,(int)ptm->tm_mon + 1,(int)ptm->tm_mday);
  return std::string(date);
}

long long createRandom(long long llbegin , long long llend)
{
    timeval tv;
    gettimeofday(&tv , NULL);
    srand(tv.tv_sec + tv.tv_usec);
     srand((int)time(NULL));
     long int result = llbegin + rand()%(llend-llbegin+1);
     return result;
}

}
