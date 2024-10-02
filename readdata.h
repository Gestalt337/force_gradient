#ifndef READDATA_H
#define READDATA_H
#include "Cell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<Cell> read_vertices(){

    vector<Cell> cells;

    std::ifstream file("vertices.csv");  // Open the CSV file
//    if (!file.is_open()) {
//        std::cerr << "Error: Could not open the file!" << std::endl;
//        return 1;
//    }

    vector<Vertex> Vs={};
    int nc = -1;
    std::string line;

    while (std::getline(move(file), line)) {
        if (line[0]=='e'){std::getline(move(file), line);
        cells.push_back(Cell(nc,move(Vs)));
        Vs.clear();
        if (file.eof()){file.close();return cells;}}
        if (line[0]=='c'){nc=nc+1;std::getline(move(file), line);}

        std::vector<std::string> row;   // Vector to store the split values for each row
        std::stringstream ss(line);      // Create a string stream from the line
        std::string value;

        // Split the line by commas
        while (std::getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
        }
        int id = stoi(row[0]);
        array<double,3> V{stod(row[1]),stod(row[2]),stod(row[3])};
        Vertex Vt(id,V);
        Vs.push_back(move(Vt));
    }
};

vector<Cell> read_faces(vector<Cell>&& cells){

    std::ifstream file("faces.csv");  // Open the CSV file
    int nc = -1;
    std::string line;

    while (std::getline(file, line)) {
        if (line[0]=='e'){std::getline(file, line);
        if (file.eof()){file.close();return cells;}}
        if (line[0]=='c'){nc=nc+1;std::getline(file, line);}

        std::vector<std::string> row;   // Vector to store the split values for each row
        std::stringstream ss(line);      // Create a string stream from the line
        std::string value;

        // Split the line by commas
        while (std::getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
        }
        const int id = stoi(row[0]);
        vector<int> F;
        for (int i=1;i<row.size();i++){F.push_back(stoi(row[i]));}
        //cout<<F[0]<<F[1]<<F[2]<<F[3]<<endl;
        move(cells)[nc].add_polygon(id, F);
    };
};

#endif

//int main() {
//    vector<Cell> a = read_faces(read_vertices());
//    cout<<a[0].polygons_.size()<<endl;
//
//    return 0;
//}
