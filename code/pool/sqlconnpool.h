//
// Created by linfan on 2021/10/19.
//

#ifndef WEBSERVER_SQLCONNPOOL_H
#define WEBSERVER_SQLCONNPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"

class SqlConnPool {
public:
    static SqlConnPool* Instance();

    MYSQL* GetConn();
    void FreeConn(MYSQL* conn);
    int GetFreeConnCount();

    void Init(const char* host, int port,
              const char* user, const char* pwd,
              const char* dbName, int connSize);
    void ClosePool();

private:
    SqlConnPool();  //构造
    ~SqlConnPool(); //析构

    int MAX_CONN_;  //最大连接数
    int useCount_;  //使用的连接数
    int freeCount_; //空闲的数量

    std::queue<MYSQL *>connQue_;
    std::mutex mtx_;
    sem_t semId_;

};


#endif //WEBSERVER_SQLCONNPOOL_H
