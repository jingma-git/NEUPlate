#ifndef REDIS_H
#define REDIS_H

/**
* @projectName   customized hiredis for easy usage
* @brief         brief
* @author        majing
* @date          2018-07-31
* @modify_author XXXX
* @modify_date   2018-07-31
*/
#include "hiredispp.h"


class Redis
{
public:
    redisContext* context;
    Redis();

    int hset(const char*key, const char* hkey, const char*value);
    std::string hget(const char*key, const char* hkey);

    int sadd(const char*key, const char* value);
    void smembers(std::string& key, std::vector<std::string>& values);

    int existsKey(const char* ID);

    virtual ~Redis();
};

#endif // REDIS_H
