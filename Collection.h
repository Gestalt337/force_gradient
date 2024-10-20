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
    vector<Polygon> polygons_;
    int id_;
    long int num_iters_=0;

    Collection(int id):id_(id) {
        get_vertices();
        get_cells();
        get_polygons_c();
        get_polygons();
    }

    void get_vertices() {
        ifstream file("data/vertices.csv");
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

    Polygon* get_poly_ptr(int i) {
        for (auto& p : polygons_) {
            if (p.id_ == i) {
            return &p;
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

        ifstream file("data/cell.csv");
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
                cells_.push_back(Cell(cell_id,vts));
            }
        };
    }

    void get_polygons(){

        ifstream file("data/polygon.csv");
        string line;
        int cell_id=-1;
        Cell* thecell=nullptr;

        while (getline(file, line)) {

            vector<std::string> row;   // Vector to store the split values for each row
            stringstream ss(line);      // Create a string stream from the line
            string value;

            // Split the line by commas
            while (getline(ss, value, ',')) {
                row.push_back(value);         // Add each value to the row vector
            }
            if (row[0]=="Cell"){
                cell_id=stoi(row[1]);
                thecell = get_cell_ptr(cell_id);
            }
            else{
                for (string pid:row){
                    Polygon* p_add = get_poly_ptr(stoi(pid));
                    thecell->add_polygon(p_add);
                    p_add->cell_ids_.push_back(cell_id);
                }
                thecell->get_volume();
                thecell->get_area();
            }
        }
    }

    void get_polygons_c(){

        ifstream file("data/polygons.csv");
        string line;

        while (getline(file, line)) {

            vector<std::string> row;   // Vector to store the split values for each row
            stringstream ss(line);      // Create a string stream from the line
            string value;

            // Split the line by commas
            while (getline(ss, value, ',')) {
                row.push_back(value);         // Add each value to the row vector
            }
            int poly_id = stoi(row[0]);
            vector<Vertex*> poly_v={};
            for (int i=1;i<row.size();++i){
                poly_v.push_back(get_vertex(stoi(row[i])));
            }
            polygons_.push_back(Polygon(poly_id,poly_v));
        }

    }

    void compute_force();
    void compute_velocity();
    void update_cell();
};





#endif // COLLECTION_H_INCLUDED
