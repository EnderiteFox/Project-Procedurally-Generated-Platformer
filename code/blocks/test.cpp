#include <vector>
#include <fstream>
#include <iostream>

int main(){
    std::ifstream file("tiles.xml");
    for(std::string line; std::getline(file, line); )
    {
        std::cout << line << std::endl;
    }

    return 0;
}
