#ifndef VERTICES_H_INCLUDED
#define VERTICES_H_INCLUDED
#include "Vertex.h"
#include <vector>
#include <algorithm>
#include "Polygon.h"
#include <iostream>
#include <cmath>

using namespace std;

class Cell
{
public:
    vector<Vertex> vertices_;
    //array<int> V_ind_;
    const int id_;
    vector<Polygon> polygons_={};
    array<double, 3> center_;
    vector<array<double,3>> cjs_;
    double volume_;
    double area_tot_;

    void get_center(){
        double xs=0, ys=0, zs=0;
        int ns = static_cast<int>(vertices_.size());
        for (auto i: vertices_)
            {xs = xs + i.pos_[0];
            ys = ys+ i.pos_[1];
            zs =zs+ i.pos_[2];}
        //center_ = {xs/ns,ys/ns,zs/ns};
        center_={xs/ns,ys/ns,zs/ns};
    };

    double get_volume(){
        double v = 0.;
        for (int j=0;j<cjs_.size();j++){
            double Rx = cjs_[j][0]-center_[0];
            double Ry = cjs_[j][1]-center_[1];
            double Rz = cjs_[j][2]-center_[2];
            int nv = polygons_[j].vertices_.size();
            double crx=0., cry=0., crz=0.;
            for (int i=0;i<nv;i++){
//                double rint1x= polygons_[j].vertices_[i].pos_[0]-center_[0];
//                double rint1y= polygons_[j].vertices_[i].pos_[1]-center_[1];
//                double rint1z= polygons_[j].vertices_[i].pos_[2]-center_[2];
//                double rint2x= polygons_[j].vertices_[(i+1)%nv].pos_[0]-center_[0];
//                double rint2y= polygons_[j].vertices_[(i+1)%nv].pos_[1]-center_[1];
//                double rint2z= polygons_[j].vertices_[(i+1)%nv].pos_[2]-center_[2];
//                double crx = crx+ rint1y*rint2z - rint1z*rint2y;
                auto vset=polygons_[j].vertices_;
                crx = crx + move(vset)[i].pos_[1]*move(vset)[(i+1)%nv].pos_[2]-move(vset)[i].pos_[2]*move(vset)[(i+1)%nv].pos_[1]+
                (move(vset)[(i+1)%nv].pos_[1]-move(vset)[i].pos_[1])*center_[2]+(move(vset)[i].pos_[2]-move(vset)[(i+1)%nv].pos_[2])*center_[1];
                cry = cry-(move(vset)[i].pos_[0]*move(vset)[(i+1)%nv].pos_[2]-move(vset)[i].pos_[2]*move(vset)[(i+1)%nv].pos_[0]+
                (move(vset)[(i+1)%nv].pos_[0]-move(vset)[i].pos_[0])*center_[2]+(move(vset)[i].pos_[2]-move(vset)[(i+1)%nv].pos_[2])*center_[0]);
                crz = crz+move(vset)[i].pos_[0]*move(vset)[(i+1)%nv].pos_[1]-move(vset)[i].pos_[1]*move(vset)[(i+1)%nv].pos_[0]+
                (move(vset)[(i+1)%nv].pos_[0]-move(vset)[i].pos_[0])*center_[1]+(move(vset)[i].pos_[1]-move(vset)[(i+1)%nv].pos_[1])*center_[0];
            }
            v=v+Rx*crx+Ry*cry+Rz*crz;
        }
        volume_ = v*1/6;
        return v*1/6;
    }

    double get_area(){
        double A=0;
        for (int j=0;j<cjs_.size();j++){
            array<double, 3> R=cjs_[j];
            int nv = polygons_[j].vertices_.size();
            for (int i=0;i<nv;i++){
                auto aset=polygons_[j].vertices_;
                int l = (i+1)%nv;
                double Ax=move(aset)[i].pos_[1]*move(aset)[l].pos_[2]-move(aset)[i].pos_[2]*move(aset)[l].pos_[1]+
                (move(aset)[l].pos_[1]-move(aset)[i].pos_[1])*R[2]+(move(aset)[i].pos_[2]-move(aset)[l].pos_[2])*R[1];
                double Ay=move(aset)[i].pos_[0]*move(aset)[l].pos_[2]-move(aset)[i].pos_[2]*move(aset)[l].pos_[0]+ //up to a neg sign...
                (move(aset)[l].pos_[0]-move(aset)[i].pos_[0])*R[2]+(move(aset)[i].pos_[2]-move(aset)[l].pos_[2])*R[0];
                double Az=move(aset)[i].pos_[0]*move(aset)[l].pos_[1]-move(aset)[i].pos_[1]*move(aset)[l].pos_[0]+
                (move(aset)[l].pos_[0]-move(aset)[i].pos_[0])*R[1]+(move(aset)[i].pos_[1]-move(aset)[l].pos_[1])*R[0];
                A = A+sqrt(pow(Ax,2)+pow(Ay,2)+pow(Az,2));
            };
        }
        area_tot_=A*0.5;
        return A*0.5;
    };

    vector<Vertex> get_vertex(vector<int>& ind_l){
        vector<Vertex> ret_l;
        for(auto j:ind_l){
            for(auto i:move(vertices_)){
                if (i.id_==j)
                    move(ret_l).push_back(move(i));
            };
        };
        return move(ret_l);
    };

    void add_polygon(const int id, vector<int>& ind_l){
        Polygon p_add = Polygon(id, get_vertex(ind_l));

        double cjx= p_add.center_[0] - center_[0];
        double cjy= p_add.center_[1] - center_[1];
        double cjz= p_add.center_[2] - center_[2];

        cjs_.push_back(array<double,3>{cjx,cjy,cjz});

        double r12x = p_add.vertices_[0].pos_[1]*p_add.vertices_[1].pos_[2] - p_add.vertices_[0].pos_[2]*p_add.vertices_[1].pos_[1];
        double r12y = -p_add.vertices_[0].pos_[0]*p_add.vertices_[1].pos_[2] + p_add.vertices_[0].pos_[2]*p_add.vertices_[1].pos_[0];
        double r12z = p_add.vertices_[0].pos_[0]*p_add.vertices_[1].pos_[1] - p_add.vertices_[0].pos_[1]*p_add.vertices_[1].pos_[0];
        double dp=cjx*r12x+cjy*r12y+cjz*r12z;

        if (dp<0){reverse(ind_l.begin(), ind_l.end());}

        p_add.vertices_= get_vertex(ind_l);

        polygons_.push_back(move(p_add));
        for (int i:ind_l){
            vertices_[i].face_ids_.push_back(id);
        }

    };

    Polygon* get_poly_from_id(int idn){
        for (auto &p:polygons_){
            if (p.id_==idn){
                return &p;
            }
        }
        return nullptr;
    };

    //Cell(int id, vector<Vertex> vertices) : id_(id), vertices_(vertices) {};


//    Cell(int id, vector<Polygon>&& polygons): id_(id), polygons_(polygons) {
//    for (auto i:polygons){
//        for (auto j:i.vertices_){
//        if (find(vertices_.begin(), vertices_.end(), j) == vertices_.end() )
//        vertices_.push_back(j);
//        }
//    };
//    get_center();};

    Cell(int id, vector<Vertex>&& vertices): id_(id), vertices_(vertices) {
    get_center();
    };



};


#endif // VERTICES_H_INCLUDED
