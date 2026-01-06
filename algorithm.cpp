#include "algorithm.h"
#include <queue>
#include <limits>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

//Djstra算法实现部分，以求得最省钱选择
QueryResult PathFinder::findCheapest(const string& from, const string& to) {
    QueryResult result;
    result.success = false;
    
    // 验证城市存在
    if (!graph.getCity(from) || !graph.getCity(to)) {
        return result;
    }
    
    // Dijkstra算法数据结构
    unordered_map<string, float> dist;      // 最短距离
    unordered_map<string, string> prevCity; // 前驱城市
    unordered_map<string, Transport> prevTrans; // 前驱班次
    
    // 优先队列（最小堆）
    using Node = pair<float, string>;
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    
    // 初始化
    for (const auto& cityName : graph.getAllCityNames()) {
        dist[cityName] = numeric_limits<float>::max();
    }
    dist[from] = 0.0f;
    pq.push({0.0f, from});
    
    // 主循环
    while (!pq.empty()) {
        auto [currentDist, currentCity] = pq.top();
        pq.pop();
        
        // 找到目标，提前结束
        if (currentCity == to) break;
        
        // 距离不是最新的，跳过
        if (currentDist > dist[currentCity]) continue;
        
        // 遍历所有从当前城市出发的班次
        const auto* transports = graph.getTransportsFrom(currentCity);
        if (!transports) continue;
        
        for (const auto& trans : *transports) {
            float newDist = dist[currentCity] + trans.price;
            
            if (newDist < dist[trans.to]) {
                dist[trans.to] = newDist;
                prevCity[trans.to] = currentCity;
                prevTrans[trans.to] = trans;
                pq.push({newDist, trans.to});
            }
        }
    }
    
    // 如果找到路径，回溯构建结果
    if (dist[to] < numeric_limits<float>::max()) {
        result.success = true;
        result.totalCost = dist[to];
        
        // 回溯路径
        string current = to;
        while (current != from) {
            result.path.insert(result.path.begin(), current);
            result.transports.insert(result.transports.begin(), prevTrans[current]);
            current = prevCity[current];
        }
        result.path.insert(result.path.begin(), from);
    }
    
    return result;
}

// Djstra算法实现部分，以求得最快选择
QueryResult PathFinder::findFastest(const string& from, const string& to) {
    QueryResult result;
    result.success = false;
    
    if (!graph.getCity(from) || !graph.getCity(to)) {
        return result;
    }
    
    // 记录到达每个城市的最早时间
    unordered_map<string, int> arrivalTime;
    unordered_map<string, string> prevCity;
    unordered_map<string, Transport> prevTrans;
    
    // 优先队列按到达时间排序
    using Node = pair<int, string>; // <到达时间, 城市>
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    
    // 初始化
    for (const auto& cityName : graph.getAllCityNames()) {
        arrivalTime[cityName] = numeric_limits<int>::max();
    }
    
    // 假设从起始城市在0分钟（午夜）开始，实际可根据需求调整
    const int START_TIME = 8 * 60; // 8:00 AM开始旅程
    arrivalTime[from] = START_TIME;
    pq.push({START_TIME, from});
    
    // 主循环
    while (!pq.empty()) {
        auto [currentTime, currentCity] = pq.top();
        pq.pop();
        
        // 优化：如果当前时间已经不如已知最优时间，跳过
        if (currentTime > arrivalTime[currentCity]) continue;
        
        // 找到目标
        if (currentCity == to) break;
        
        // 遍历所有班次
        const auto* transports = graph.getTransportsFrom(currentCity);
        if (!transports) continue;
        
        for (const auto& trans : *transports) {
            // 计算可行出发时间
            // 必须在中转等待后才能乘坐下一班
            int earliestDeparture = currentTime + MIN_TRANSFER_TIME;
            
            // 如果这班车已经开走了，就跳过它
            if (trans.departure < earliestDeparture) {
                continue;
            }
            
            // 计算总时间：等待时间 + 行程时间
            int waitTime = trans.departure - currentTime;
            int travelTime = trans.duration();
            int newArrivalTime = trans.arrival;
            
            // 如果找到更早到达的方式，更新
            if (newArrivalTime < arrivalTime[trans.to]) {
                arrivalTime[trans.to] = newArrivalTime;
                prevCity[trans.to] = currentCity;
                prevTrans[trans.to] = trans;
                pq.push({newArrivalTime, trans.to});
            }
        }
    }
    
    // 构建结果
    if (arrivalTime[to] < numeric_limits<int>::max()) {
        result.success = true;
        result.totalCost = arrivalTime[to] - START_TIME; // 总耗时（分钟）
        
        // 回溯路径
        string current = to;
        while (current != from) {
            result.path.insert(result.path.begin(), current);
            result.transports.insert(result.transports.begin(), prevTrans[current]);
            current = prevCity[current];
        }
        result.path.insert(result.path.begin(), from);
    }
    
    return result;
}

// 综合算法：求得最快和最省钱的路径
QueryResult PathFinder::findBalanced(const string& from, const string& to, 
                                    float timeWeight, float costWeight) {
    // 简化的实现：分别计算最快和最省钱，然后加权平均
    // 如果要实现更复杂的，需要修改Dijkstra的成本函数
    auto fastest = findFastest(from, to);
    auto cheapest = findCheapest(from, to);
    
    QueryResult result;
    result.success = fastest.success && cheapest.success;
    
    if (result.success) {
        // 选择加权得分更优的路径
        float fastestScore = fastest.totalCost * timeWeight;
        float cheapestScore = cheapest.totalCost * costWeight;
        
        if (fastestScore <= cheapestScore) {
            return fastest;
        } else {
            return cheapest;
        }
    }
    
    return result;
}

// 分钟数转时间字符串qwq
string PathFinder::minutesToTime(int mins) const {
    int hours = mins / 60;
    int minutes = mins % 60;
    
    stringstream ss;
    ss << setw(2) << setfill('0') << hours << ":" 
       << setw(2) << setfill('0') << minutes;
    return ss.str();
}

// 打印查询结果
void QueryResult::print() const {
    if (!success) {
        cout << "未找到可行路径！" << endl;
        return;
    }
    
    cout << "\n========== 查询结果 ==========" << endl;
    cout << "总成本: " << totalCost 
         << (totalCost > 100 ? " 分钟" : " 元") << endl;
    cout << "路径详情：" << endl;
    
    int currentTime = 8 * 60; // 假设从8:00开始
    
    for (size_t i = 0; i < transports.size(); i++) {
        const auto& trans = transports[i];
        
        // 计算等待时间（如果是第一段，则等待时间为0或从START_TIME开始）
        int waitTime = 0;
        if (i == 0) {
            waitTime = max(0, trans.departure - currentTime);
        } else {
            // 从前一段的到达时间开始计算
            int prevArrival = transports[i-1].arrival;
            waitTime = max(0, trans.departure - prevArrival - 30); // 30分钟最小中转
        }
        
        cout << i+1 << ". ";
        if (waitTime > 0) {
            cout << "等待 " << waitTime << " 分钟，";
        }
        cout << trans.departure/60 << ":" 
             << setw(2) << setfill('0') << trans.departure%60
             << " 乘坐 " << trans.type << " " << trans.id
             << " 从 " << path[i] << " 到 " << path[i+1]
             << " (到达: " << trans.arrival/60 << ":" 
             << setw(2) << setfill('0') << trans.arrival%60 << ")"
             << " 票价: " << trans.price << " 元" << endl;
    }
    cout << "=============================\n" << endl;

}
