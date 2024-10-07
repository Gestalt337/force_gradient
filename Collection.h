#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED
#include "Cell.h"
#include <fstream>
#include <sstream>

using namespace std;

class Collection
{
public:
    vector<Vertex> vertices_;
    vector<Cell> cells_;
    string path1_, path2_, path3_;

    Collection(string path1, string path2, string path3) : path1_(path1), path2_(path2), path3_(path3) {
        get_vertices();
        get_cells();
        get_polygons();
    }

    void get_vertices() {
        ifstream file(path1_);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            vector<string> row;
            while (getline(ss, value, ',')) {
                row.push_back(value);
            }
            int id = stoi(row[0]);
            array<double, 3> pos{stod(row[1]), stod(row[2]), stod(row[3])};
            vertices_.emplace_back(id, pos);
        }
    }

    Vertex* get_vertex(int i) {
        for (auto& v : vertices_) {
            if (v.id_ == i) {
            return &v;
            }

        }
        return nullptr;
    }

    Cell* get_cell_ptr(int i) {
    for (auto& c : cells_) {
        if (c.id_ == i) {
        return &c;
        }

    }
    return nullptr;
}

void get_cells(){

    ifstream file(path2_);
    string line;
    int cell_id;

    while (std::getline(file, line)) {

        std::vector<std::string> row;   // Vector to store the split values for each row
        std::stringstream ss(line);      // Create a string stream from the line
        std::string value;

        // Split the line by commas
        while (std::getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
    }
    if (row[0]=="Cell"){cell_id=stoi(row[1]);}
    else{
    vector<Vertex*> vts={};
    for(auto i:row){vts.push_back(get_vertex(stoi(i)));}
    cells_.push_back(Cell(cell_id,vts));}
    };
}

void get_polygons(){

    ifstream file(path3_);
    string line;
    int cell_id;
    Cell* thecell;

    while (getline(file, line)) {

        vector<std::string> row;   // Vector to store the split values for each row
        stringstream ss(line);      // Create a string stream from the line
        string value;

        // Split the line by commas
        while (getline(ss, value, ',')) {
            row.push_back(value);         // Add each value to the row vector
    }
        if(row[0]=="Cell"){thecell=get_cell_ptr(stoi(row[1]));}
        else{
            vector<Vertex*> vts;
            int pid=stoi(row[0]);
            int is_wall=stoi(row.back());
            row.erase(row.begin());
            row.pop_back();

            for(auto i:row){vts.push_back(get_vertex(stoi(i)));}

            thecell->add_polygon(pid,vts,is_wall);


        }




}
}
};





#endif // COLLECTION_H_INCLUDED
