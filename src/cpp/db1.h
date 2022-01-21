/*
 The MIT License (MIT)
 
 Copyright 2021, 2022 Jimmy Débé

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the "Software"), to deal in the Software without restriction, including without 
limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef DB1_H
#define DB1_H
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include "sqlite3.h"
#include "json/json.h"

class dataB{
public:
    bool openDb();
    void closeDb();
    bool insert(Json::Value obj);
    std::string select(const char* sql, bool wr, int num_c);
    bool setupDb();
    bool prepare(std::string sql);
    void bind_text(std::string val, int place);
    void bind_int(int val, int place);
    void stepping();
    sqlite3* dbSnail;
private:
    int rc, ut;
    const char* zTail = nullptr;
    const char** pzTail = &zTail;
    sqlite3_stmt* sta = nullptr;
    int  ticketId, port, interval, timeR, timeS, time, id, valA;
    std::string consensus, sql, pro1, pro2, clusterId, clusterName, followConfig, peerAdr, nodeRole, snIpfs, snClus;
     
};


#endif /* DB1_H */

