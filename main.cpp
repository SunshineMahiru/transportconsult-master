//主程序，后期可选gui交互功能，先做控制台
#include "graph.h"
#include "algorithm.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
using namespace std;

void printMenu() {
    cout << "\n========== 全国交通咨询系统 ==========" << endl;
    cout << "欢迎使用本交通咨询系统，请选择您的项目：" << endl;
    cout << "1. 查看交通网络图" << endl;
    cout << "2. 查询最省钱路径" << endl;
    cout << "3. 查询最快到达路径" << endl;
    cout << "4. 多标准平衡查询" << endl;
    cout << "5. 重新加载数据" << endl;
    cout << "6. 管理城市" << endl;
    cout << "7. 管理交通班次" << endl;
    cout << "0. 退出系统" << endl;
    cout << "=====================================" << endl;
    cout << "请选择操作 (0-7): ";
}



void loadData(TransportGraph& graph) {
    cout << "加载数据文件中..." << endl;
    if (graph.loadFromFile("data.txt")) {
        cout << "数据加载成功！" << endl;
        graph.printGraph();
    } else {
        cout << "警告：无法加载数据文件，使用示例数据..." << endl;
        
        // 添加示例数据
        graph.addCity("北京");
        graph.addCity("上海");
        graph.addCity("广州");
        graph.addCity("深圳");
        
        // 添加示例班次
        graph.addTransport({"G101", "train", "北京", "上海", 480, 900, 553.0});   // 8:00-15:00
        graph.addTransport({"MU5101", "flight", "北京", "上海", 540, 690, 1010.0}); // 9:00-11:30
        graph.addTransport({"G102", "train", "上海", "广州", 600, 1080, 793.0});    // 10:00-18:00
        graph.addTransport({"CZ3101", "flight", "广州", "深圳", 780, 840, 680.0});   // 13:00-14:00
        graph.addTransport({"G103", "train", "北京", "广州", 600, 1260, 943.0});    // 10:00-21:00
    }
}

void manageCities(TransportGraph& graph) {
    int choice;
    do {
        cout << "\n----- 城市管理 -----" << endl;
        cout << "1. 添加城市" << endl;
        cout << "2. 删除城市" << endl;
        cout << "3. 返回主菜单" << endl;
        cout << "请选择: ";
        cin >> choice;
        cin.ignore(); // 清除输入缓冲区中的换行符

        if (choice == 1) {
            string name;
            cout << "请输入要添加的城市名称: ";
            getline(cin, name);
            if (graph.addCity(name)) {
                cout << "城市 '" << name << "' 添加成功！" << endl;
            } else {
                cout << "添加失败，城市可能已存在。" << endl;
            }
        } 
        else if (choice == 2) {
            string name;
            cout << "请输入要删除的城市名称: ";
            getline(cin, name);
            if (graph.removeCity(name)) {
                cout << "城市 '" << name << "' 删除成功！" << endl;
            } else {
                cout << "删除失败，城市可能不存在。" << endl;
            }
        }
    } while (choice != 3);
}

void manageTransports(TransportGraph& graph) {
    int choice;
    do {
        cout << "\n----- 班次管理 -----" << endl;
        cout << "1. 添加班次" << endl;
        cout << "2. 删除班次" << endl;
        cout << "3. 返回主菜单" << endl;
        cout << "请选择: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Transport t;
            cout << "请按以下格式输入班次信息：" << endl;
            
            cout << "交通工具类型 (train/flight): ";
            getline(cin, t.type);
            
            cout << "班次号 (如 G101): ";
            getline(cin, t.id);
            
            cout << "出发城市: ";
            getline(cin, t.from);
            
            cout << "到达城市: ";
            getline(cin, t.to);
            
            // 处理时间输入
            int dep_h, dep_m, arr_h, arr_m;//出发时间，到达时间
            cout << "出发时间 (小时 分钟，如 8 0): ";
            cin >> dep_h >> dep_m;
            cout << "到达时间 (小时 分钟，如 15 0): ";
            cin >> arr_h >> arr_m;
            t.departure = dep_h * 60 + dep_m;
            t.arrival = arr_h * 60 + arr_m;
            
            cout << "票价: ";
            cin >> t.price;
            cin.ignore(); // 清除数字输入后的换行符
            
            if (graph.addTransport(t)) {
                cout << "班次 '" << t.id << "' 添加成功！" << endl;
            } else {
                cout << "添加失败，请检查城市是否存在。" << endl;
            }
        } 
        else if (choice == 2) {
            string id;
            cout << "请输入要删除的班次号: ";
            getline(cin, id);
            if (graph.removeTransport(id)) {
                cout << "班次 '" << id << "' 删除成功！" << endl;
            } else {
                cout << "删除失败，班次号可能不存在。" << endl;
            }
        }
    } while (choice != 3);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // 设置控制台输出编码为UTF-8

    TransportGraph graph;
    PathFinder finder(graph);
    
    // 下面是初始加载数据
    loadData(graph);
    
    int choice;
    do {
        printMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: {
                graph.printGraph();
                break;
            }
            
            case 2: {
                string from, to;
                cout << "请输入起始城市: ";
                cin >> from;
                cout << "请输入目的城市: ";
                cin >> to;
                
                QueryResult result = finder.findCheapest(from, to);
                result.print();
                break;
            }
            
            case 3: {
                string from, to;
                cout << "请输入起始城市: ";
                cin >> from;
                cout << "请输入目的城市: ";
                cin >> to;
                
                QueryResult result = finder.findFastest(from, to);
                result.print();
                break;
            }
            
            case 4: {
                string from, to;
                float timeWeight, costWeight;
                cout << "请输入起始城市: ";
                cin >> from;
                cout << "请输入目的城市: ";
                cin >> to;
                cout << "请输入时间权重 (0-1): ";
                cin >> timeWeight;
                cout << "请输入费用权重 (0-1): ";
                cin >> costWeight;
                
                QueryResult result = finder.findBalanced(from, to, timeWeight, costWeight);
                result.print();
                break;
            }
            
            case 5: {
                loadData(graph);
                break;
            }
            case 6: {
                manageCities(graph);
                break;
            }
            case 7: {
                manageTransports(graph);
                break;
            }
            
            case 0: {
                cout << "感谢您的使用，我们下次再见！" << endl;
                break;
            }
            
            default: {
                cout << "输入失败，请您重新输入！" << endl;
                break;
            }
        }
        
    } while (choice != 0);
    
    return 0;

}
