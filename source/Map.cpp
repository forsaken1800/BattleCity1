#include "Map.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<vector<int>> loadMapFromCSV(const string& filename) {
    vector<vector<int>> mapData;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Cannot open file: " << filename << endl;
        return mapData;
    }

    string line;
    while (getline(file, line)) {
        vector<int> row;
        istringstream sstream(line);
        string cell;
        while (getline(sstream, cell, ';')) {
            try {
                row.push_back(stoi(cell));
            } catch (exception& e) {
                // Nếu chuyển đổi thất bại, đẩy giá trị 0 mặc định
                row.push_back(0);
            }
        }
        mapData.push_back(row);
    }

    file.close();
    return mapData;
}
