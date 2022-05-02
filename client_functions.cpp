#include "client_functions.h"
#include "declarations.h"
#include "server.h"
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
namespace client{
int Client_Server_Conn::init(bool single_player){
    std::cout << "init " << single_player << " iitn\n";
    if(!single_player){
        std::ifstream port_file("ports.txt", std::ios_base::out | std::ios_base::app);
        std::cout << "==Ports==\n";
        std::string line;
        std::getline(port_file, line);
        std::stringstream ss(line);
        std::vector<int> available_ports;
        int new_port;
        while(ss >> new_port){
            available_ports.push_back(new_port);
            std::cout << new_port << std::endl;
            ss.ignore(1);
        }
        std::cout << "\n====\nChoose one: ";
        std::cin >> port;
        while(std::count(available_ports.begin(), available_ports.end(), port) <= 0){
            std::cout << "Choose one: ";
            std::cin >> port;
        }
    }else{
        port = 12345 + std::rand() % 12345;
        std::ofstream port_file("ports.txt", std::ios_base::out | std::ios_base::app);
        port_file << port << ";";
    }
    return port;
}
void Client_Server_Conn::receiveMessages(){
    while(1){
        sf::Packet new_packet; 
        if(server_socket.receive(new_packet) != sf::Socket::Done){
            return;
        }
        Q.push(std::move(new_packet));
    }
}
void Client_Server_Conn::endConnection(){
    std::string upd_available_ports;
    std::ifstream ports_in("ports.txt", std::ios_base::in);
    std::string av_ports;
    std::getline(ports_in, av_ports);
    ports_in.close();
    std::stringstream iss(av_ports);
    std::stringstream oss;
    int n_port;
    while(iss >> n_port){
        if(n_port != port){
            oss << n_port << ';';
        }
        iss.ignore(1);
    }  
    std::ofstream ports_out("ports.txt", std::ios_base::out);
    std::string res = oss.str();
    ports_out << res;
}
int Client_Server_Conn::run(bool single_player){
    if(server_socket.connect(ip, port) != sf::Socket::Done){
        std::cout << "cant connect to " << port << "\n";
        return -1;
    }
    std::thread receive_thread([this](){
        receiveMessages();
    });
    receive_thread.detach();
    if(!single_player){
        sf::Packet msg_to_send;
        msg_to_send << 2;
        send(std::move(msg_to_send));
        int type;
        sf::Packet msg;
        if(!getMessage(type, msg) || type != 2){
            std::cout << "error\n";
        }
        msg >> player_id;
    }
    return player_id;
}
sf::Packet Client_Server_Conn::getPacket(int msg_type){
    sf::Packet new_msg;
    new_msg << msg_type << player_id;
    return std::move(new_msg);
}
bool Client_Server_Conn::send(sf::Packet&& packet){
    if(server_socket.send(packet) != sf::Socket::Done){
        std::cout << "could nto send\n";
        return false;
    }
    return true;
}
bool Client_Server_Conn::getMessage(int& msg_type, sf::Packet& message){
    if(Q.empty()){
        return false;
    }
    message = std::move(Q.pop());
    message >> msg_type;
    return true;
}
void Client_Server_Conn::try_connect(){
    if(server_socket.connect(ip, port) != sf::Socket::Done){
        std::cout << "CONN FAIL " << ip.toString() << ' ' << port << "\n";
        return;
    }
    std::cout << "CONN SUCCESS " << ip.toString() << ' ' << port << "\n";
}
}