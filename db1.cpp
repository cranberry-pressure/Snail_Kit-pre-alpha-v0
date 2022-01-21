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

#include <fstream>
#include <istream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "sqlite3.h"
#include "json/json.h"
#include "db1.h"
#include "cluster.h"
#include <json/writer.h>
#include <json/reader.h>

using namespace std;
static int callback(void *noused, int argc, char **argv, char **azColName);
int z = 0;
Json::Value node;
Json::Value obj;
cluster cmA;

//Close db
void dataB::closeDb(){
    sqlite3_close(dbSnail);
    return;
}

//open db
bool dataB::openDb(){
    int result;
    result = sqlite3_open("./snail.db", &dbSnail);
    if(result){
        std::cout << "fail openDb"<< std::endl;
        sqlite3_close(dbSnail);
        return false; //message err
    }//success
    return true;
    
}

//Sql insert requests
bool dataB::insert(Json::Value obj){
    std::string sql, home, table;
    
    table = obj["table"].asString();
    if(table == "start"){
        snIpfs = obj["checkIpfs"].asString();
        snClus = obj["cluster"].asString();
        
        sql = "INSERT INTO startStat (checkIpfs, cluster) VALUES ( '"  + snIpfs + "','" + snClus + "');";
        prepare(sql);
        stepping();
        return true;
    }
    
    table = obj["table"].asString();
    if(table == "ClusterInfo"){
        
        clusterName = obj["ClusterName"].asString();
        followConfig = obj["Config"].asString();
        nodeRole = obj["NodeRole"].asString();
        
        sql = "INSERT INTO ClusterInfo (ClusterName, NodeRole, Config) VALUES ( '"  + clusterName + "','" + nodeRole + "','" + followConfig +  "');";
        prepare(sql);
        
        stepping();
        
        return true;
    }  
    
    if(table == "ClusterList"){
        
        clusterName = obj["NamePath"].asString();
        time = obj["Date"].asInt();
        
        sql = "INSERT INTO ClusterList(Name, Date) VALUES('" + clusterName+ "',' ?);";
        prepare(sql);
        bind_int(time, 1);
        stepping();
        
        return true;
    }
    
    if(table == "port"){
        port = obj["port"].asUInt();
        sql = "INSERT INTO portUsed(port) VALUES(?);";
        prepare(sql);
        bind_int(id, 1);
        stepping();
        return true;
        
    }
        
    return false;
} 


bool dataB::prepare(std::string sql){
    openDb();
    const char* soo = sql.c_str();
    rc = sqlite3_prepare_v2(dbSnail, soo, -1, &sta, pzTail);
    
    if(rc != SQLITE_OK){
            std::cout<<"Err "<< sqlite3_errmsg(dbSnail)<<endl;      
    }
    if(rc != SQLITE_OK){
        std::cout<<"fail prepare dB"<< std::endl;
        return false;
    }
    cout << "There are: "<< soo <<"and "<< sqlite3_bind_parameter_count(sta)<<endl;
    return true;
}

void dataB::bind_int(int val, int place){
    ut = sqlite3_bind_int(sta, place, val);
    
    if(ut != SQLITE_OK){
        cout<<"no bind int 1st"<<endl;
        
    }
    return;
}

void dataB::stepping(){
    if(sqlite3_step(sta) != SQLITE_DONE){
        sqlite3_finalize(sta);
        return;
    }
    sqlite3_reset(sta);
    sqlite3_clear_bindings(sta);
    sqlite3_close(dbSnail);
    return;
    
}

//Sql select requests
std::string dataB::select(const char* sql, bool wr, int val){
    
    const char* zTail = nullptr;
    char* ctail;
    const char** pzTail = &zTail;
    sqlite3_stmt* sta = nullptr;
    const char* data = "Callback called";
    
    openDb();
    rc = sqlite3_exec(dbSnail, sql, callback, (void*)data, &ctail);
    if(rc != SQLITE_OK){
        std::cout << "Failed clusInfo" << std::endl;
        node["res"] = "Failed ClusInfo";
        //cm.sendRes(node);
        sqlite3_free(pzTail);
    }
    closeDb();
    sqlite3_close(dbSnail);
    cmA.sendRes(node);
   
}

//Setup db at installation 
bool dataB::setupDb(){
    
    char* err = 0;
    const char *sqls;
    int rc;
    
    openDb();
    
     sqls = "CREATE TABLE startStat(checkIpfs TEXT NOT NULL, cluster TEXT NOT NULL);";
    rc = sqlite3_exec(dbSnail, sqls, NULL, 0, NULL);

    if(rc != SQLITE_OK){
        std::cout<<"Err"<< sqlite3_errmsg(dbSnail) << std::endl; 
        return false;
    }
    
    sqls = "CREATE TABLE ClusterList(Name TEXT NOT NULL, Date INTEGER NOT NULL);";
    rc = sqlite3_exec(dbSnail, sqls, NULL, 0, NULL);

    if(rc != SQLITE_OK){
        std::cout<<"Err"<< sqlite3_errmsg(dbSnail) << std::endl; 
        return false;
    }

    sqls = "CREATE TABLE ClusterInfo(ClusterName TEXT NOT NULL, NodeRole TEXT NOT NULL, Config TEXT NOT NULL);";
    rc = sqlite3_exec(dbSnail, sqls, NULL, 0, NULL);

    if(rc != SQLITE_OK){
       std::cout<<"Err"<< sqlite3_errmsg(dbSnail)<<endl; 
       return false;
    }
    
    sqls = "CREATE TABLE PortUsed(value INTEGER NOT NULL);";
    rc = sqlite3_exec(dbSnail, sqls, NULL, 0, NULL);

    if(rc != SQLITE_OK){
       std::cout<<"Err"<< sqlite3_errmsg(dbSnail)<<endl; 
       return false;
    }
    
    
    sqlite3_close(dbSnail);
    return true;
    
 
}

static int callback(void *noused, int argc, char **argv, char **col){
    
    for(int i = 0; i < argc; i++){
        std::cout <<  col[i] << argv[i] << std::endl;
        if(col[i] == col[0]){
            z = z + 1;
        }
        
        obj[col[i]] = argv[i];
        node[to_string(z)] = obj;
        
    }
    cmA.sendRes(node);
    
    return 0;
}
