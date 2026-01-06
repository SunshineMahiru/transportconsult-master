#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

bool TransportGraph::addCity(const string& name) {
    if (cities.find(name) != cities.end()) return false;
    cities[name] = CityNode{name, {}};
    return true;
}

bool TransportGraph::removeCity(const string& name) {
    return cities.erase(name) > 0;
}

bool TransportGraph::addTransport(const Transport& t) {
    if (cities.find(t.from) == cities.end() || cities.find(t.to) == cities.end()) {
        return false;
    }
    cities[t.from].outgoing.push_back(t);
    return true;
}

bool TransportGraph::removeTransport(const string& id) {
    for (auto& cityPair : cities) {
        auto& transports = cityPair.second.outgoing;
        auto it = find_if(transports.begin(), transports.end(),
                         [&id](const Transport& t) { return t.id == id; });
        if (it != transports.end()) {
            transports.erase(it);
            return true;
        }
    }
    return false;
}

const CityNode* TransportGraph::getCity(const string& name) const {
    auto it = cities.find(name);
    return it != cities.end() ? &(it->second) : nullptr;
}
=
const vector<Transport>* TransportGraph::getTransportsFrom(const string& city) const {
    auto it = cities.find(city);
    if (it != cities.end()) {
        return &(it->second.outgoing);
    }
    return nullptr;
}

vector<string> TransportGraph::getAllCityNames() const {
    vector<string> names;
    for (const auto& pair : cities) {
        names.push_back(pair.first);
    }
    return names;
}

bool TransportGraph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string type, id, from, to;
        int dep_h, dep_m, arr_h, arr_m;
        float price;

        if (iss >> type >> id >> from >> to >> dep_h >> dep_m >> arr_h >> arr_m >> price) {
            Transport t;
            t.type = type;
            t.id = id;
            t.from = from;
            t.to = to;
            t.departure = dep_h * 60 + dep_m;
            t.arrival = arr_h * 60 + arr_m;
            t.price = price;

            addCity(from);
            addCity(to);
            addTransport(t);
        }
    }
    file.close();
    return true;
}

bool TransportGraph::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& cityPair : cities) {
        for (const auto& trans : cityPair.second.outgoing) {
            file << trans.type << " "
                 << trans.id << " "
                 << trans.from << " "
                 << trans.to << " "
                 << trans.departure/60 << " " << trans.departure%60 << " "
                 << trans.arrival/60 << " " << trans.arrival%60 << " "
                 << trans.price << endl;
        }
    }
    file.close();
    return true;
}

void TransportGraph::printGraph() const {
    cout << "========== 交通网络图 ==========" << endl;
    if (cities.empty()) {
        cout << "（图为空）" << endl;
        return;
    }

    for (const auto& pair : cities) {
        const CityNode& node = pair.second;
        cout << "城市 [" << node.name << "] 出发班次：" << endl;

        if (node.outgoing.empty()) {
            cout << "  （无出发班次）" << endl;
        } else {
            for (const auto& trans : node.outgoing) {
                cout << "  -> " << trans.to 
                     << " 乘坐 " << trans.type << " " << trans.id
                     << " 出发:" << trans.departure/60 << ":" 
                     << (trans.departure%60 < 10 ? "0" : "") << trans.departure%60
                     << " 到达:" << trans.arrival/60 << ":" 
                     << (trans.arrival%60 < 10 ? "0" : "") << trans.arrival%60
                     << " 票价:" << trans.price << "元" << endl;
            }
        }
        cout << endl;
    }
    cout << "=================================" << endl;

}
