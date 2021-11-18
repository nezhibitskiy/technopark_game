#include <iostream>
#include <string.h>
#include "Serv.h"

struct person{
    int id;
    int x,y;
};

int main(int argc, char *argv[]) {
    Serv sock(2); // Создать подключение с 2 клиентами
    std::cout<<sock.get_all_clients()<<std::endl;
    bool new_m;

    while (1){
        sock.Chek_New_Mesage_On_Sockets(new_m);
        if (new_m==true){
            std::cout<<"Ok"<<std::endl;
            sock.Get_Messages();
        } else {
            std::cout<<"No"<<std::endl;
        }
        sleep(1);
    }
    
        

    

	return 0;
}
