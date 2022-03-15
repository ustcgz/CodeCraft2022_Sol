#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <map>
#include <unordered_map>
static std::string CONFIG_PATH="/data/config.ini";
static std::string DATA_PATH="/data/"; 
static std::string OUTPUT_PATH="/output/solution.txt";
const static int MAX_CLIENT = 36;// 最大的客户节点
const static int MAX_SITE = 135;//最大的边缘节点
int clientArrLen;
int siteArrLen;
int QOS_LIMIT; // 最大延迟

// 主需要获取每个时间点所有的需求序列及其通过索引能够确定相应的客户节点的名字即可。
std::vector<std::string> timeSeqName;//时间序列的名字
std::vector<std::vector<int> > demand;//每个时间节点的贷款需求序列
/**
 * Site,即任务书中的边缘节点
 */
struct SiteNode{
    std::string siteName;
    int bandwidth;//该Site可分配的最大上限，不超过1000000MB
    int curAlloc;//当前该节点分配到流量。
    bool ifAllocSat(int cnt)// 是否满足分配cnt流量
    {
        return curAlloc + cnt <= bandwidth;
    }
} siteArr[MAX_SITE];
/**
 * 
 * 客户节点
 */
struct ClientNode
{
    std::string clientName;
    std::vector<std::pair<int, int>> alloc;//每个时间点site节点分配的索引及流量
} clientArr[MAX_CLIENT];

// Qos Table
// struct QosTable
// {
//     std::unordered_map<std::string,int> site;//网站对应的索引
//     std::unordered_map<std::string, int> client;//客户节点对应的索引
//     std::unordered_map<std::pair<int, int>,int> qos;//每个网站及其索引对应的延迟
//     /**
//      * 查找site name对应的索引
//      */
//     int findSiteIndex(const std::string& siteName)
//     {
//         return site.find(siteName);
//     }
//     /**
//      * 查找client对应的索引
//      */
//     inline int findClientIndex(const std::string& clientName)
//     {
//         return client.find(clientName);
//     }
//     /**
//      * 根据索引查找Qos
//      */
//     inline int findQos(int siteIndex, int clientIndex)
//     {
//         return qos.find({siteIndex, clientIndex});
//     }
//     /**
//      * 添加新的site索引对
//      */
//     void addSite(const std::string& siteName,int index)
//     {
//         site[siteName]=index;
//     }
//     /**
//      * 添加新的client索引对
//      */
//     void addClient(const std::string& clientName,int index)
//     {
//         client[clientName] = index;
//     }
//     /**
//      * 判断该qos是否满足延迟要求
//      */
//     inline bool ifQosSat(int qos)
//     {
//         return qos < QOS_LIMIT;
//     }
// };
struct QosTable
{
    // 初始化每个连接关系都为最大值
    int qosData[MAX_SITE][MAX_CLIENT];
    inline bool ifQosSat(int siteIndex, int clientIndex)
    {
        return qosData[siteIndex][clientIndex]<QOS_LIMIT;
    }
}qosTable;

/**
 * String split by delimiters
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
/**
 * 返回一行的数字vector
 */
static std::vector<int> split2(const std::string& s, const std::string &delimiters = ",")
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
    return tokens;
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
    config.close();
}
/**
 * read site_bandwidth.csv, qos.csv, demand.csv
 */
void readData(){
    std::vector<std::string> tmp_vec;
    std::vector<int> tmp_vec2;
    std::ifstream data;
    std::string tmp_line;
    unsigned int index = 0;
    //初始化边缘节点列表以及每个边缘节点的带宽上限
    data.open(DATA_PATH+"site_bandwidth.csv");
    getline(data,tmp_line);
    // std::cout << tmp_line << std::endl;
    while(getline(data,tmp_line)){
        tmp_vec = split(tmp_line, ",");
        siteArr[index].siteName = tmp_vec[0];
        siteArr[index].bandwidth = atoi(tmp_vec[1].c_str());
        siteArr[index].curAlloc = 0;
        index++;
    }
    siteArrLen = index;
    // for (int i = 0; i < siteArrLen;++i)
    // {
    //     std::cout << i << " " << siteArr[i].siteName << " " << siteArr[i].bandwidth << " " << siteArr[i].curAlloc << std::endl;
    // }
    data.close();
    data.clear();

    //客户节点和边缘节点的Qos
    data.open(DATA_PATH + "qos.csv");
    getline(data, tmp_line);
    // std::cout << tmp_line << std::endl;
    tmp_vec = split(tmp_line, ",");
    for (index = 0;index < tmp_vec.size()-1;index++)
    {
        clientArr[index].clientName = tmp_vec[index + 1];
    }
    clientArrLen = index;
    // 处理每个节点的Qos
    index = 0;
    while (getline(data, tmp_line))
    {
        // std::cout << index << " ";
        tmp_vec2 = std::move(split2(tmp_line, ","));
        for (unsigned int k=0; k < tmp_vec2.size();++k)
        {
            qosTable.qosData[index][k] = tmp_vec2[k];
            // std::cout << qosTable.qosData[index][k] << " ";
        }
        // std::cout << std::endl;
        index++;
    }
    data.close();
    data.clear();

    //客户节点在不同时刻的带宽需求信息
    data.open(DATA_PATH+"demand.csv");
    getline(data, tmp_line);
    // std::cout << tmp_line << std::endl;
    index = 0;
    while(getline(data,tmp_line)){
        timeSeqName.push_back(tmp_line.substr(0,tmp_line.find_first_of(",")));
        demand.push_back(split2(tmp_line, ","));
    }
    // for (int i = 0; i < timeSeqName.size();++i)
    // {
    //     std::cout << i << " " << timeSeqName[i] << ":";
    //     for (int j = 0; j < demand[i].size();++j)
    //         std::cout << demand[i][j] << " ";
    //     std::cout << "\n";
    // }
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
    // testIO();
    return 0;
}
