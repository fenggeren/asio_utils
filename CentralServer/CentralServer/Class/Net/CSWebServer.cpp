//
//  CSWebServer.cpp
//  CentralServer
//
//  Created by huanao on 2018/11/26.
//  Copyright © 2018年 fgr. All rights reserved.
//

#include "CSWebServer.hpp"
#include <CPG/Third/json/json.hpp>
#include <map>
#include <regex>
#include <CPG/MatchHelper/CPGMatchCreateFactory.hpp>
#include <CPG/MatchHelper/CPGMatchStructHelper.hpp>
#include <CPG/ErrorCodeDefine.h>
#include "../MatchManager/CSMatchManager.hpp"
#include "PromiseFutureQueue.hpp"

#define WEB_PORT 18080

using namespace nlohmann;

std::map<std::string, std::string>
getQueryParams(const std::string& params);


void CSWebServer::start()
{
    route();
    
    app.port(WEB_PORT).run();
}


void CSWebServer::route()
{
    CROW_ROUTE(app, "/match/create")
    .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req)
     {
         auto params = getQueryParams(req.body);
         
         nlohmann::json j(params["data"]);
         // 根据json， 创建比赛.
         auto& promise = PromiseFutureQueue::instance().nextPromise();
         
         auto future = promise.promise.get_future();
         CPGMatchProfile profile = j;
         int code = CSMatchManager::instance().createMatch(profile,
                                                        promise.promiseID);
         // 创建失败，直接返回
         if (code != kNoneError)
         {
             nlohmann::json resjson;
             resjson["code"] = code;
             crow::response resfailure(resjson.dump());
             resfailure.set_header("content-type", "application/json");
             return resfailure;
         }
         
         // 阻塞本线程， 直到创建成功返回.
         int errcode = future.get();
         nlohmann::json resjson;
         resjson["code"] = code;
         if (errcode == kNoneError)
         {
             const CPGMatchProfile& cmatch =  boost::any_cast<CPGMatchProfile>
             (PromiseFutureQueue::instance().futureData(promise.promiseID));
             resjson["data"] = {
                 {"mid", cmatch.mid}
             };
         }
         
         crow::response res(resjson.dump());
         res.set_header("content-type", "application/json");
         
         return res;
     });
}













std::map<std::string, std::string>
getQueryParams(const std::string& params)
{
    std::map<std::string, std::string> queryParams;
    static const std::regex queryParamRegex(
                                            "(^|&)" /*start of query or start of parameter "&"*/
                                            "([^=&]*)=?" /*parameter name and "=" if value is expected*/
                                            "([^=&]*)" /*parameter value*/
                                            "(?=(&|$))" /*forward reference, next should be end of query or
                                                         start of next parameter*/);
    std::cregex_iterator paramBeginItr(
                                       params.data(), params.data() + params.size(), queryParamRegex);
    std::cregex_iterator paramEndItr;
    for (auto itr = paramBeginItr; itr != paramEndItr; itr++) {
        if (itr->length(2) == 0) {
            // key is empty, ignore it
            continue;
        }
        queryParams.emplace(
                            std::string((*itr)[2].first, (*itr)[2].second), // parameter name
                            std::string((*itr)[3].first, (*itr)[3].second) // parameter value
                            );
    }
    
    return queryParams;
}
