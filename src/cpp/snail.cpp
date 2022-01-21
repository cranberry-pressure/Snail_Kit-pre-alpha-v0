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
#include <string>
#include <cstring>
#include <tchar.h>
#include "unistd.h"
#include "windows.h"
#include <fstream>
#include <ios>
#include <cstdio>
#include <stdlib.h>
#include <cstdlib>
#include "json/json.h"
#include "sqlite3.h"
#include "snail.h"
#include "cluster.h"
#include "follow.h"
#include "db1.h"

using namespace std;

cluster cmD;
dataB dbB;
follow flB;

bool snail::callNode(const char* cmdArg, const char* logFile){
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    HANDLE init = CreateFile(_T(logFile),FILE_APPEND_DATA, FILE_SHARE_WRITE | FILE_SHARE_READ, &sa, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE stdInput = NULL; //check**
    HANDLE childStd_in_wr = NULL;
    HANDLE childStd_out_rd = NULL;
    HANDLE childStd_out_wr = NULL; 
    STARTUPINFO startUpInfo;
    PROCESS_INFORMATION processInfo;
    
    char buff[500];
    
    ZeroMemory(&processInfo, sizeof(processInfo));
    bool ok = FALSE;
    ZeroMemory(&startUpInfo, sizeof(startUpInfo));
    startUpInfo.cb = sizeof(STARTUPINFO);
    startUpInfo.dwFlags = STARTF_USESHOWWINDOW;
    startUpInfo.wShowWindow = SW_HIDE;
    startUpInfo.dwFlags |= STARTF_USESTDHANDLES;
    startUpInfo.hStdError = init;
    startUpInfo.hStdInput = stdInput;
    startUpInfo.hStdOutput = init;
    
     
    LPCSTR cmdPath = "C:\\Windows\\system32\\cmd.exe";
    char appA[] = "C:\\Windows\\system32\\cmd.exe /c";
    
    strcpy(buff, appA);
    strcat(buff, cmdArg);
    
    ok = CreateProcess(cmdPath, buff, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startUpInfo, &processInfo);
        
    if(ok){
        std::cout << "Nope!!" << GetLastError() << std::endl;
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    
    DWORD rc;
    bool hope = GetExitCodeProcess(processInfo.hProcess, &rc);
    
    
    
    CloseHandle(init);
    CloseHandle(processInfo.hThread);
    CloseHandle(processInfo.hProcess);
    
    
    return ok;
   
        
}

//Shutdown running IPFS daemon/**may not need
void snail::shutdownNode(){
    //shutdown server node
    const char* log = "/shutLog.txt";
    const char* killB = "taskkill /F /IM 'ipfs.exe";
    bool ra;
    dbB.openDb();
    ra = callNode(killB, log);
    if(ra){
        std::cout << "Success kill ipfs-cluster" << std::endl;
    }
    std::string sql = "UPDATE startStat SET checkIpfs = 'false'";
    ra = dbB.prepare(sql);
    dbB.stepping();
    dbB.closeDb();
    return;
    
}

//Start IPFS daemon
bool snail::startIPFS(){
    const char* log = "./ipfsLog.txt";
    const char* node = "cd C:\\Program Files && ipfs daemon";
    bool ra;
    Json::Value obj;
    
    // test *************************
    std::string sql = "UPDATE startStat SET checkIpfs = 'true'";

    ra = callNode(node, log);
    if(!ra){
        std::cout<< "fail callNode startIpfs" << std::endl;
        obj["res"] = "failed callNode startIpfs";
        cmD.sendRes(obj);
        
        return false;
    }else{
        std::cout<< "Success callNode startIpfs" << std::endl;
        obj["res"] = "success callNode startIpfs";
        cmD.sendRes(obj);
    }
    
    
    ra = dbB.prepare(sql);
    dbB.stepping();
    dbB.closeDb();
    return true;
}

//Move exe to correct folder
void snail::setUp(){
    const char* log = "/setup.txt";
    const char* path = "C://Program Files";
    const char* IPFSNode = "./ipfs.exe";
    const char* IPFSClus = "./ipfs-cluster-service.exe";
    const char* follow = "./ipfs-cluster-follow.exe";
    const char* setEnvz = "setx IPFS ipfs.exe";
    const char* ipfsInitz = "cd C:\\Program Files && ipfs init";
    bool ra;
    
    std::ifstream ifs(IPFSNode, std::ios::in | std::ios::binary);
    if(ifs.fail()){
        std::cout << "failed ipfs.exe move" << std::endl;
        ifs.close();
        return;
    }
    
    std::ofstream out(path, std::ios::out | std::ios::binary);
    out << ifs.rdbuf();
    ifs.close();
    
    std::ifstream inClus(IPFSClus, std::ios::in | std::ios::binary);
    
    if(inClus.fail()){
        std::cout << "failed clus.exe move" << std::endl;
        inClus.close();
        return;
    }
    out << inClus.rdbuf();
    inClus.close();
    
    std::ifstream inFol(follow, std::ios::in | std::ios::binary);
    
    if(inFol.fail()){
        std::cout << "failed follow.exe move" << std::endl;
        inFol.close();
        return;
    }
    out << inFol.rdbuf();
    inFol.close();
    out.close();
    
    ra = callNode(setEnvz, log);
    if(!ra){
        std::cout << "callNode setEnz fail" << std::endl;
    }
    ra = callNode(ipfsInitz, log);
    if(!ra){
        std::cout << "callNode ipfsInit fail" << std::endl;
    }
    return;
    
    
}

//Create new cluster
bool snail::createC(Json::Value val){
    
    string clusInfo;
    
    bool ra;
    
    std::string err;
    Json::Value obj;
    
    //what? test*****
    //send with obj[Table]*****
    const std::string cName(val["ClusterName"].asString());
    const std::string consensus(val["Consensus"].asString());
    const char* clusN = cName.c_str();
    const char* home = getenv("USERPROFILE");
    const char* clusP = "/.ipfs-cluster/";

    char buff[100];
    strcpy(buff, home);
        strcat(buff, clusP);
        const char* nameA = buff;
        strcat(buff, clusN);
        _mkdir(buff);

        ra = cmD.service(nameA, buff, true, consensus);
        if(!ra){
            std::cout << "create cluster fail" << std::endl;
            return false;
        }

        //check
        cmD.fConfig(buff, clusN);
        std::string cons = cName + "Fsn.json";
        const char* conF = cons.c_str();
        strcat(buff, conF);
        
        val["Config"] = buff;
        ra = dbB.insert(val);
        if(!ra){
            std::cout << "insert CreateClus fail" << std::endl;
        }
        obj["addrs"] = cons;
        obj["res"] = "success creating new cluster";
        cmD.sendRes(obj);
                   
        return true;
    
}

//Add new follower cluster
bool snail::follower(Json::Value val){
    bool ra;
    
    std::string fName(val["ClusterName"].asString());
    std::string fAddr(val["addrs"].asString());
    val["nodeRole"] = "peer";
    val["table"] = "ClusterInfo";
        
    ra = flB.followerInit(fName, fAddr);
    if(!ra){
      
        return false;
    }

    
    ra = dbB.insert(val);
    if(!ra){
        std::cout << "failed insert followClus" << std::endl;
    }
    
        return true;
    
}

//Start all followed/owned clusters
void snail::startClus(const char* name){
    char buff[200];
    int rs;
    const char* log = "./sClog.txt";
    //test ~/.ipfs-cluster
    const char* path = "C\\Program Files\\.ipfs-cluster\\";
    const char* cmd = "cd C:\\Program Files && ipfs-cluster-service --config ";
    const char* dae = " daemon";
    Json::Value obj;
    strcpy(buff, path);
    strcat(buff, name);
    strcat(buff, dae);
    
    rs = callNode(buff, log);
    if(!rs){
        std::cout<< "fail callNode cluster" << std::endl;
        obj["res"] = "failed callNode cluster";
        cmD.sendRes(obj);
        
        return;
    }else{
        std::cout<< "Success callNode cluster" << std::endl;
        obj["res"] = "success callNode cluster";
        cmD.sendRes(obj);
    }
    //insert running cluster
    return;
   
}
