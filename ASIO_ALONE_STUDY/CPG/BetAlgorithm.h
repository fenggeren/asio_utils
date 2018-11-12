/**
 * BetAlgorithm.h
 * The algorithm of bet.
 */
#ifndef CORE_BETALGORITHM_H__
#define CORE_BETALGORITHM_H__

#include <vector>
#include <string>

namespace BetCoreAlgo {

unsigned gen_session_key();
unsigned long long GetNextGameId();
unsigned long long GetNextSessionId();
void srand8(unsigned int i);
long long createRandom(long long llbegin , long long llend);
/**
 * find a similar number of val use binary search.
 * @return the similar number index.
 */
int bsearch_similar_number(const std::vector<int>& arr, int val);

/**
 * find a similar number of val use common search.
 * @return the similar number index.
 */
int get_similar_number(const std::vector<unsigned>& arr, unsigned val);

/**
 * convert ip address/domain to host and service.
 */
void convert_http(const std::string& http, std::string& host, std::string &service);


/**
  * calculate a number's total bytes.
  */
size_t get_packet_header_size(const char* packet);

/**
 * convert packetet to real packet.
 */
size_t convert_packet(const char* packet, size_t size, char* out_packet);

/**
 * format current time
 */
std::string format_current_time();
int GetCurrentDayInt();

/**
 * format expiredate.
 */
std::string format_expiredate_time();

/**
 * generate sequence.
 */
void generate_sequence(int type, char sequence[33]);

/**
 * generate a number from m to n.
 */
unsigned long long rand_between(unsigned long long from, unsigned long long to);

/**
 * get string from file
 */
std::string getStringFromFile(const std::string& file_name);

/**
 * convert string to second.
 */
long long convert_time(const char* str_date);
long long convert_date_to_int(const char* str_date);
long long convert_time_to_int(const char* str_date);
long long GetCurDateInt();
long long GetCurTimeInt();
long long GetCurMonDayFormat();
long long GetCurDateFormat();
long long GetNextDateFormat(long long cur_date_format);//20150918
long long GetDateIntFromFormat(long long date_format);
long long GetDateTimeIntFromFormat(const char *szDateTime);
std::string getCurrentSystemDate();
}

#endif // CORE_BETALGORITHM_H__
