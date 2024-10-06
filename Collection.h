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
    vector<Cell> cells_={};

    void get_vertices(){
        ifstream file("data/vertices.csv");  // Open the CSV file
        string line;
        while (getline(move(file), line)) {

            vector<string> row;   // Vector to store the split values for each row
            stringstream ss(line);      // Create a string stream from the line
            string value;

            // Split the line by commas
            while (getline(ss, value, ',')) {
                row.push_back(value);         // Add each value to the row vector
            }
            int id = stoi(row[0]);
            array<double,3> pos{stod(row[1]),stod(row[2]),stod(row[3])};
            Vertex v(id,pos);
            vertices_.push_back(v);
        }
    };

    Vertex* get_vertex(int i){
        for(Vertex v: move(vertices_) ){
            if (v.id_ == i){
                Vertex* vptr = &v;
                return vptr;
            }
        }
        return nullptr;
    }

    void get_cells(){
        ifstream file("data/cell.csv");  // Open the CSV file
        ifstream data("data/polygon.csv");  // Open the CSV file
        string line;
        string linep;
        getline(move(file), line);
        getline(move(data), linep);

        while (!file.eof()) {

            vector<string> row;   // Vector to store the split values for each row
            stringstream ss(line);      // Create a string stream from the line
            string value;

            // Split the line by commas
            while (getline(ss, value, ',')) {
                row.push_back(value);         // Add each value to the row vector
            }
            int cell_id;
            vector<Vertex*> vptrs;
            if (row[0]=="Cell"){
                int cell_id = stoi(row[1]);
            } else{
                for (auto i:row){
                    vector<Vertex*> vptrs;
                    Vertex* vptr = get_vertex(stoi(i));
                    vptrs.push_back(vptr);
                }
                Cell cl = Cell(cell_id,vptrs);

                while(linep[0]!='C'){
                    vector<string> rowp;   // Vector to store the split values for each row
                    stringstream ssp(linep);      // Create a string stream from the line
                    string valuep;

                    // Split the line by commas
                    while (getline(ssp, valuep, ',')) {
                        rowp.push_back(valuep);         // Add each value to the row vector
                    }
                    const int poly_id=stoi(rowp[0]);
                    int is_wall = stoi(rowp.back());
                    rowp.pop_back();
                    rowp.erase(rowp.begin());
                    vector<Vertex*> pptrs;
                    for(auto pid: rowp){Vertex*pptr=get_vertex(stoi(pid));pptrs.push_back(pptr);}
                    cl.add_polygon(poly_id,pptrs,is_wall);
                    getline(move(data), linep);
                }
                    cells_.push_back(cl);

            }

        getline(move(file), line);
        if(linep[0]=='C'){getline(move(data), linep);}
        }
    };

    //Collection(){get_vertices();get_cells;}

};
#endif // COLLECTION_H_INCLUDED
