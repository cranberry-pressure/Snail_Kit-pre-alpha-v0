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

#include "windows.h"
#include <ios>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <string>
#include <cstring>
#include "info.h"
#include "json/json.h"
#include "./cluster.h"

cluster cmC;
using namespace std;

//Check local memory
int info::checkMemory(){
    
    Json::Value obj;
    int res, resA;
    DWORDLONG memUsed;
    DWORDLONG totalPyhs;
    MEMORYSTATUSEX meminfo;
    meminfo.dwLength = sizeof(MEMORYSTATUSEX);
    
    rs = GlobalMemoryStatusEx(&meminfo);
    if(!rs){
        std::cout << "failed memoryStatus"<< GetLastError() << std::endl;
        return 0;
    }
    totalPyhs = meminfo.ullTotalPhys;
    memUsed = meminfo.ullTotalPhys - meminfo.ullAvailPhys;
    
    
    res = totalPyhs / (1024*1024);
    resA = memUsed / (1024*1024);
    res = res - resA;
    
    
    return res;
    
}

//Check local storage space
int info::freeSpace(){
    Json::Value obj;
    long long int totalFree;
    long long int freeBytes;
    long long int totalBytes;
    LPCSTR writeable = NULL;
    __int64 pFreeBytes;
    __int64 pTotalBytes;
    __int64 pTotalFreeBytes;
    DWORD dwSectPerClust, dwBytes, dwFreeCluster, dwTotalClusters;
    bool one;
    
    one = GetDiskFreeSpaceEx(writeable, (PULARGE_INTEGER)&pFreeBytes, (PULARGE_INTEGER)&pTotalBytes, (PULARGE_INTEGER)&pTotalFreeBytes);
    
    if(one){
        std::cout << GetLastError() << "failed disk space check" << std::endl;
        totalFree = 0;
        obj["space"] = res;
        cmC.sendRes(totalFree);
        
        return 0;
    }
    
    totalFree = pTotalFreeBytes;
    obj["space"] = totalFree;
    res = checkMemory();
    obj["mem"] = res;
    cmC.sendRes(obj);
    return totalFree;
    
}

//Change config file if os does not have enough memory/** may not need
bool info::changeMemory(std::string file){
    
    std::ifstream ifs(file);
    if(ifs.fail()){
        std::cout <<"failed ifs changememory" << std::endl;
        ifs.close();
        return false;
    }
    
    std::string err;
    Json::CharReaderBuilder reading;
    Json::Value obj;
    
    
    if(Json::parseFromStream(reading, ifs, &obj, &err)){
        Json::Value& datastore = obj["datastore"];
        Json::Value& badger = datastore["badger"];
        Json::Value& options = badger["badger_options"];
        
        Json::StreamWriterBuilder build;
        const std::unique_ptr<Json::StreamWriter> writer(build.newStreamWriter());

        options["table_loading_mode"] = "0";
        options["value_log_loading_mode"] = "0";

        std::ofstream ofs(file);
        writer->write(options, &ofs);
        ifs.close();
        ofs.close();
        return true;
    }
    
}

