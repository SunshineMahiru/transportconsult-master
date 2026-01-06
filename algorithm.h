#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "graph.h"
#include <vector>


// 路径查询结果
struct QueryResult {
    bool success;
    std::vector<std::string> path;          // 城市序列
    std::vector<Transport> transports;      // 乘坐的班次
    float totalCost;                        // 总费用或总时间
    
    void print() const;
};
// 路径查找器
class PathFinder {
private:
    const TransportGraph& graph;
    const int MIN_TRANSFER_TIME = 30; // 最小中转时间（分钟）
    
公共:
    PathFinder(const TransportGraph& g) : graph(g) {}
    QueryResult findCheapest(const std::string& from, const std::string& to);// 最省钱路径
    QueryResult findFastest(const std::string& from, const std::string& to); // 最快到达路径
    QueryResult findBalanced(const std::string& from, const std::string& to, // 多标准平衡路径（加权和）
                            float timeWeight, float costWeight);
private:
    std::string minutesToTime(int mins) const; // 分钟数转换为时间字符串
    const Transport* findEarliestTransport(const std::string& fromCity, // 寻找下一班可用交通工具
                                          int arrivalTime, 
                                          const std::string& toCity) const;
};

#endif
