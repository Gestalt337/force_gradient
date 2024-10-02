#include <iostream>
//#include "Vertices.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "Cell.h"


int main() {
//    std::ifstream file("cells.csv");  // Open the CSV file
//    if (!file.is_open()) {
//        std::cerr << "Error: Could not open the file!" << std::endl;
//        return 1;
//    }
//
//    std::string line;
//    while (std::getline(file, line)) {  // Read each line
//        std::vector<std::string> row;   // To store each row as a vector of strings
//        std::stringstream ss(line);     // String stream to split the line by commas
//        std::string value;
//
//        while (std::getline(ss, value, ',')) {  // Split by comma
//            row.push_back(value);  // Add each value to the row vector
//        }
//
//        // Output the row for demonstration purposes
//        //for (const auto& field : row) {
//          //  std::cout << field << " ";
//        //}
//        cout << row[1][1] <<endl;
//    }
//
//    file.close();  // Close the file


    Vertex v0(0,{0,0,0});
    Vertex v1(1,{1,2,3});
    Vertex v2(2,{2,2,3});
    Vertex v3(3,{1,0,3});

    Polygon p0(0, vector<Vertex> {v0,v1});
    Polygon p1(1, vector<Vertex> {v0,v3});
    Cell cell0(0,vector<Vertex> {v0,v1,v2,v3});
    cell0.add_polygon(0,vector<int> {0,1});
    cell0.add_polygon(0,vector<int> {2,3});
    //cout << p0.vertices_[1].id_ << endl;
    cout << cell0.vertices_.size() << endl;
//    cout<<p0.vertices_[0].pos_[0] <<endl;
//    cout<< cell0.center_[2]<<endl;









    return 0;
}
