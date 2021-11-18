#include <iostream>
#include <string.h>
#include "Clien.h"

struct person{
    int id;
    int x,y;
};

int main(int argc, char *argv[]) {
    Clien sock(11113); // Создать подключение с 2 клиентами
    
    bool new_m = false;

    while (1){
        /*
        sock.Chek_New_Mesage_On_Sockets(new_m);
        if (new_m==true){
            std::cout<<"Ok"<<std::endl;
            sock.Get_Messages();
        } else {
            std::cout<<"No"<<std::endl;
        }
        */
        sock.Send_For_Server();
        sleep(1);
    }
    
	return 0;
}
