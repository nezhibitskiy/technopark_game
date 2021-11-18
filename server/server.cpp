#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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

#include "Serv.h"

using namespace std;

struct person{
    int id;
    int x,y;
};


//Server side
int main(int argc, char *argv[]) {
    //for the server, we only need to specify a port number
    if(argc != 2) {
        cerr << "Usage: port" << endl;
        exit(0);
    }
    //grab the port number
    int port = atoi(argv[1]);
    //buffer to send and receive messages with
    char msg[1500];
    
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0) {
        cout << "Error establishing the server socket" << endl;
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0) {
        cout << "Error binding socket to local address" << endl;
        exit(0);
    }
    cout << "Waiting for a client to connect..." << endl;
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int sum_client=0,vspom=0;
    //int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    int sd_connect[10]={0};
    int all_clients=2;
    cout << "> clients? :"; //cin>> all_clients;
    // Регистрация клиентов
    
    // select
    fd_set readset;
    // Задаём таймаут
    timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    cout << endl;
    for(int i = 0; i < all_clients; i++){
        sd_connect[i] = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
        if(sd_connect[i] < 0) {
            cout << "! Error accepting request from client "<<i+1<<". ID: "<<sd_connect[i] << endl;
        }else{
            cout << "Connected with client "<<i+1<<". ID: "<<sd_connect[i] << endl;
        }
        FD_ZERO(&readset);
        FD_SET(sd_connect[i], &readset);
        vspom=sd_connect[i];
        send(sd_connect[i], &vspom, sizeof(vspom), 0);
    }

    /*cout<<"Clients: "<<endl;
    for(int i = 0; i < all_clients;i++)
        cout<<"ID: "<<sd_connect[i]<<endl;*/
    

    cout << "work!!!"<<endl;
    //lets keep track of the session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;

    string data;
    person a1 ={1,2,3},get;

    int bytes_read, chet=0,retval;           
    // Тестовая отправка по клиентам
    while(1) {
        
        for(int i = 0; i < all_clients;i++){
            send(sd_connect[i], &a1, sizeof(a1), 0);
            std::cout<<"Sending to "<< sd_connect[i]<<endl;
            //sleep(1);
        }

        sleep(2);
        /*
        for(int i = 0; i < all_clients;i++){ // Тупое опрашиваниеы
            bytes_read = recv(sd_connect[i], &get, sizeof(get), 0);
            cout<<"("<<bytes_read<<" bite; "<<chet<<") ID: "<<sd_connect[i]<<": "<<get.id<<' '<<get.x<< ' ' <<get.y<<endl;
        }*/
        
        retval = select(all_clients, &readset, NULL, NULL, &timeout);
        /* Don't rely on the value of tv now! */
        cout<<retval<<endl;

        /* // Использование select
        for(int i = 0; i < all_clients;i++){
            if (FD_ISSET(*(sd_connect+i), &readset)) {
                bytes_read = recv(sd_connect[i], &get, sizeof(get), 0);
                cout<<"("<<bytes_read<<" bite; "<<chet<<") ID: "<<sd_connect[i]<<": "<<get.id<<' '<<get.x<< ' ' <<get.y<<endl;
            }
        }*/
        
        chet++;
    }
    //we need to close the socket descriptors after we're all done
    gettimeofday(&end1, NULL);
    
    for(int i = 0; i<all_clients;i++){
        close(sd_connect[i]);
    }
    close(serverSd);
    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) 
        << " secs" << endl;
    cout << "Connection closed..." << endl;
    return 0;   
}
