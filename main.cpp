#include "client.h"
#include "server.h"
#include <thread>



//int windowWidth, windowHeight;

//void create_map_file(const std::string& filename, sf::RenderWindow& window){
//    int numOfBlocksW = 1 + window.getSize().x / Map::BlockSize;
//    int numOfBlcoksH = 1 + window.getSize().y / Map::BlockSize;
//    std::cout << "creating file " << numOfBlocksW << ' ' << numOfBlcoksH << std::endl;
//    std::ofstream file(filename, std::ios_base::out);
//    file << numOfBlocksW << ' ' << numOfBlcoksH << "\n";
//    for(int row = 0; row < numOfBlcoksH; row++){
//        file << 0;
//        for(int col = 1; col < numOfBlocksW; col++){
//            file << ' ' << 0;
//        }
//        file << "\n";
//    }
//}  

int main(){

    ClientGame clg;
    std::cout << "client running\n";    
    clg.runGame();

}