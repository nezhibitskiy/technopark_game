#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <set>
#include <algorithm>
#include <chrono>
#include <sys/select.h>
#include <sys/poll.h>
using namespace std;

class Clien {
private:
    int port;
    int clientSd;
    //char* serverIp;
    

    struct person{
        int id;
        int x,y;
    };

public:
 	Clien(int port_in){ // конструктор класса
        //strcpy(serverIp,"127.0.0.1");
        char serverIp[]="127.0.0.1";
        port=port_in;
        std::cout<<serverIp<<":"<<port<<std::endl;
        //setup a socket and connection tools 
        struct hostent* host = gethostbyname(serverIp); 
        sockaddr_in sendSockAddr;   
        bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
        sendSockAddr.sin_family = AF_INET; 
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
        sendSockAddr.sin_port = htons(port);
        clientSd = socket(AF_INET, SOCK_STREAM, 0);
        //try to connect...
        int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if(status < 0) {
            cout<<"Error connecting to socket!"<<endl; 
            exit(1);
        }
        cout << "Connected to the server!" << endl;
    }
 	
    // Отправить ID всем пользователям
    void Send_For_Server(){
        person get,post={4,5,6};
        send(clientSd, &post, sizeof(post), 0);
    }
    void Chek_New_Mesage_On_Sockets(bool &new_message){

    }
    void Get_Messages(){
        
    }
};
