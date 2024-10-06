#include <iostream>
//#include "Vertices.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "Collection.h"


int main() {
    Collection C;
    C.get_vertices();
    cout<<C.vertices_[0].id_<<endl;
    C.get_cells();







    return 0;
}
