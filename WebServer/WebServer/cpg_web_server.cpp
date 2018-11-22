//
//  cpg_web_server.cpp
//  reids_demo
//
//  Created by HuanaoGroup on 2018/3/28.
//  Copyright © 2018年 HuanaoGroup. All rights reserved.
//

#include "cpg_web_server.hpp"
#include "json.hpp"
#include <map>
#include <regex>

#define WEB_PORT 18080

std::map<std::string, std::string>
getQueryParams(const std::string& params);


void cpg_web_server::start()
{
    
    // 增加比赛
    // 校验数据： 存储到mysql
    // 发送mid至cs
    CROW_ROUTE(app, "/match/add")
    .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req)
    {
        auto params = getQueryParams(req.body);
        std::cout << params["name"] << std::endl;
        
        nlohmann::json j(params);
        
        crow::response res(j.dump());
        res.set_header("content-type", "application/json");
        return res;
    });
    
    CROW_ROUTE(app, "/match/add")
    .name("name")
    ([](const crow::request& req){
        std::cout << req.url_params << std::endl;
        return req.body;
    });
    
    CROW_ROUTE(app,"/add/<int>/<int>")
    ([](const crow::request& /*req*/, crow::response& res, int a, int b){
        std::ostringstream os;
        os << a+b;
        res.write(os.str());
        res.end();
    });
    
    app.port(WEB_PORT).run();
}

void cpg_web_server::route()
{
    
}

#include <boost/regex.hpp>

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






