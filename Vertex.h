#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <array>
#include <vector>

using namespace std;


class Vertex
{
public:
    const int id_;
    array<double,3> pos_;
    array<double, 3> force_={0,0,0};
    vector<int> face_ids_;

    Vertex(int id, const array<double,3> &pos) : id_(id), pos_(pos) {};

    bool operator==(const Vertex& other) const {
        // Define the equality of Vertex class.
        return this->id_ == other.id_;
    }


};
#endif // VERTEX_H_INCLUDED
