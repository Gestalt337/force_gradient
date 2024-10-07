#include <iostream>
#include <string>
#Collection.h"


using namespace std;


int main() {
    Collection C("data/vertices.csv","data/cells.csv","data/polygons.csv");
    cout<<C.vertices_[0].pos_[0]<<C.vertices_[0].pos_[1]<<C.vertices_[0].pos_[2]<<"duhovo"<<endl;
    return 0;
}
