#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include "Vertex.h"
#include <vector>

using namespace std;

class Polygon
{
public:
    vector<Vertex> vertices_;
    //array<int> V_ind_;
    const int id_;
    array<double, 3> center_;

    void get_center(){
        double xs=0, ys=0, zs=0;
        long int ns = static_cast<int>(vertices_.size());
        for (auto i: vertices_)
            {xs = xs + i.pos_[0];
            ys = ys+ i.pos_[1];
            zs =zs+ i.pos_[2];}
        //center_ = {xs/ns,ys/ns,zs/ns};
        center_={xs/ns,ys/ns,zs/ns};
    };


    Polygon(int id, vector<Vertex>&& vertices) : id_(id), vertices_(vertices) {get_center();};

//    vector<Vertex> get_vertex(vector<int> ind_l){
//        vector<Vertex> ret_l;
//        for(int i=0;i<vertices_.size();i++){
//            for(int j=0; j<ind_l.size();j++){
//                if (vertices_[i].id_==ind_l[j])
//                    ret_l.push_back(vertices_[i]);
//            };
//        };
//        return ret_l;
//    };

};






#endif // POLYGON_H_INCLUDED
