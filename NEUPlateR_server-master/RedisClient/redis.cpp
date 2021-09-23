#include "redis.h"
#include <iostream>

Redis::Redis(){
    const char *ip="127.0.0.1";
    int port=6379;
    this->context = redisConnect(ip, port);
    if(this->context==NULL || context->err){
        if(context){
            std::cout<<"Error: "<<context->errstr<<std::endl;
            exit(-1);
        }else{
            std::cout <<"can not allocate redis context\n";
        }

    }
}

/**
* @functionName  HGET key field (HGET person first_name)
* @Description   brief
* @author        majing
* @date          2018-07-31
* @parameter     void
* @return        the corresponding value (Ming)
*/

std::string Redis::hget(const char*key, const char* hkey){
    const char* argv[3];
    size_t argvlen[3];
    argv[0]="HGET";
    argvlen[0]=4;
    argv[1]=key;
    argvlen[1]=strlen(key);
    argv[2]=hkey;
    argvlen[2]=strlen(hkey);

    redisReply* reply = (redisReply*) redisCommandArgv(this->context,3,argv,argvlen);
    std::string value;
    if(reply->type != REDIS_REPLY_NIL){
        value=std::string(reply->str, reply->str+reply->len);
    }
    freeReplyObject(reply);
    return value;
}

/**
* @functionName  HSET key field val (HSET person first_name Ming)
* @Description   brief
* @author        majing
* @date          2018-07-31
* @parameter     void
* @return        void
*/

int Redis::hset(const char*key, const char* hkey, const char*value){
    redisReply* reply =(redisReply*) redisCommand(this->context, "HSET %s %s %s",key,hkey, value);
    freeReplyObject(reply);
    return 1;
}

/**
* @functionName  SADD key val
* @Description   brief
* @author        majing
* @date          2018-07-31
* @parameter     void
* @return        void
*/

int Redis::sadd(const char*key, const char* value){
    redisReply* reply =(redisReply*) redisCommand(this->context, "SADD %s %s",key, value);
    freeReplyObject(reply);
    return 1;
}

/**
* @functionName  SMEMBERS key
* @Description   brief
* @author        majing
* @date          2018-07-31
* @parameter     void
* @return        members in the SET
*/
void Redis::smembers(std::string& key, std::vector<std::string>& values){
    const char *key_str = key.c_str();
    redisReply* reply =(redisReply*) redisCommand(this->context, "SMEMBERS %s",key_str);

    if(reply->type != REDIS_REPLY_NIL){
        int size = reply->elements;
        for(int i=0; i<size; ++i){
            char *val = reply->element[i]->str;
            values.push_back(std::string(val));
        }
    }
    freeReplyObject(reply);
}

int Redis::existsKey(const char* ID){
    redisReply * reply =
            (redisReply*) redisCommand(this->context,"exists %s",ID);

    int res = 0;
    if(reply->type == REDIS_REPLY_INTEGER){
        if(reply->integer == 1L){
            res = 1;
        }
    }
    freeReplyObject(reply);
    return res;
}

Redis::~Redis(){
    redisFree(this->context);
}
