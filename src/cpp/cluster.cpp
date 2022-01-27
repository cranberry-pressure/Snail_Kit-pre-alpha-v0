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

#include <iostream>
#include "stdio.h"
#include <string>
#include <ios>
#include <fstream>
#include "windef.h"
#include <string>
#include "sqlite3.h"
#include "db1.h"
#include "json/json.h"
#include "json/value.h"
#include "follow.h"
#include "snail.h"
#include "cluster.h"
#include "sqlite3.h"
#include "json/reader.h"
#include "json/writer.h"
using namespace std;

bool rs;
bool begh = false;
bool call = false;
bool del = false;
bool portC = false;
std::string port;
static int callback(void *noused, int argc, char **argv, char **col);
follow flA;

snail sn;
dataB dbA;
Json::Value objRes;

//Create initialization of a new cluster by operator
bool cluster::service(const char* clusPath, const char* clusPathB, bool initFlag, std::string consensus){
    bool rs;
    const char* cmd = "cd C:\\Program Files && ipfs-cluster-service ";
    const char* log = "./cLog.txt";
    const char* clusInit = "init";
    const char* co = "init consensus raft";
    
    if(initFlag){
        if(consensus == "raft"){
            strcpy(buff, cmd);
            strcat(buff, co);
            rs = sn.callNode(buff, log);
            if(!rs){
                std::cout <<"failed callNode cluster" << std::endl;
                return false;
            }
        }else{
            strcpy(buff, cmd);
            strcat(buff, clusInit);
            rs = sn.callNode(buff, log);
            if(!rs){
                std::cout << "failed callNode cluster"<< std::endl;
                return false;
            }
        }
        grabPorts();
        clusIt(clusPath, clusPathB);
        
    }
    
    sn.startClus(clusPathB);
    return true;
    
}


//Sql select request for cluster functions
bool cluster::clusS(std::string sql, bool callC, bool beginM){
    const char* zTail = nullptr;
    char* ctail;
    const char** pzTail = &zTail;
    sqlite3_stmt* sta = nullptr;
    const char* data = "Callback called";
    const char* sqlC = sql.c_str();
    call = callC;
    begh = beginM;
    
    dbA.openDb();
    rc = sqlite3_exec(dbA.dbSnail, sqlC, callback, (void*)data, &ctail);
    if(rc != SQLITE_OK){
        std::cout << "Failed clusS exec func" << std::endl;
        sqlite3_free(pzTail);
        return false;
    }
    dbA.closeDb();
    
    return true;
    
}


static int callback(void *noused, int argc, char **argv, char **col){
   
     if(begh){
         if(argv[0] == false){
             bool result = sn.startIPFS();
             if(!result){
                 std::cout << "failed startIPFS from clusterSelect"<<std::endl;
              return 0;
             }
         }
     }
     
    if(del){
        std::cout << "Test" << argv[0]<< std::endl;
        return 0;
    }
    if(portC){
        port = argv[0];
        return 0;
    }
    
    for(int i = 0; i < argc; i++){
        //std::cout <<  col[i] << argv[i] << std::endl;
        if(call){
            sn.startClus(argv[i]);
            //insert running cluster
        }else{
            flA.followerStart(argv[i]);
        }
    }
    
    return 0;
}

//change config file with correct port numbers
bool cluster::grabPorts(){
    int port;
    bool rt;
    std::string config = "C\\Program Files\\.ipfs-cluster\\service.json";
    std::string sql = "SELECT value FROM PortUsed WHERE rowid = 'MAX(rowid)'";
    std::ifstream ifs(config);
    if(ifs.fail()){
        std::cout << "failed ifstream grabports" << std::endl;
        ifs.close();
        return false;
    }
    std::string err;
    Json::CharReaderBuilder reading;
    Json::Value obj;
    
    
    if(Json::parseFromStream(reading, ifs, &obj, &err)){
        Json::Value& api = obj["api"];
        Json::Value& proxy = api["ipfsproxy"];
        Json::Value& restApi = api["restapi"];
        portC = true;
        rt = clusS(sql, false, false);
        if(rt != SQLITE_OK){
            std::cout << "Failed clusS" << std::endl;
            ifs.close();
            return false;
        }
        port = port + 1;
        std::string por = to_string(port);
        Json::StreamWriterBuilder build;
        const std::unique_ptr<Json::StreamWriter> writer(build.newStreamWriter());

       

        std::string newPort = "/ip4/127.0.0.1/tcp/";

        proxy["listen_multiaddress"] = newPort + por;
        rt = clusS(sql, false, false);
        if(rt != SQLITE_OK){
            std::cout << "Failed clusS" << std::endl;
            ifs.close();
            return false;
        }
        port = port + 1;
        por = to_string(port);

        restApi["http_listen_multiaddress"] = newPort + por;

        std::ofstream ofs(config);
        writer->write(obj, &ofs);
        portC = false;
        ofs.close();
        ifs.close();
        return true;
    }
    
    
}

//move config file and identity file in cluster direcotry locally
bool cluster::clusIt(const char* pathMain, const char* newPath){
    const char* config = "/service.json";
    const char* ident = "/identity.json";
   

    strcpy(buff, pathMain);
    strcpy(buffA, pathMain);
    strcat(buff, config);
    strcat(buffA, ident);
    
    std::ifstream ifs(buff, std::ios::in | std::ios::binary);
    if(ifs.fail()){
        std::cout << "failed ifstream clusItA" << std::endl;
        ifs.close();
        return false;
    }
    std::ofstream ofs(newPath, std::ios::out | std::ios::binary);
    ofs << ifs.rdbuf();
    ifs.close();
    
    
    std::ifstream iffs(buffA, std::ios::in | std::ios::binary);
    if(iffs.fail()){
        std::cout << "fail ifstream clusItB" << std::endl;
        iffs.close();
        return false;
    }
    ofs << iffs.rdbuf();
    iffs.close();
    ofs.close();
    return true;
   
} 
//Create a config file for cluster followers
bool cluster::fConfig(const char* path, const char* name){
    char buffA[200], buffB [300];
    const char* config = "/service.json";
    strcpy(buffA, path);
    strcat(buffA, config);
    const char* con = "Fsn.json";
    strcpy(buff, name);
    strcat(buff, con);
    strcpy(buffB, name);
    strcat(buffB, buff);
    std::ifstream ifs(buffB);
    ifs.close();
    
    std::ifstream ifss(buffA);
    if(ifss.fail()){
       std::cout << "failed ifstream mkeCon" << std::endl;
       ifss.close();
       return false;
    }
    
    std::string err;
    Json::CharReaderBuilder reading;
    Json::Value obj;
    
    
    if(Json::parseFromStream(reading, ifs, &obj, &err)){
        Json::StreamWriterBuilder build;
        const std::unique_ptr<Json::StreamWriter> writer(build.newStreamWriter());
        //test blanking object
        obj["api"] = " ";
        std::ofstream ofs(buffB);
        writer->write(obj, &ofs);
        ifs.close();
        ofs.close();

        return true;
    }
    
}

// Delete cluster from  db
bool cluster::delClus(std::string name){
    
    std::string dels = "DELETE FROM ClusterInfo WHERE ClusterName = '" + name + "'";
    del = true;
    rs = clusS(dels, false, false);
    if(rs){
        objRes["res"] = "Success delete cluster";
        
    }else{
        objRes["res"] = "failed delte cluster";
    }
    sendRes(objRes);
    return true;
}

//Add follow config cidAddress in db
bool cluster::updateC(Json::Value obj){
    std::string item = obj["addrs"].asString();
    std::string name = obj["ClusterName"].asString();
    std::string sql = "UPDATE ClusterInfo SET Config ='" +item+"' WHERE ClusterName = '"+name+"'; ";
    dbA.prepare(sql);
    dbA.stepping();
    Json::Value objA;
    objA["resA"] = "Success adding clusFollow config address";
    sendRes(objA);
    return true;
}
//Send request to open user interface
bool cluster::sendRes(Json::Value obj){
    
    std::string err;
    
    Json::StreamWriterBuilder build;
    const std::unique_ptr<Json::StreamWriter> writer(build.newStreamWriter());
    
    std::ofstream ofs("./response.json");
    writer->write(obj, &ofs);
    ofs.close();
    
    return true;
}



