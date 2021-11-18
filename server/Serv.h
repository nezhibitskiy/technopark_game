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

class Serv {
private:
    int all_clients;
    int sd_connect[10]={0};
    fd_set readset;
    int serverSd;
    timeval timeout;
    struct person{
        int id;
        int x,y;
    };

public:
 	Serv(int all_clients_acc){ // конструктор класса
        all_clients=all_clients_acc;
        int port=11113,vspom;
        //setup a socket and connection tools
        sockaddr_in servAddr;
        bzero((char*)&servAddr, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(port);
    
        //open stream oriented socket with internet address
        //also keep track of the socket descriptor
        serverSd = socket(AF_INET, SOCK_STREAM, 0);
        if(serverSd < 0) {
            std::cout << "Error establishing the server socket" << endl;
            //exit(0);
        }
        //bind the socket to its local address
        int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
            sizeof(servAddr));
        if(bindStatus < 0) {
            std::cout << "Error binding socket to local address" << endl;
            //exit(0);
        }
        std::cout << "Waiting for a client to connect..." << endl;
        //listen for up to 5 requests at a time
        listen(serverSd, 5);
        //receive a request from client using accept
        //we need a new address to connect with the client
        sockaddr_in newSockAddr;
        socklen_t newSockAddrSize = sizeof(newSockAddr);
        //accept, create a new socket descriptor to 
        //handle the new connection with client
        
        // Задаём таймаут
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        std::cout << endl;
        for(int i = 0; i < all_clients; i++){
            sd_connect[i] = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
            if(sd_connect[i] < 0) {
                std::cout << "! Error accepting request from client "<<i+1<<". ID: "<<sd_connect[i] << endl;
            }else{
                std::cout << "Connected with client "<<i+1<<". ID: "<<sd_connect[i] << endl;
            }
            FD_ZERO(&readset);
            FD_SET(sd_connect[i], &readset);
        }
    }
 	
    // Отправить ID всем пользователям
    void send_Clients_ID(){
        int vspom;
        for(int i = 0; i < all_clients; i++){
            vspom=sd_connect[i];
            send(sd_connect[i], &vspom, sizeof(vspom), 0);
        }
    }

    // Получить кол-во клиентов
    int get_all_clients() { return all_clients; }

    // Отправить данные всем клиентам
    void sends_for_clients() {
        person asd={1,2,3};
        for(int i = 0; i < all_clients;i++){
            send(sd_connect[i], &asd, sizeof(asd), 0);
            std::cout<<"Sending to "<< sd_connect[i]<<endl;
        }
    }

    // Отправить данные определённому клиенту
    void send_for_client(int clientID) {
        person asd={1,2,3};
        send(clientID, &asd, sizeof(asd), 0);
        std::cout << "Sending to " << clientID << endl;
    }
    
    // Функция, которая должна изменять переменную new_message, когда происходит изменение в сокетах
    void Chek_New_Mesage_On_Sockets(bool &new_message){
        person get;
        //std::cout<<"chek"<<std::endl;
        int bytes_read;
        new_message=false;
        for (int i = 0; i < all_clients; i++) {
            std::cerr << "sizeof readset = " << sizeof(readset) << std::endl;
            //std::cout<<*(sd_connect+i)<<std::endl;
            if(*(sd_connect+i)!=0){
                if (FD_ISSET(*(sd_connect+i), &readset)) {
                    // Поступили данные от клиента, читаем их
                    //bytes_read = recv(*(sd_connect+i), &get, sizeof(get), 0);
                    //cout<<"("<<bytes_read<<" bite; "<<") ID: "<<sd_connect[0]<<": "<<get.id<<' '<<get.x<< ' ' <<get.y<<endl;
                    new_message=true;
                    if (bytes_read <= 0) {
                        // Соединение разорвано, удаляем сокет из множества
                        close(*(sd_connect+i));
                        *(sd_connect+i)=0;
                        // FD_CLR
                        continue;
                    }
                }
            }
        }
    }

    void Get_Messages(){
        person get;
        for (int i = 0; i < all_clients; i++) {
            //std::cout<<*(sd_connect+i)<<std::endl;
            if(*(sd_connect+i)!=0){
                if (FD_ISSET(*(sd_connect+i), &readset)) {
                    // Поступили данные от клиента, читаем их
                    int bytes_read = recv(*(sd_connect+i), &get, sizeof(get), 0);
                    cout<<"("<<bytes_read<<" bite; "<<") ID: "<<sd_connect[0]<<": "<<get.id<<' '<<get.x<< ' ' <<get.y<<endl;
                    //new_message=true;
                    if (bytes_read <= 0) {
                        // Соединение разорвано, удаляем сокет из множества
                        close(*(sd_connect+i));
                        *(sd_connect+i)=0;
                        // FD_CLR
                        continue;
                    }
                }
            }
        }
    }

    void Get_Message_Only(int connect_ID){
        person get;
        if((connect_ID)!=0){
            if (FD_ISSET((connect_ID), &readset)) {
                int bytes_read = recv((connect_ID), &get, sizeof(get), 0);
                cout<<"("<<bytes_read<<" bite; "<<") ID: "<<connect_ID<<": "<<get.id<<' '<<get.x<< ' ' <<get.y<<endl;
            }  
        }
    }
    
};
