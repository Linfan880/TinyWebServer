//
// Created by linfan on 2021/10/19.
//

#ifndef WEBSERVER_SQLCONNRAII_H
#define WEBSERVER_SQLCONNRAII_H
#include "sqlconnpool.h"

/* 资源在对象构造初始化 资源在对象析构时释放*/
class SqlConnRAII {
public:
    SqlConnRAII(MYSQL** sql, SqlConnPool *connpool){
        assert(connpool);
        *sql = connpool->GetConn();
        sql_ = *sql;
        connpool_ = connpool;
    }
    ~SqlConnRAII(){
        if(sql_) {connpool_->FreeConn(sql_);}
    }

private:
    MYSQL *sql_;
    SqlConnPool* connpool_;
};

#endif //WEBSERVER_SQLCONNRAII_H
