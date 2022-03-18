#include "client.h"
#include "server.h"
#include <thread>


int main(){

    ClientGame clg;
    std::cout << "client running\n";    
    clg.runGame();

}