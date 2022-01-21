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

#ifndef CLUSTER_H
#define CLUSTER_H
class cluster{
public:
    bool sendRes(Json::Value obj);
    bool updateC(Json::Value obj);
    bool delClus(std::string name);
    bool fConfig(const char* path, const char* name);
    bool clusIt(const char* pathMain, const char* newPath);
    bool grabPorts();
    bool clusS(std::string sql, bool callC, bool beginM);
    bool service(const char* clusPath, const char* clusPathB, bool initFlag, std::string consensus);
    bool startF =  false;
    
private:
    bool call = true;
    bool del = false;
    bool portC = false;
    std::string port;
    char buff[400];
    char buffA[400];
    
    int rc;
};


#endif /* CLUSTER_H */

