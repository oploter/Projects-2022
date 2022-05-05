#include "../include/client.h"
#include "../include/server.h"
#include <thread>


int main(){

    client::ClientGame client_game;
    std::cout << "client running\n";    
    client_game.runGame();

}