#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <map>
#include <unordered_map>
static std::string CONFIG_PATH="../data/config.ini";
static std::string DATA_PATH="../data/"; 
static std::string OUTPUT_PATH="./output/solution.txt";
int QOS_LIMIT; // 最大延迟

// 主需要获取每个时间点所有的需求序列及其通过索引能够确定相应的客户节点的名字即可。
std::vector<std::string> demandName;//需求节点的名字，可以查找第i个需求的名字
std::map<std::string, std::vector<int> > demand;//每个时间节点的贷款需求序列


// Qos Table
struct QosTable
{
    std::unordered_map<std::string,int> site;//网站对应的索引
    std::unordered_map<std::string, int> client;//客户节点对应的索引
    std::unordered_map<int, int> qos;//每个网站及其索引对应的延迟
    /**
     * 查找site name对应的索引
     */
    int findSiteIndex(const std::string& siteName)
    {
        // TODO
    }
    /**
     * 查找client对应的索引
     */
    inline int findClientIndex(const std::string& clientName)
    {
        // TODO
    }
    /**
     * 根据索引查找Qos
     */
    inline int findQos(int siteIndex, int clientIndex)
    {
        // TODO
    }
    /**
     * 添加新的site索引对
     */
    void addSite(const std::string& siteName,int index)
    {
        // TODO
    }
    /**
     * 添加新的client索引对
     */
    void addClient(const std::string& siteName,int index)
    {
        // TODO
    }
    /**
     * 判断该qos是否满足延迟要求
     */
    inline bool ifSatQos(int qos)
    {
        // TODO
    }
};

/**
 * String splict by delimiters
 */
static std::vector<std::string> split(const std::string &s, const std::string &delimiters = ",")
{
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
static std::pair<std::string,std::vector<int> > split2(const std::string& s, const std::string &delimiters = ",")
{
    std::string str;
    std::vector<int> tokens;
    std::size_t lastPos = s.find_first_not_of(delimiters,0);//查找第一个非,
    std::size_t pos = s.find_first_of(delimiters,lastPos);//查找第一个,
    str = s.substr(lastPos, pos - lastPos);
    lastPos = s.find_first_not_of(delimiters, pos);
    pos = s.find_first_of(delimiters, lastPos);
    while (pos != std::string::npos || lastPos != std::string::npos) {
        tokens.emplace_back(atoi(s.substr(lastPos, pos - lastPos).c_str()));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
    return {str,tokens};
}
/**
 * read Config
 */
void readConf(){
    std::ifstream config;
    config.open(CONFIG_PATH);
    std::string tmp_line;
    getline(config,tmp_line);
    getline(config,tmp_line);
    // std::cout << tmp_line << std::endl;
    QOS_LIMIT = atoi(std::string(tmp_line.begin() + tmp_line.find('=')+1, tmp_line.end()).c_str());
    // std::cout << QOS_LIMIT << std::endl;
    // TODO
    config.close();
}
/**
 * read Data
 */
void readData(){
    std::vector<std::string> tmp_vec;
    std::ifstream data;
    data.open(DATA_PATH+"qos.csv");//客户节点和边缘节点的Qos
    std::string tmp_line;
    
    while (getline(data, tmp_line)){
        // TODO
    }
    data.close();
    data.clear();
    data.open(DATA_PATH+"demand.csv");//客户节点在不同时刻的带宽需求信息
    getline(data, tmp_line);
    tmp_vec = split(tmp_line, ",");
    demandName = std::vector<std::string>(std::make_move_iterator(tmp_vec.begin() + 1), std::make_move_iterator(tmp_vec.end()));
    // for(auto &str:demandName)
    //     // std::cout << str << "\n";
    while(getline(data,tmp_line)){
        demand.insert(split2(tmp_line, ","));
    }
    // for(auto &p:demand)
    // {
    //     std::cout << p.first << ":";
    //     for(auto &num:p.second)
    //     {
    //         std::cout << num << ",";
    //     }
    //     std::cout << "\n";
    // }

    data.close();
    data.clear();
    data.open(DATA_PATH+"site_bandwidth.csv");//边缘节点列表以及每个边缘节点的带宽上限
    while(getline(data,tmp_line)){
        //TODO

    }
    data.close();
    data.clear();
}

void Output(){
    // TODO 

}
/**
 * 
 */
void testIO()
{
    readConf();
    readData();
}

int main() {
    testIO();

    return 0;
}
