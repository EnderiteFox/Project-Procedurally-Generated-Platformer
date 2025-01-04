#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

std::pair<std::string,std::map<std::string,std::string>> parseLine(std::string line){
    std::size_t i = line.find("<")+1;
    size_t size = line.size();
    std::string tagName;
    std::map<std::string,std::string> elements;
    // Tag
    while(line[i] != '>' && line[i] != ' '){
        tagName += line[i];
        i++;
    }
    if(line[i] == '>'){
        return {tagName,elements};
    }
    i++;
    // Rest of the elements
    while(line[i] != '>' && i < size){
        while(line[i]==' ') i++;
        std::string tag;
        std::string value;
        while (line[i] != '='){
            tag += line[i];
            i++;
        }
        i+=2;
        while(line[i] != '"'){
            value += line[i];
            i++;
        }
        elements[tag] = value;
        i++;
    }
    return {tagName,elements};
}


int main(){
    std::ifstream file("tiles.xml");
    for(std::string line; std::getline(file, line); )
    {
        if(line.find("<!--") == 0) continue;


        std::cout << line << std::endl;
        auto[tagName,tags] = parseLine(line);

        if (tagName[0] == '/') continue;

        std::cout << "\t" << tagName << std::endl;
        for(const auto&[tag,value]:tags){
            std::cout << "\t" << tag << " : " << value << std::endl;
        }
    }

    return 0;
}
