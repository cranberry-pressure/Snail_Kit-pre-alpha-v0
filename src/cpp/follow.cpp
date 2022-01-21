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

#include <ios>
#include <fstream>
#include <istream>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include "json/json.h"
#include "snail.h"
#include "follow.h"
#include "cluster.h"

using namespace std;
snail snA;
cluster cmB;

//Start follower deamon on start up
bool follow::followerStart(char* name){
    
    char buff[400];
    
    const char* cmd = "cd C:\\Program Files && ipfs-cluster-follower ";
    const char* r = " run";
    strcpy(buff, cmd);
    strcat(buff, name);
    strcat(buff, r);
    
    rm = followerRun(buff);
    if(!rm){
        return false;
    }
    return true;
}

//initialization of new follower
bool follow::followerInit(std::string fName, std::string fPath){
    
    std::string args = fName + " init " + fPath;
    std::string cmd = "cd C:\\Program Files && ipfs-cluster-follower ";
    cmd = cmd + args;
    const char* arg = cmd.c_str();
    
    rm = followerRun(arg);
    if(!rm){
        return false;
    }
    
    return true;
    
}
// Run follower daemon
bool follow::followerRun(const char* path){
    
    const char* log = "./cLog.txt";
    Json::Value obj;
    
    rm = snA.callNode(path, log);
    if(!rm){
        std::cout<< "fail callNodefollow" << std::endl;
        obj["res"] = "failed callNode followr";
        //cm.sendRes(obj);
        
        return false;
    }else{
        std::cout<< "Success callNode cluster" << std::endl;
        obj["res"] = "success callNode followr";
        
    }
    cmB.sendRes(obj);
    return true;
    
}
