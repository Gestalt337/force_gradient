#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

#include "Vertex.h"
#include "Vertices.h"
#include <cmath>

class Edge
{
private:
    Vertex v0_;
    Vertex v1_;
    double length_;

    double cal_len(){
        double a = pow(v0_.pos()[0]-v1_.pos()[0],2);
        double b = pow(v0_.pos()[1]-v1_.pos()[1],2);
        double c = pow(v0_.pos()[2]-v1_.pos()[2],2);
        return sqrt(a+b+c);
    };

public:
    Edge(Vertex v0, Vertex v1) :v0_(v0), v1_(v1) {length_=cal_len();};

    double get_length(){
        return length_;
    };




};

#endif // EDGE_H_INCLUDED
