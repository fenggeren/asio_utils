#include "cpg_match_create_factory.h"
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <limits.h>
#include "cpg_json_struct_helper.h"
#include <Net/logging/Logging.hpp>
#include <CPG/BetAlgorithm.h>

using namespace fasio::logging;
 
#define ONE_DAY_SECOND 3600*24


namespace
{
  int string_date_to_int(const char* str_date);
  void delete_all_mark(std::string& str, const std::string& mark);
  int date_to_weekday(time_t date);

  struct create_date_pair
{
    create_date_pair(const std::string& begin_date_str,
                     const std::string& end_date_str = "")
    {
        begin_date = string_date_to_int(begin_date_str.c_str());
        if (end_date_str == "")
        {
            end_date = begin_date + ONE_DAY_SECOND;
        }
        else
        {
            end_date = string_date_to_int(end_date_str.c_str());
        }
    }

    create_date_pair(int begin, int end)
            : begin_date(begin), end_date(end)
    {}

    std::list<int> get_dates()
    {
        std::list<int> dates;
        for (int beg = begin_date; beg < end_date; beg += ONE_DAY_SECOND)
        {
            dates.push_back(beg);
        }
        return dates;
    }

    friend std::ostream& operator<<(std::ostream& os, create_date_pair& pair)
    {
        os << "begin date: " << pair.begin_date << "   "
           << "end date: " << pair.end_date;
        return os;
    }

    int begin_date;
    int end_date;
};

class create_date_handler
{
public:
   
  class create_constraint
  {
    public:
    virtual ~create_constraint() {};
    virtual bool operator()(size_t date) { return true;}
  };
  // 日期区域约束[20180311-20180315]
  // 星期约束 [1,2,4,6,7]
  class create_constraint_weekly : public create_constraint
  {
  public:
      create_constraint_weekly(std::list<int>& weekdays):
              weekdays_(weekdays)
      {}

      virtual bool operator()(size_t date) override
      {
          int weekday = date_to_weekday(date);
          LOG_MINFO << "weekday:  " << weekday  << "  date: " << date;
          for (auto& wd : weekdays_)
          {
              if (weekday == wd)
              {
                  return true;
              }
          }
          return false;
      }
  private:
      std::list<int> weekdays_;
  };

  class create_constraint_deadline : public create_constraint
  {
      
  };

public:
 

    std::list<int>
    operator()(const std::string &dates_str,
                size_t cycle_life,
                const std::string &times_str,
                std::list<create_constraint*> constraints = {})
    {
        std::list<int> dates = create_date(dates_str, cycle_life);
        std::list<int > times = create_time(times_str);

        std::list<int> date_times;

        for (auto &date : dates)
        {
            for (auto &time : times)
            {
                int t = date + time;
                for (auto& constraint: constraints)
                {
                    if (!(*constraint)(t))
                    {
                        t = 0; break;
                    }
                }

                if (t != 0) date_times.push_back(t);
            }
        }

        return date_times;
    }

    std::list<int>
            create_date(const std::string& dates_str,
                        size_t cycle_life)
    {
        std::string dates(dates_str);
        delete_all_mark(dates, " ");

        boost::char_separator<char> comma(",");
        boost::char_separator<char> dash("-");

        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
        tokenizer durations(dates, comma);

        std::list<create_date_pair> pairs;
        for (auto& duration : durations)
        {
            tokenizer dates(duration, dash);
            auto begin = dates.begin();
            // 是时间间隔
            if (dates.end() != ++begin)
            {
                pairs.push_back({*dates.begin(), *begin});
            }
            else  // 单独的日期
            {
                pairs.push_back({*dates.begin()});
            }
        }

        for (auto& pair : pairs)
        {
           LOG_MINFO << "begin date: " << pair.begin_date << "   "
           << "end date: " << pair.end_date;
        }

        // 赛选 目前时间和最早时间, 找出最大的值，并且
        int dateInt = BetCoreAlgo::GetCurDateFormat();
        int cur_date = BetCoreAlgo::GetDateIntFromFormat(dateInt);//12343423423

        int latest_date = INT_MAX;
        for (auto& pair : pairs)
        {
            if (pair.begin_date < latest_date)
            {
                latest_date = pair.begin_date;
            }
        }

        latest_date = std::max(latest_date, cur_date);
        int show_end_date =  latest_date + cycle_life * ONE_DAY_SECOND;
        std::list<create_date_pair> filter_pairs;
        for (auto& pair : pairs)
        {
            if (pair.begin_date > show_end_date) continue;
            if (pair.end_date < latest_date) continue;

            if (pair.begin_date >= latest_date && pair.end_date <= show_end_date)
            {
                filter_pairs.push_back(pair);
            }
            else
            {
                if (pair.begin_date >= latest_date)
                {
                    filter_pairs.push_back({pair.begin_date, show_end_date});
                }
                else if(pair.end_date <= show_end_date)
                {
                    filter_pairs.push_back({latest_date, pair.end_date});
                }
                else /*if(pair.end_date >= show_end_date && pair.begin_date <= latest_date) */
                {
                    filter_pairs.push_back({latest_date, show_end_date});
                }
            }
        }

        LOG_MINFO << "===========================================";
        LOG_MINFO << "date_duration:  [" << latest_date << "," << show_end_date;
        LOG_MINFO << "===========================================";
        for (auto& pair : filter_pairs)
        {
            LOG_MINFO << "begin date: " << pair.begin_date << "   "
           << "end date: " << pair.end_date;
        }

        LOG_MINFO << "===========================================";
        std::list<int> int_dates;
        for (auto& pair : filter_pairs)
        {
            auto pds = pair.get_dates();
            for (auto& d : pds)
            {
                int_dates.push_back(d);
                LOG_MINFO << "size: " << pds.size() << "     " << d;
            }
        }

        return int_dates;
    }

    std::list<int>
            create_time(const std::string& times_str)
    {
        std::string times(times_str);
        delete_all_mark(times, " ");

        boost::char_separator<char> comma(",");
        typedef boost::tokenizer<boost::char_separator<char> > CustonTokenizer;
        CustonTokenizer tokens(times,comma);

        std::list<int> time_list;
        for(auto& time : tokens)
        {
            int begin_time = BetCoreAlgo::convert_time_to_int(time.c_str());
            time_list.push_back(begin_time);
        }

        return time_list;
    }

    std::list<int>
            create_weekday(const std::string& weekdays_str)
    {
        std::string weekdays(weekdays_str);
        delete_all_mark(weekdays, " ");
        boost::char_separator<char> comma(",");
        typedef boost::tokenizer<boost::char_separator<char> > CustonTokenizer;
        CustonTokenizer tokens(weekdays, comma);
        std::list<int> week_days;
        for (auto& weekday : tokens)
        {
            week_days.push_back(std::atol(weekday.c_str()));
        }
        return week_days;
    }
};

}



////////////////////////////////////////////////////////////////////////////////////////
///////////    cpg_match_create_factory   /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

// redis加载所有
std::list<std::shared_ptr<cpg_match_info>> 
cpg_match_create_factory::load_from_reids()
{
//   std::list<cpg_match_generate_date> patterns = {};
//   std::list<cpg_match_rule> rules = {};
  
  std::list<std::shared_ptr<cpg_match_info>> cached_matches;

//  std::list<cpg_match_info> infos = g_data_store->get_all_match_info();
//  for (auto& info : infos)
//  {
//      auto match_info = std::make_shared<cpg_match_info>(std::move(info));
//      matches_.insert({match_info->match_id, match_info});
//      cached_matches.push_back(match_info);
//  }
  return cached_matches;
}

// file 加载所有, 
// 加载的数据，会覆盖掉之前读取的数据
// file，load的 rule及patterns不存入redis
std::list<std::shared_ptr<cpg_match_info>>
cpg_match_create_factory::load_from_file()
{
  using nlohmann::json;
   
  std::ifstream in("match.config.json");

  nlohmann::json file_json;
  in >> file_json;

  if (file_json.is_null())
  {
      LOG_MINFO << "match.config.json file is invalid!!!!!";
      return {};
  }  

  patterns_.clear();
    
    return load_from_json(file_json);
}

std::list<std::shared_ptr<cpg_match_info>>
cpg_match_create_factory::load_from_json(const nlohmann::json& file_json)
{
    using nlohmann::json;
    std::list<std::shared_ptr<cpg_match_info>> added_matches;
    // 根据ruleid创建比赛的  创建模式
    std::vector<json> patterns_json = file_json["patterns"];
    for(auto& pattern : patterns_json)
    {
        patterns_.emplace_back(pattern);
    }
    
    // 规则id
    std::vector<json> rules_json = file_json["rules"];
    for(auto& rule : rules_json)
    {
        cpg_match_rule r = rule;
        rules_.emplace(r.rule_id, std::move(r));
    }
    
    // 直接创建比赛
    std::vector<json> matches_json = file_json["matches"];
    for(auto& match : matches_json)
    {
        int begin_date = (int)BetCoreAlgo::GetDateTimeIntFromFormat(
                                                                    match["begin_date"].get<std::string>().c_str());
        auto match_info = create_match(match, begin_date);
        if (match_info)
        {
            added_matches.push_back(match_info);
        }
    }
    
    return added_matches;
}


std::shared_ptr<cpg_match_info> 
cpg_match_create_factory::create_match(const nlohmann::json& match, int begin_date)
{   
    cpg_match_rule rule = match; 
    auto match_info = std::make_shared<cpg_match_info>();
    match_info->begin_date_time = begin_date;
    match_info->match_id = get_next_match_id();
    match_info->rule_id = match_info->match_id;
    struct_utils::match_rule_to_info(rule, *match_info.get());
    rules_.emplace(rule.rule_id, std::move(rule));
    if (check_match_valid(match_info))
    {
        match_info->match_state = ENUM_STATE_READY;
        matches_.insert({match_info->match_id, match_info});
        return match_info;
    }
    return nullptr;
}

std::list<std::shared_ptr<cpg_match_info>>
cpg_match_create_factory::create_all_matches()
{
    std::list<std::shared_ptr<cpg_match_info>> new_matches;
    for(auto& pattern : patterns_)
    {
        if (rules_.find(pattern.rule_id) == rules_.end())
        {
            continue;
        }
        auto tmp_new = create_match(pattern, rules_[pattern.rule_id]);
        new_matches.merge(std::move(tmp_new));
    }

    return new_matches;
}

std::list<std::shared_ptr<cpg_match_info>>
cpg_match_create_factory::create_match(const cpg_match_generate_date& date_pattern, 
const cpg_match_rule& rule)
{   
    std::list<std::shared_ptr<cpg_match_info>> matches;
    // 获取所有可能创建比赛的时间
    auto date_times = get_all_begin_date_time(date_pattern);
    // auto rule = rules_[date_pattern.rule_id];

    // 创建比赛
    for(auto& date_time : date_times)
    {
        auto match_info = std::make_shared<cpg_match_info>();
        match_info->begin_date_time = date_time;
        match_info->match_id = get_next_match_id();
        match_info->match_state = ENUM_STATE_READY;
        struct_utils::match_rule_to_info(rule, *match_info.get());
        match_info->rule_id = match_info->match_id;
        if(check_match_valid(match_info))
        {   
            matches_.insert({match_info->match_id, match_info});
            matches.push_back(match_info);
        }
    }

    return matches;
}
 

bool cpg_match_create_factory::check_match_valid(std::shared_ptr<cpg_match_info> info)
{ 
    // 开赛时间已过
    if (info->begin_date_time < time(NULL))
    {
        return false;
    }
    auto iter = 
    std::find_if(matches_.begin(), matches_.end(),
                     [&](const std::map<int,  std::shared_ptr<cpg_match_info>>::value_type &pair)
                     {
                         if ((info->match_type == pair.second->match_type) && 
                            (info->begin_date_time == pair.second->begin_date_time) && 
                            (info->match_name == pair.second->match_name) && 
                            (info->reward_type == pair.second->reward_type))
                            {
                                return true;
                            } else { return false; }
                     });
    return iter == matches_.end();
}

std::list<int> 
cpg_match_create_factory::get_all_begin_date_time(const cpg_match_generate_date& generate_date)
{ 
std::list<int> date_times;

int cur_time = BetCoreAlgo::GetCurDateFormat();
int cur_time_int = BetCoreAlgo::GetDateIntFromFormat(cur_time);
{
    // 指定日期创建，即客户端可见
    int show_date = string_date_to_int(generate_date.generate_date.c_str());
    // 未到 创建日期
    if (cur_time_int < show_date) return date_times;
}

create_date_handler create_date;

if (generate_date.type == CreateDate)
{ 
    date_times =
            create_date(generate_date.date_duration, 
                        generate_date.generate_time_cycle, 
                        generate_date.times);
}
else if (generate_date.type == CreateDaily)
{
    date_times = create_date.create_time(generate_date.times);
    for(auto& t : date_times)
    {
        t += cur_time_int;
    } 
}
else if (generate_date.type == CreateWeekly)
{
    auto week_days = create_date.create_weekday(generate_date.weeks);
    create_date_handler::create_constraint_weekly constraint(week_days);
    date_times =
            create_date(generate_date.date_duration, 
                        generate_date.generate_time_cycle, 
                        generate_date.times, 
                        {&constraint});         
}

return date_times;
}

static int matchID = 10000;
int cpg_match_create_factory::get_next_match_id()
{
//    return g_data_store->get_next_match_id();
    return matchID++;
}
static int ruleID = 10000;
int cpg_match_create_factory::get_next_rule_id()
{
    return ruleID++;
//    return g_data_store->get_next_rule_id();
}


//////////////////////////////////////////////////////////////////////
namespace
{


int string_date_to_int(const char* str_date)
{
    struct tm mttbegintm = {0,0,0,0,0,0,0,0,0,0,0};
    strptime(str_date, "%Y%m%d", &mttbegintm); //将字符串转换为tm时间
    mttbegintm.tm_isdst = -1;
    int date_time = mktime(&mttbegintm); //将tm时间转换为秒时间
    return date_time;
}

void delete_all_mark(std::string& str, const std::string& mark)
{
    unsigned int nSize = mark.size();
    while(1)
    {
        auto pos = str.find(mark);
        if(pos == std::string::npos)
        {
            return;
        }

        str.erase(pos, nSize);
    }
}

int date_to_weekday(time_t date)
{
  struct tm* ptr = localtime(&date);
  return ptr->tm_wday == 0 ? 7 : ptr->tm_wday;
}

}
