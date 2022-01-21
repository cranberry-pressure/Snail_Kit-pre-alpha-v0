#include "windows.h"
#include "unistd.h"
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <istream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include "json/json.h"
#include <json/value.h>
#include "db1.h"
#include "snail.h"
#include "cluster.h"

cluster cm;
snail sn;
dataB db;
int main() {
    
    //Start up check installs, open windows, start process manager, listen for request
    //??Does setup return flase if already created
    bool dat = db.setupDb();
    if(!dat){
        setUp();
        startIPFS();
        return 0;
    }
    
    bool rc;
    std::ifstream ifs("./request.json");
    Json::Value obj, objA;
    Json::Reader reading;
    reading.parse(ifs,obj);
    
    std::string act = obj["act"].asString;
    ifs.close();
    
    
    // if already running,skip
    if(act == "start"){
        //startIPFS();
        //
        std::string sql = "SELECT checkIpfs FROM startStat";
        rc = cm.clusS(sql, true, true);
        
        
        sql = "SELECT ClusterName FROM ClusterInfo WHERE nodeRole = 'operator'";
        rc = cm.clusS(sql, true, false);
        
        
        
        sql = "SELECT ClusterName FROM ClusterInfo WHERE nodeRole = 'peer'";
        
        rc = cm.clusS(sql, false, false);
        return 0;
    }
    //new cluster button/page
    if(act == "newCluster"){
        //take name?etc..
        rs = sn.createC(obj);
        if(rs){
            //send res
            std::cout << "success createC"<< std::endl;
        }else{
            std::cout << "failed createC" << std::endl;
        }
        
    }
    
    if(act == "cUpdate"){
        rs = cm.updateC(obj);
        if(rs){
            std::cout << "success cUpdate"<<std::endl;
        }
    }
    
    
    //join button
    if(act == "follow"){
        sn.follower(obj);
        
    }
    
    //unjoin button
    if(act == "unfollow"){
        std::string del = obj["ClusterName"].asString();
        cm.delClus(del);
                
    }
    
    //status page
    if(act == "status"){
        rs = info.freeSpace();
        if(rs){
            std::cout <<"success mem/storage check" <<std::endl;
        }else{
            std::cout << "failed mem check" <<std::endl;
        }
        /*
        rs = freeSpace();
        if(rs){
            std::cout <<"success space check"<< std::endl;
        }else{
            std::cout <<"failed space check" <<std::endl;
        }
        */
    }
    //new cluster button/ not needed??????
    if(act == "addClus"){
        
        rs = db.insert(obj);
        
        //rs = addClus(obj);
        if(!rs){
            //sendRes
            std::cout << "success addClus"<< std::endl;
        }else{
            std::cout << "failed addClus" << std::endl;
        }
    }
    
    //cluster manage for operators
    if(act == "clusPort"){
        std::string name = obj["ClusterName"].asString();
        cm.clusPort(name);
    }
    
    //cluster manage for followers*****tester?????
    if(act == "foList"){
        std::string name = obj["ClusterName"].asString();
        fl.foList(name);
    }
    
    //joined clusters ***************test??
    if(act == "ClusInfo"){
       
        sql = "SELECT * FROM ClusterInfo";
        db.select(sql);
         
    }
    
    //shut shitdown
    if(act == "shutdown"){
         sn.shutdownNode();
         //return 0;
    }
    
    
    return 0;
}




/*
    // check running 
    //bool started = dbStarts();
    //if(started != 1){
        //openWindows();
        //dbStarts(1,1);
    
        //startNodes();
        //openDB();
    startIPFS();
    startClus();
    followerStart();
        //startManager();
        //openWindows();
        //dbStarts(1,1);
    
    //startProcessManager();
    listeningSnail();
    */


