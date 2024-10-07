#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include "Vertex.h"
#include <vector>

using namespace std;

class Polygon
{
public:
    vector<Vertex*> vertices_;
    const int id_;
    array<double, 3> center_;
    int is_wall_;

    Polygon(int id, vector<Vertex*>& vertices) : id_(id), vertices_(vertices) {
        get_center();
    }

    void get_center() {
        double xs = 0, ys = 0, zs = 0;
        long int ns = static_cast<int>(vertices_.size());
        for (auto i : vertices_) {
            xs += i->pos_[0];
            ys += i->pos_[1];
            zs += i->pos_[2];
        }
        center_ = {xs/ns, ys/ns, zs/ns};
    }

    Vertex* get_vt_from_id(int idn) {
        for (auto vt : vertices_) {
            if (vt->id_ == idn) {
                return vt;
            }
        }
        return nullptr;
    }
};

#endif // POLYGON_H_INCLUDED
