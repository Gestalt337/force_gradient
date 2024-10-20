#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED
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
    vector<Vertex*> vertices_;
    const int id_;
    vector<Polygon*> polygons_;
    array<double, 3> center_;
    double volume_;
    double area_tot_;

    Cell(int id, vector<Vertex*>& vertices): id_(id), vertices_(vertices) {
        get_center();}

    void get_center() {
        double xs = 0., ys = 0., zs = 0.;
        int ns = static_cast<int>(vertices_.size());
        for (auto i : vertices_) {
            xs += i->pos_[0];
            ys += i->pos_[1];
            zs += i->pos_[2];
        }
        center_ = {xs/ns, ys/ns, zs/ns};
    }

    void get_volume() {
        double v = 0.;
        for (size_t j = 0; j < polygons_.size(); ++j) {
            array<double,3> R;
            R[0]=polygons_[j]->center_[0]-center_[0];
            R[1]=polygons_[j]->center_[1]-center_[1];
            R[2]=polygons_[j]->center_[2]-center_[2];

            int nv = polygons_[j]->vertices_.size();
            double crx = 0., cry = 0., crz = 0.;
            for (int i = 0; i < nv; i++) {
                auto* vi = polygons_[j]->vertices_[i];
                auto* vi1 = polygons_[j]->vertices_[(i+1)%nv];
                double xi = vi->pos_[0] - center_[0];
                double yi = vi->pos_[1] - center_[1];
                double zi = vi->pos_[2] - center_[2];
                double xi1 = vi1->pos_[0] - center_[0];
                double yi1 = vi1->pos_[1] - center_[1];
                double zi1 = vi1->pos_[2] - center_[2];
                crx += yi*zi1 - zi*yi1;
                cry += -xi*zi1 + zi*xi1;
                crz += xi*yi1 - yi*xi1;
            }
            v += R[0]*crx + R[1]*cry + R[2]*crz;
        }
        volume_ = v / 6;
        //return volume_;
    }

    void get_area() {
        double A = 0;
        for (size_t j = 0; j < polygons_.size(); ++j) {
            array<double,3> R;
            R[0]=polygons_[j]->center_[0]-center_[0];
            R[1]=polygons_[j]->center_[1]-center_[1];
            R[2]=polygons_[j]->center_[2]-center_[2];

            int nv = polygons_[j]->vertices_.size();
            for (int i = 0; i < nv; i++) {
                auto* vi = polygons_[j]->vertices_[i];
                auto* vl = polygons_[j]->vertices_[(i+1)%nv];
                double xi = vi->pos_[0] - center_[0];
                double yi = vi->pos_[1] - center_[1];
                double zi = vi->pos_[2] - center_[2];
                double xl = vl->pos_[0] - center_[0];
                double yl = vl->pos_[1] - center_[1];
                double zl = vl->pos_[2] - center_[2];
                double Ax = yi*zl - zi*yl + R[1]*(zi-zl) + R[2]*(yl-yi);
                double Ay = -(xi*zl - zi*xl + R[0]*(zi-zl) + R[2]*(xl-xi));
                double Az = xi*yl - yi*xl + R[0]*(yi-yl) + R[1]*(xl-xi);
                A += sqrt(Ax*Ax + Ay*Ay + Az*Az);
            }
        }
        area_tot_ = A * 0.5;
        //return area_tot_;
    }

    void add_polygon(Polygon* p_add) {
        //p_add.is_wall_ = wall;

        array<double, 3> cj = {
            p_add->center_[0] - center_[0],
            p_add->center_[1] - center_[1],
            p_add->center_[2] - center_[2]
        };

        array<double, 3> r1 = {
            p_add->vertices_[0]->pos_[0] - center_[0],
            p_add->vertices_[0]->pos_[1] - center_[1],
            p_add->vertices_[0]->pos_[2] - center_[2]
        };
        array<double, 3> r2 = {
            p_add->vertices_[1]->pos_[0] - center_[0],
            p_add->vertices_[1]->pos_[1] - center_[1],
            p_add->vertices_[1]->pos_[2] - center_[2]
        };

        double r12x = r1[1]*r2[2] - r1[2]*r2[1];
        double r12y = -r1[0]*r2[2] + r1[2]*r2[0];
        double r12z = r1[0]*r2[1] - r1[1]*r2[0];
        double dp = r12x*cj[0] + r12y*cj[1] + r12z*cj[2];

        if (dp < 0) {
            reverse(p_add->vertices_.begin(), p_add->vertices_.end());
        }

        polygons_.push_back(p_add);
        for (Vertex* i : p_add->vertices_) {
            i->face_ids_[id_].push_back(p_add->id_);
        }
    }

    Polygon* get_poly_from_id(int idn) {
        for (auto& p : polygons_) {
            if (p->id_ == idn) {
                return p;
            }
        }
        return nullptr;
    }
};

#endif // CELL_H_INCLUDED
