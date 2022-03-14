#include <iostream>
#include <fstream>
#include <vector>
#include <string>

static std::string CONFIG_PATH="./data/config.ini";
static std::string DATA_PATH="./data/"; 
static std::string OUTPUT_PATH="./output/solution.txt";

//Tool
static std::vector<std::string> split(const std::string &s, const std::string &delimiters = ",") {
    std::vector<std::string> tokens;
    std::size_t lastPos = s.find_first_not_of(delimiters,0);
    std::size_t pos = s.find_first_of(delimiters,lastPos);
    while (pos != std::string::npos || lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
    return tokens;
}

// Data input&output
void readConf(){
    std::ifstream config;
    config.open(CONFIG_PATH);
    std::string tmp_line;
    getline(config,tmp_line);
    getline(config,tmp_line);
    // TODO
    config.close();
}

void readData(){
    std::ifstream data;
    data.open(DATA_PATH+"qos.csv");
    std::string tmp_line;
    while(getline(data,tmp_line)){
        //TODO

    }
    data.close();
    data.clear();
    data.open(DATA_PATH+"demand.csv");
    while(getline(data,tmp_line)){
        //TODO

    }
    data.close();
    data.clear();
    data.open(DATA_PATH+"site_bandwidth.csv");
    while(getline(data,tmp_line)){
        //TODO

    }
    data.close();
    data.clear();
}

void Output(){
    // TODO 

}



int main() {
    

	return 0;
}
