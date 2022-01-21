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


const ipfsC = require('ipfs-cluster-api');
const cluster = ipfsC(port);
const ipfsClient = require('ipfs-http-client');
const ipfshttp = ipfsClient('http://localhost:5001');
var fls = require('fs');
const BufferList = require('bl/BufferList');

var hash;
var pingRes;
var gcRes;
var bootRes;
var bootAdd;
var bootList;
var resConf;
var resList;
var resKnown;
var addNew;
var idA;
var ver;
var opts = "null";
var clusPage = true;
var foldersP = false;
var page;
var port;

var addrs = {
    dir : "/"
};
var nodeP ={
    
};

var adding = {
    "act" : " ",
    "ClusterName" : "n/a",
    "addrs" : "n/a",
    "Consensus" : " ",
    
};

var items = {
    id:"" ,
    name:"" 
};

var swarmAddr ={
        id : {
            
            
        }, 
        addr : {
            
        }
};

var objClus = {
    
};
var ans = {
    
};

var objBw ={
    a: "n/a",
    b: "n/a",
    c: "n/a",
    d: "n/a"
};

var objBit = {
    a:"n/a",
    b:"n/a",
    c: "n/a",
    d: "n/a",
    e: "n/a",
    f: "n/a",
    g: "n/a"
};

var objRstat ={
    a: "n/a",
    b: "n/a",
    c: "n/a",
    d: "n/a"
};

var listDir = {
    
    name: {
        
    },
    type: {
        
    },
    size: {
        
    },
    cid: {
        
    }
   
};

var statList = {
    size: "",
    type: ""
};

window.onload = function(event){
    
    //SideBar
    document.getElementById("manageBtn").addEventListener('click', function() {grab("manage");}, false);
    document.getElementById("clusterBtn").addEventListener('click', function() {grab("cluster");}, false);
    document.getElementById("statusBtn").addEventListener('click', function() {grab("stat");}, false);
    document.getElementById("settingsBtn").addEventListener('click', function() {grab("settings");}, false);
    
    function grab(item){
        var ch;
        if(item = "manage"){
            ch = 'manage.html';
        }
        if(item = "cluster"){
            ch = 'clusters.html';
        }
        if(item = "stat"){
            ch = 'manage-status.html';
        }
        if(item = "settings"){
            ch = 'settings.html';
        }
        
        window.location.href = ch;
    }
    
    
    //manage____________________________________________________
    if(ch = 'manage.html'){
            //var pins = snlPls();
        var resB;
        if(foldersP){
            resB = snlClusLs(); 
            resB.then(setTimeout (() => {
                cols();

            }, 10000)).catch(error => function(){
                console.log("Failed snlClusLs");
                console.log(error.message);
            });


        }else{
                resB = snlFLs();
                resB.then(setTimeout (() => {
                    cols();

                }, 10000)).catch(error => function(){
                    console.log("Failed fLs()");
                    console.log(error.message);//maybe display area
                });
            }

        }
    }
    
    //status__________________________________________________________
    
    if(ch = 'status'){

        var data = document.getElementsByTagName("INFO");

        // call on home page, if error request start***
        var getId = snlId();
        getId.then(setTimeout (() => {
            data[0].innerHTML = idA;
        }, 15000)).catch(error => function(){
            console.log(error.message);
        });

        var getVer = snlVersion();  
        getVer.then(setTimeout (() => {
            data[1].innerHTML = ver;
        }, 15000)).catch(error => function(){
            console.log(error.message);
        });



        var getR = resource();//****************************************


        getR.then(setTimeout (() => {
            data[2].innerHTML ="" ;
        }, 5000)).catch();

        // get mem and storage
        // resource();   


        //callNode("memory");
       // data[2].innerHTML = WebSocket("AvailableMemory");

        //callNode("storage");
       // data[3].innerHTML =  WebSocket("AvailableStorage");

        var repoS = snlRepoStat();
        repoS.then(setTimeout( () => {
            data[4].innerHTML = objRstat.a; //snlRepoStat("numObjects");
            data[5].innerHTML = objRstat.b; //snailRepoStat("repoPath");
            data[6].innerHTML = objRstat.c; //snlRepoStat("repoSize");
            data[7].innerHTML = objRstat.d; //snlRepoStat("storageMax");
        }, 15000)).catch(error => function() {
            console.log(error.message);
        });

       var statBw = snlStatBw();
        statBw.then(setTimeout( () => {
            data[8].innerHTML = objBw.a;
            data[9].innerHTML = objBw.b;
            data[10].innerHTML = objBw.c;
            data[11].innerHTML = objBw.d;
        }, 15000)).catch(error => function() {
           console.log(error.message);
        });



        var statBit = snlBitStat();
        statBit.then(setTimeout(() =>{
            data[12].innerHTML = objBit.a;
            data[13].innerHTML = objBit.b;
            data[14].innerHTML = objBit.c;
            data[15].innerHTML = objBit.d;
            data[16].innerHTML = objBit.e;
            data[17].innerHTML = objBit.f;
            data[18].innerHTML = objBit.g;
        }, 15000)).catch( error => function(){
            console.log(error.message);
        });

    
    }
    
   
    //cluster______________________________________________
    if(ch = 'cluster.html'){
        
        myList();    
    
        var file = document.getElementById("cList");
        
        file.onchange = function(){
            if(this.files && this.files[0]){
                var readg = new FileReader();
                readg.onload = function(evt) {
                    var nObj = evt.target.result;
                    var resA = JSON.parse(nObj);
                    clusPage = false;
                    createNodes(resA);
                }
            }
        }
        
    }
    
    
    
    
}

// create___________________________________


document.getElementById("create").addEventListener('click', () => {create();});
document.getElementById("add").addEventListener('click', () => {add();});

if(document.querySelector('input[name="Pri"]')){
            document.querySelectorAll('input[name="Pri"]').forEach((item) => {
                item.addEventListener("change", function(event) {
                    adding.con = event.target.value;
                })
            })
}

//idk about 2 callNode in smae function???
function create(){
    
    adding.name = document.getElementById("cName").value;
    adding.act = "newCluster";
    adding.ClusterName = addng.name;
    adding.Consensus = adding.cons;
        
    callNode(adding);
    clusRes();
    
    var config = snailAdd(ans.addrs);
    config.then(setTimeout (() => {
        adding.act = "cUpdate";
        adding.addrs = hash;
        callNode(adding);
        
        clusRes();
        document.getElementById("detail-areaB").innerHTML = "Success!!" + ans.res;
        
    }, 15000)).catch(error => function(){
        console.log(error.message);
    });
    
}

//follow new cluster/newFollow on clusterList
function add(){
    adding.ClusterName = document.getElementById("aName").value;
    adding.addrs = document.getElementById("addrs").value;
    adding.act = "follow";
    
    
    callNode(adding);
    clusRes();

    document.getElementById("detail-areaA").innerHTML = "Success!!" + ans["res"];
    
}


//manage____________________________________________________ 


function dirN(id){
    var node = document.getElementById(id);
    var child = node.childNodes;
    var sizd = child[3].childNodes;
    var typeA = sizd[3].childNodes;
    var childA = child[1].childNodes;
    var nameA = childA[1].childNodes;
    var addressA = childA[3].childNodes;
    
    if(typeA[1].innerHTML == 1){
        addrs.dir = addrs.dir + nameA[1] + "/";
        var resA = snlFLs(addrs.dir);
        resA.then(setTimeout (() => {
            foldersP = false;
            window.location.href = 'manage.html';
        }, 10000)).catch(error => function(){
            console.log("failed select snlFLs folder");
        })
        
    }else{
        
        if(nodeP[node.id] == true){
            //nodeP[node.id] = false;
            delete nodeP[node.id];
            document.getElementById(id).style = "background-color:none";
        }else{
            nodeP[node.id] = true;
            document.getElementById(id).style = "background-color:yellow";
        }
        
    }
    
    
}

function cols(){
    
    
    var node = document.getElementById("f1");
    
    
    //make new nodes
    for(var i = 0; i == fold; i++){
        var newNode = node.cloneNode(true);
        newNode.id = "f1" + i;
        document.getElementById("scob").appendChild(newNode);
        
        
        var child = newNode.childNodes;
        var childA = child[1].childNodes;
        var nameA = childA[1].childNodes;
        var addressA = childA[3].childNodes;
        var sizd = child[3].childNodes;
        var sizeA = sizd[1].childNodes;
        var typeA = sizd[3].childNodes;
        nameA[2].innerHTML = listDir.name[i];
        addressA[3].innerHTML = listDir.cid[i];
        sizeA[1].innerHTML = listDir.size[i];
        if(listDir.type[i] == 1){
            typeA[1].innerHTML = "directory";
        }else{
            typeA[1].innerHTML = "file";
        }
        
    }
    
    node.style = "display:none";
}



 
document.getElementById("nwF").addEventListener('click', function() {nwF();}, false);
document.getElementById("foldCre").addEventListener('click', function() {mkDir();}, false);

function nwF(){
    document.getElementById("nwF").style = "display:none";
    document.getElementById("foldNew").style = "display:block";
    document.getElementById("foldCre").style = "display:block";
    /*
    var folds = document.getElementById("nwF");
    folds.innerHTML = "Create";
    folds.id = "nwFA";
    */
}

function mkDir(){
    var folds = document.getElementById("foldNew").value;
    var path = addrs.dir + folds;
    var newDir = snlFmkdir(path);
    newDir.then(setTimeout (() => {
        document.getElementById("display-area").innerHTML = "Successful new folder";
    }, 10000)).catch(error => function(){
        document.getElementById("display-area").innerHTML = "failed new Folder";
        console.log(error.message);
    });
    document.getElementById("foldNew").style = "display:none";
    document.getElementById("foldCre").style = "display:none";
    document.getElementById("nwF").style = "display:block";
}

document.getElementById("rmPin").addEventListener('click', function() {rm();}, false);
function rm(){
    var node;
    var child;
    var childA;
    var addrA;
    
    for(const [key,value] of Object.entries(nodeP)){
        
        if(value == true){
            node = document.getElementById(key);
            child = node.childNodes;
            childA = child[1].childNodes;
            addrA = childA[3].childNodes;
            
            var removing = snlFrm(addrA);
            removing.then(setTimeout(() => {
                console.log("succes fileRemove");
            }, 10000)).catch(error => function(){
                console.log(error.message);
                console.log("failed fileRemove");
                
            });
                
            removing = snlPinRm(addrA);
            removing.then(setTimeout(() => {
                console.log("succes unpinn + Fremoving");
                document.getElementById("display-area").innerHTML = "Success removing item/s manage page wil reflect on reload";
            }, 10000)).catch(error => function(){
                console.log(error.message); 
                document.getElementById("display-area").innerHTML = "failed removing item/s"; 
            });
        }
        delete nodeP.key;
        
    }
    
    
}

//open in browser with localhost gateway
document.getElementById("viewF").addEventListener('click', function(){viewF();});
function viewF(){
    var node;
    var child;
    var childA;
    var addrA;
    
    for(const [key, value] of Object.entries(nodeP)){
        if(value == true){
            node = document.getElementById(key);
            child = node.childNodes;
            childA = child[1].childNodes;
            addrA = childA[3].childNodes;
            var path = "localhost:8080/ipfs/" + addrA;
            window.open(path);
            
        }
        delete nodeP.key;
        break;
    }
    
    
    
}


document.getElementById("addFbtn").addEventListener('click', function() {addFiles();});

function addFiles(){
    var resA;
    var pathA;
    document.getElementById('addF').click();
    var file = document.getElementById("addF");
        
        file.onchange = function(e){
            if(this.files && this.files[0]){
                var readg = new FileReader();
                readg.onload = function(evt) {
                    var nObj = evt.target.result;
                    resA = JSON.parse(nObj);
                
                }
            }
        }

    var addF = snlAddGlob(resA);
    addF.then(setTimeout (() => {
        
        pathA = "ipfs/" + addNew;
        
    }, 10000)).catch(error => function(){
        console.log(error.message);
        console.log("failed adding file");
    });
    var pins = snlPinAdd(addNew);
    pins.then(setTimeout(() => {
            console.log("pin successful");
            
        })).catch(error => function(){
            console.log("failed pin");
            
        });
    var copyA = snlFCp(pathA, addrs.dir);
        copyA.then(setTimeout (() => {
            console.log("Successful file added to MFS");
            document.getElementById("display-area").innerHTML = "success adding file to" + addrs.dir;
        }, 10000)).catch(error => function(){
            console.log("Failed Copy to mfs");
            console.log(error.message);
        });
      
}


//delete from ipfs completely
document.getElementById("moving").addEventListener('click', function() {multiMv();});
function multiMv(){
    //block all options expect add button/search bar/clickable divs
    document.getElementById("moving").style= "display:none";
    document.getElementById("chooses").style = "display:block";
    document.getElementById("display-area").innerHTML = "Please go to destination folder then click choose.";
    
    
}

//**************************************************************************
document.getElementById("chooses").addEventListener('click', function(){choosePath();});

function choosePath(){
    //block all options expect add button/search bar/clickable divs/cancel button
    
        for(const [key, value] of Object.entries(nodeP)){
            if(value == true){
                pathB = addrs.dir;
                var mov = snlFmv(key,pathB);
                mov.then(setTimeout (() => {
                    console.log("successful move");
                    document.getElementById("display-area").innerHTML = "Successful move of files";
                }, 10000)).catch(error => function(){
                    console.log("failed moving");
                    document.getElementById("display-area").innerHTML = "failed moving";
                });
                delete nodeP.key;
                
            }
            
            
        }
        
    
    
}
document.getElementById("cancel").addEventListener('click', function() {cancel();});

function cancel(){
    for(const [key, value] of Object.entries(nodeP)){
        
        document.getElementById(key).style = "background-color:none";
        delete nodeP.key;
    }
}


//settings_____________________________________________________________

document.getElementById("PingGo").addEventListener('click', function(){pingPeers(pingId);});

async function pingPeers(id){
    var pingId = document.getElementById("Pingpeer").value;
    var pinging = snlPing(id);
    pinging.then(setTimeout (() => {
        document.getElementById("displayPing").innerHTML = pingRes;
    }, 15000)).catch(error => function(){
        document.getElementById("displayPing").innerHTML = pingRes;
        console.log(error.message);
    });
    
}



document.getElementById("GarbageGo").addEventListener('click', function(){garbage();});

async function garbage(){
    var garb = snlRepoGc();
    garb.then(setTimeout (() => {
        document.getElementbyId("displayGarbage").innerHTML = gcRes;
    }, 15000)).catch(error => function(){
        document.getElementbyId("displayGarbage").innerHTML = gcRes;
        console.log(error.message);
    });
    
}

var bootP = document.getElementById("BootPeer").value;
document.getElementById("goBootadd").addEventListener('click', function(){bAdd(bootP);});

async function bAdd(bootPa){
    var bootP = document.getElementById("BootPeer").value;
    var bootA = snlBootstrapAdd(bootPa);
    bootA.then(setTimeout (() => {
        document.getElementById("displayBoot").innerHTML = bootAdd;
    }, 15000)).catch(error => function(){
        document.getElementById("displayBoot").innerHTML = bootAdd;
        console.log(error.message);
    });
     
}

document.getElementById("goBootrm").addEventListener('click', function(){bRm(bootP);});
                                                
async function bRm(bootPrm){
    var bootR = snlBootstrapRm(bootPrm);
    bootR.then(setTimeout(() => {
        document.getElementById("displayBoot").innerHTML = bootRes;
    }, 15000)).catch(error => function(){
        document.getElementById("displayBoot").innerHTML = bootRes;
        console.log(error.message);
    });
    
}

document.getElementById("ShowBoot").addEventListener("click", function(){bList();});
                                                     
async function bList(){
    var listB = snlBootstrapList();
    listB.then(setTimeout(() => {
        document.getElementById("displayBoot").innerHTML = bootList;
    }, 15000)).catch(error => function(){
        document.getElementById("displayBoot").innerHTML = bootList;
        console.log(error.message);
    });
    
}


document.getElementById("goConfig").addEventListener('click', function(){configGet();});
                                                     
async function configGet(){
    var configA = snlConfigGet();
    configA.then(setTimeout(() => {
        document.getElementById("displayConf").innerHTML = resConf;
    }, 15000)).catch(error => function(){
        document.getElementById("displayConf").innerHTML = resConf;
        console.log(error.message);
    });
     
}



document.getElementById("goRenameK").addEventListener('click', function(){renameK(oldkey, newKey);});
                                                     
async function renameK(){
    var oldKey = document.getElementById("KeyOld").value;
    var newKey = document.getElementByid("KeyNew").value;
    var kRename = snlKeyRename(oldKey, newKey);
    kRename.then(setTimeout(() => {
        document.getElementById("displayRename").innerHTML = "New Name:" + items.name +"Id:" + items.id ;
    }, 15000)).catch(error => function(){
        document.getElementById("displayRename").innerHTML = "error!!;
        console.log(error.message);
    });
    
}


document.getElementbyId("goKeyL").addEventListener('click', function(){keyL();});
                                                   
async function keyL() {
    var Lkey = snlKeyList();
    Lkey.then(setTimeout(() => {
       document.getElementById("displayKeys").innerHTML= resList; 
    }, 15000)).catch(error => function(){
        document.getElementById("displayKeys").innerHTML= resList;
        console.log(error.message);
    });
    
}


document.getElementById("goRemoveKey").addEventListener('click', function(){rmKey(rName);});
                                                        
async function rmKey(name){
    var rName = document.getElementById("removeK").value;
    var keyRm = snlKeyRm(name);
    keyRm.then(setTimeout(() => {
        document.getElementById("displayEx").innerHTML = "Removed" + itmes.name + items.id;
    }, 15000)).catch(error => funtion(){
              document.getElementById("displayRename").innerHTML = "Failed Remove";
            console.log(error.message);
        });
    
}


document.getElementbyId("goNewName").addEventListener('click', function(){newName();});
                                                      
async function newName(){
    var newKey = document.getElementById("KeyNewName").value;
    
    var genK = snlKeyGen(newKey);
    genK.then(setTimeout(() => {
        document.getElementById("displayNewName").innerHTML = "New Key Id: " + items.id + "Name" + newKey;
    }, 15000)).catch(error => function(){
        document.getElementById("displayNewName").innerHTML = "failed key generation";
        console.log(error.message);
    });
    
}

document.getElementById("knownConnect").addEventListener('click', function(){connknow();});
                                                         
async function connKnow()){
    var swarmA = snlSwarmAddrs();
    swarmA.then(setTimeout( () => {
        document.getElementById("displaySwarm").innerHTML = "Id:" + swarmAddr.id + " Address:" + swarmAddr.addr;
    }, 15000)).catch(error => function(){
        document.getElementById("displaySwarm").innerHTML = "Failed xx";
        console.log(error.message);
    });
    
}

document.getElementById("goLocalAddr").addEventListener('click', function(){localS();});
                                                        
async function localS()){
    var swarmL = snlSwarmLocalAddrs();
    swarmL.then(setTimeout(() => {
        document.getElementById("displaySwarm").innerHTML = resKnown;
    }, 15000)).catch(error => function(){
        document.getElementById("displaySwarm").innerHTML = "Failed ";
        console.log(error.message);
    });
    
}

document.getElementById("disconnect").addEventListener('click', function(){swarmD();});
                                                       
async function swarmD(){
    var addrPeer = document.getElementById("SwarmAddr").value;
    var pAddr = snlSwarmDis(addrPeer);
    pAddr.then(setTimeout(() => {
        doucment.getElementById("displayConn").innerHTML = resConn;
    }, 15000)).catch(error => function(){
        doucment.getElementById("displayConn").innerHTML = "Failed xxx";
        console.log(error.message);
    });
    
}


document.getElementById("goConnect").addEventListener('click', function(){swarmC();});
                                                      
async function swarmC(){
    var addrPeer = document.getElementById("SwarmAddr").value;
    var connS = snlSwarmConn(addrPeer);
    connS.then(setTimeout(() => {
        document.getElementById("displayConn").innerHTML= resConn;
        }, 15000)).catch(error => function(){
            document.getElementById("displayConn").innerHTML= "Failed xxxx";
            console.log(error.message);
    });
    
    
}



var x = document.getElementById("contents");
document.getElementById("act").addEventListener('click', function(){displayContent(x);});


function displayContent(x){
    if(x.style.display === "none"){
        x.style.display ="block";
    }
    else{
        x.style.display="none";
    }
}


//cluster______________________________________________

//return all clusters in json format/ name,addrs,port
document.getElementById("myList").addEventListener('click', function(){myList();});
function myList(){
    adding.act = "ClusInfo";
    callNode(adding);
    clusRes();
    var pick = ans["res"];
    if(pick = "Failed ClusInfo"){
        document.getElementById("display-area").innerHTML = ans["res"];
        return;
    }
    //clusPage = "myList";
    resA = ans;
    createNodes(ans);
}

//check correct json and buttons 
function createNodes(var obj){
   
    for(var i =0; i < obj.length; i++){
        var nnode = document.getElementById("clusInfo"); 
        var newNode = node.cloneNode(true);
        newNode.id = "clusInfo" + i;
        var child = newNode.childNodes;
        var nodeA = child[3].childNodes;
        var name = child[1].childNodes;
        name[1].innerHTML = obj[i].name;
        name[3].innterHTML = obj[i]addrs;
        nodeA[1].id = "fol" + i; //follow btn
        nodeA[5].id = "mana" + i //manage btn
        
        if(clusPage){
            nodeA[1].innerHTML = "unFollow";
            
        }else{
            nodeA[1].innerHTML = "unFollow";
        }
        document.getElementById("rows").appendChild(newNode);
        
    }

}


document.getElementById("fol").addEventListener('click', function(){follow(this.id);});

function follows(id){
    //join
    var parNode = document.getElementById(id).parentNode.id;
    var parNodeA = document.getElementById(parNode).parentNode.id;
    var clus = "clusInfo" + parNodeA;
    var clusA = document.getElementById(clus);
    var nodeA = clusA.childNodes;
    var nodeB = nodeA[1].childeNodes;
    var name = nodeB[1].innerHTML;
    var address = nodeB[2].innerHTML;
    
    if(!clusPage){
        
        adding.act = "follow";
        adding.ClusterName = name;
        adding.addrs = address;
    
        callNode(adding);
        clusRes();
        document.getElementById("display-area").innerHTML = ans["res"];
        
    }else{
        
        adding.act = "unfollow";
        adding.ClusterName = name;
        callNode(adding);
        clusRes();
    
        document.getElementById("display-area").innerHTML = ans["res"];
        
    }
     
    
}

//open followed cluster's pin list/ peer 
function foList(){
    page = true;
    var clus = "clusInfo" + id;
    var clusA = document.getElementById(clus);
    var nodeA = clusA.childNodes;
    var nodeB = nodeA[1].childeNodes;
    var name = nodeB[1].innerHTML;
    
    adding.act = "foList";
    adding.ClusterName = name;
    callNode(adding);
    clusRes();
    resA = ans;
    window.location.href = 'manage.html';
}


document.getElementById("fol").addEventListener('click', function(){manageC(this.id);});
// operator
function manageC(id){
    page = true;
    var parNode = document.getElementById(id).parentNode.id;
    var parNodeA = document.getElementById(parNode).parentNode.id;
    var clus = "clusInfo" + parNodeA;
    var clusA = document.getElementById(clus);
    var nodeA = clusA.childNodes;
    var nodeB = nodeA[1].childeNodes;
    var name = nodeB[1].innerHTML;
    //return port
    
    adding.act = "clusPort";
    adding.ClusterName = name;
    
    callNode(adding);
    clusRes();
    
    port = ans["res"];
    foldersP = true;
    window.location.href = 'manage.html';
    
}





//SnailApi: IPFS + IPFSCLUSTER + Resource   -------------------------------------------------------------------------------------------------------------------------------------------



//resource________________________________

function callNode(name){
    
    
    fls.writeFile("./request.json", JSON.stringify(name), function(err)){
            if(err) throw err;
            console.log('complete');
        }
    
    //set timeout inbetween calls
    nw.Shell.openItem("./snail.exe");
    
}


function clusRes(){
    
    var opt = fls.readFileSync("./response.json", 'utf8');
    ans = JSON.parse(opt);
    
    
}

function resource(){
    
    adding.act = "status";
    callNode(adding);
    
    clusRes();
    data[2].innerHTML = ans["mem"];
    data[3].innerHTML = ans["space"];
    
    
}

function shutdown(){
    adding.act ="shutdown";
    callNode(adding);
}


//IPFS/Cluster Node_____________________________________

//Add a File to the network(txt) **options**/create.html
async function snailAdd(var data){
    for await (const cors of ipfshttp.add(data)){
        new Promise(function(resolve, reject) => {
            if(cors){
            
                setTimeout (() => resolve(console.log(cors)),8000);
                setTimeout(() => {
                    hash = cors["cid"];
                }, 10000);
            
           }else{
                setTimeout(()=> reject(new Error('nope')), 9000)}
            })
        }
}

//Manage.html
//type:1 = directory /**try with empty path
async function snlFLs(path){
    
    var i =0; 
    for await( const lsFil of ipfshttp.files.ls(path)){
        new Promise((resolve,reject) =>{
            if(lsFil){
                setTimeout (() => {
                    //console.log(lsFil);
                    i = i + 1;
                    listDir.name[i] = "Cluster";
                    listDir.type[i] = lsFil.type;
                    listDir.size[i] = lsFil.size;
                    listDir.cid[i] = lsFil.cid;
                            //console.log(i);
                    },10000);
            }else{
                setTimeout(() => reject(console.log('nopes')), 10000);
                    
            }

                    
        })
    }
    
}


//List of pins in Cluster pinlist
async function snlClusLs(){
     const cluster = ipfsC(port);       
    await clusters.pin.ls({filter: 'all'},(err,peers) => {
        
        if(!err){
            for(var i = 0; i < peers.length; i++){
                listDir.type[i] = peers[i]["type"];
                listDir.size[i] = peers[i]["cid"]['/'];
                //console.log("ClusPinLs success");
            }else{
                console.log(err);
                
            }
        }
    })
    

}


async function snlPinRm(hash){
    
    const pind = await ipfshttp.pin.rm(hash);
    
            if(pind){
                setTimeout (() => {console.log("Succesful unpin")}, 10000);
             }else{
                setTimeout (() => {console.log("failed unpin")}, 10000);
             }
            
   

}

//Add IPFS onject to pinset and store in IPFS repo
async function snlPinAdd(path){
    
    const pind = await ipfshttp.pin.add(path)
    
            if(pind){
                setTimeout (() => ( console.log(pind)), 10000);
            }
            else{
                setTimeout(() => (console.log('failed pinAdd')),8000);
            }
   

}


// Add a file directly from filesystem('./docs') **options** auto pin all 
async function snlAddGlob(data){
    for await(const corAdd of await ipfshttp.add(globSource(data, {recurisive: true}))){ 
        new Promise(function(resolve, reject){
            if(corAdd){
                setTimeout (() =>{
                    addNew = corAdd.cid;
                } ,10000);
                
            }else{
                setTimeout(() => reject(console.log('no addGlob')), 10000);
            }
            })
        }
}
    



// MFS Copy Files(options, reports rejection instead resolve)
async function snlFCp(path1, path2){
        const copies = await ipfshttp.files.cp(path1, path2);
        if(copies){
            setTimeout (() => console.log('Successful copy'), 10000);
        }else{
            console.log("failed copy");
        } 
}

async function snlFmkdir(path){
        const mkS = await ipfshttp.files.mkdir(path);
        if(mkS)
        {
            setTimeout (() => console.log(mkS), 10000);
        }else{
            setTimeout (() => console.log('failed mkDir'), 10000);
        }
}



//MFS remove file or directory (directory recurisve)
async function snlFrm(path){
        const soons = await ipfshttp.files.rm(path);
        if(soons){
                setTimeout (() => console.log('yup removed file'), 10000);
        }
        
}

//MFS Get file or directory stat (opts: withLocal true)
async function snlFstat(path){
    const statRes = await ipfshttp.files.stat(path);
    if(statRes){
        setTimeout (() => {
            listDir.type = statRes.type;
            if(listDir.type == "file"){
                listDir.size = statRes.size;
            }else{
                listDir.size = statRes.blocks;
            }
        }, 10000);
    }else{
        console.log("files stat fail");
    }

}


//MFS move files (options, reports no but good)
async function snlFmv(path1,path2){
    var mvF = await ipfshttp.files.mv(path1, path2);
    
    if(mvF){
        setTimeout(() => console.log("Successfful move files"), 10000);
    }else{
        setTimeout(() => console.log("no file moved"), 9000);
    }
    
}


//status.html
  
//Show stat of bitswap agent
async function snlBitStat(){
    const bStat = await ipfshttp.bitswap.stat()
    
    if(bStat){
            setTimeout (() => console.log(bStat),  10000);
            setTimeout (() => {
                    objBit.a = bStat["provideBufLen"],
                    objBit.b = bStat["blocksReceived"].c,
                    objBit.c = bStat["dataReceived"].c,
                    objBit.d = bStat["dataSent"].c,
                    objBit.e = bStat["blocksSent"].c,
                    objBit.f = bStat["dupBlksReceived"].c,
                    objBit.g = bStat["dupDataReceived"].c
            }, 10000);
        }
        else{
            setTimeout (() => console.log("Failed BitswapStat"), 7000);
        }

}

//Get stats for current repo
async function snlRepoStat(){
    var statsR = await ipfshttp.repo.stat({human: true})
            if(statsR){
                setTimeout(() => (console.log("Susccess repo stat")),10000);
                setTimeout(() => {
                    objRstat.a = statsR["numObjects"].c;
                    objRstat.b = statsR["repoPath"].c;
                    objRstat.c = statsR["repoSize"].c;
                    objRstat.c = statsR["storageMax"].c;
                
                }, 10000);
            }
            else{
                console.log('Fail repo stat');
                
            }

}

async function snlStatBw(){
        for await( const info of ipfshttp.stats.bw()){
             new Promise((resolve,reject) => {
                
            if(info){
                setTimeout (() => resolve(console.log("Success statBw ")), 10000);
                 setTimeout (() => {
                    objBw.a = info["totalIn"].c;
                    objBw.b = info["totalOut"].c;
                    objBw.c = info["rateIn"].c;
                    objBw.d = info["rateOut"].c;
                
                 }, 10000);
            }
            else{
                setTimeout(() => reject(console.log('An error phas occured, statBw')), 9000);
            }
            
            })
        } 
}

// @Returns Peer Identity
async function snlId(){
 var getId = await ipfshttp.id()
   if(getId){
        setTimeout (() => {
            console.log("Id Success"); 
            idA = getId["id"];
        },10000);
        
   }else{
        console.log('Id Fail');
   }
}

async function snlVersion(){
    var versionA = await ipfshttp.version()
    
    if(versionA){
            setTimeout (() => console.log("Success version"),  10000);
            setTimeout (() => {
                ver = versionA["version"];
            }, 11000);
        }
        else{
            console.log("version fail");
            
        }

}

//settings

async function snlPing(ids){
    for await( const pings of ipfshttp.ping(ids)){
        return new Promise(function(resolve,reject){
            if(pings.time){
                setTimeout(() => resolve(console.log('Success Ping')), 10000)
                setTimeout(() => {pingRes = "Successfull ping at time=$pings.time";}, 10000);
            }
            else{
                setTimeout(() => reject(console.log(ping.text)),10000)
                setTimeout(() => {pingRes = "Unsuccessful ping"}, 10000)
            }
    })}
    
    

}

//garbage collection on repo
async function snlRepoGc(){
        
    for await(const core of ipfshttp.repo.gc()){
        return new Promise((resolve,reject) => {   
            if(core){ 
                setTimeout (() => resolve(console.log("Successful garbage collection sweep on the repo")), 10000); 
                setTimeout (() => {gcRes = "Successful garbage collection sweep on the repo!!"}, 10000); 
            }else{  
                setTimeout(() => reject(console.log('Unsuccessful')), 9000); 
                gcRes = "Unsuccessful";
                } 
            })
        }

}

//Remove a peer from bootstrap list
async function snlBootstrapRm(peer){
    const rmPeerB = await ipfshttp.bootstrap.rm(peer);
            if(rmPeerB){
                setTimeout(() => {
                    bootRes = rmPeerB.peers; 
                    console.log("Success remove peers");
                },10000); 
            }
            else{
                setTimeout(() => console.log('no removed removed'),10000);
                bootRes = "no remoeved peer";
            }

}

// Show all peers in bootstap list
async function snlBootstrapList(){
    const listB = await ipfshttp.bootstrap.list();
            if(listB){
                setTimeout(() => {
                    bootList = listB.peers; 
                    console.log("Success bootList!")
                },10000);
                          // console.log(coo.Peers))10000);
            }
            else{
                setTimeout(() => (console.log('no list')),10000);
                bootList = "No boot list";
            }
   

}

//Add a peer to bootstrap list
async function snlBootstrapAdd(peer){
    const bootA = await ipfshttp.bootstrap.add(peer);
            if(bootA){
                setTimeout(() => {
                    bootAdd = bootA.peers; 
                    console.log("Succesfully add peer")
                } ,10000);
            }
            else{
                setTimeout(() => (console.log('no add to boot')),10000);
                bootAdd = "no added to bootstrap list";
            }

}

//Get the current config
async function snlConfigGet(){

    const con = await ipfshttp.config.get();
            if(con){
                setTimeout (() => {
                    console.log("Config File: "); 
                    resConf = con; 
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('nope')),10000);
                resConf = "no config file";
            }
}

//Rename a key
async function snlKeyRename(oldName, newName){
    const reNam = await ipfshttp.key.rename(oldName, newName);
            if(reNam){
                setTimeout (() => { 
                    items.id = reNam.id; 
                    items.name = reNam.name; 
                    console.log(reNam);
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('no rename key')), 10000);
                //res = "did not rename";
            }

}

//Remove a key
async function snlKeyRm(name){
    const rmKey = await ipfshttp.key.rm(name);
            if(rmKey){
                setTimeout (() => {
                    items.id = rmKey.id; 
                    items.name = rmKey.name; 
                    console.log(coo);
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('no rm key!')),11000);
                //res ="no key removed";
            }
}

//List all the keys
async function snlKeyList(){
    const list = await ipfshttp.key.list();
            if(list){
                setTimeout (() => {
                    resList = list; 
                    console.log("Success keyList");
                }, 10000);
                
             }
            else{
                setTimeout(() => (console.log('no key list')),8000);
                resList = "no key list";
                
            }
}

//Generate a new Key
async function snlKeyGen(name){
    const keyG = await ipfshttp.key.gen(name, {type:'rsa', size:2048});
    
            if(keyG){
                setTimeout (() => {
                    console.log(keyG.id); 
                    items.id = keyG.id; 
                    
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('no new Key')),8000);
                //res = " not generated";
            }
}


//Local addrs this node listening on
async function snlSwarmLocalAddrs(){
    const coo = await ipfshttp.swarm.localAddrs();
            if(coo){
                setTimeout (() => console.log("Success Local Address"), 10000);
                setTimeout (() => {
                    resKnown = coo;
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('nope')),8000);
                //res = "Local address not working!";
            }
    
   

}

//Disconnect to an address
async function snlSwarmDis(addrs){
    const coo = await ipfshttp.swarm.disconnect(addrs);
            if(coo){
                setTimeout (() => console.log("Succesful Disconnect"), 10000);
                setTimeout(() => {
                    resConn = "Disconnected";
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('no disconnect')),8000);
                res = "Disconnect did not work";
            }
    
   

}

//Connect to an address(multiadd)
async function snlSwarmConn(addrs){
    const coo = await ipfshttp.swarm.connect(addrs);
            if(coo){
                setTimeout (() => console.log("Success Connect"), 9000);
                setTimeout(() => {
                    resConn = "Connected";
                }, 10000);
             }
            else{
                setTimeout(() => (console.log('no connect')),8000);
                res ="not work";
            }
}

//List known connected peers
async function snlSwarmAddrs(home){
    
    
    const coo = await ipfshttp.swarm.addrs();
            if(coo){
                coo.forEach((infoss, i) => { 
                    setTimeout (() => {
                        swarmAddr.id[i] = infoss.id; 
                        console.log(swarmAddr.id[i]); 
                    }, 10000);                     
                infoss.addrs.forEach((addr) => setTimeout(() => {
                    swarmAddr.addr[i] = addr;
                }, 15000));
            })}
            //}
            else{
                setTimeout(() => (console.log('nope')),8000);
                res = "Swarm address list not working";
            }
    

}
