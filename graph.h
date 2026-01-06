#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

// 交通班次（图的边）
struct Transport {
    std::string id;           // 车次/航班号，如 "G101"
    std::string type;         // "train" 或 "flight"
    std::string from;         // 出发城市
    std::string to;           // 到达城市
    int departure;            // 出发时间（从0:00开始的分钟数，如540=9:00）
    int arrival;              // 到达时间（分钟数）
    float price;              // 票价
    
    // 计算行程时间（分钟）
    int duration() const { return arrival - departure; }
};

// 城市节点（图的顶点）
struct CityNode {
    std::string name;
    std::vector<Transport> outgoing; // 从该城市出发的班次列表
};

// 交通图类
class TransportGraph {
private:
    std::unordered_map<std::string, CityNode> cities;
    
public:
    // 城市管理
    bool addCity(const std::string& name);
    bool removeCity(const std::string& name);
    
    // 班次管理
    bool addTransport(const Transport& t);
    bool removeTransport(const std::string& id);
    
    // 查询接口
    const CityNode* getCity(const std::string& name) const;
    const std::vector<Transport>* getTransportsFrom(const std::string& city) const;
    std::vector<std::string> getAllCityNames() const;
    
    // 数据持久化
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    
    // 工具函数
    void printGraph() const;
};

#endif