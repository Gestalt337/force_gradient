#include "readdata.h"

double kv=10., ka=1., V0=1., s0=5.4, gm=1.;

void compute_force_v(Cell cl, Vertex vtx){

    double V = cl.get_volume();
    double P = 2*kv*(V-V0);



};

void compute_force_a(Cell cl, Vertex vtx){

    double A = cl.get_area();
    double T = 2*ka*(A-s0);

};










int main(){

    //double p = 2*()

    Cell a = read_faces(read_vertices())[0];
    cout<< a.get_area()<<endl;
    cout<< a.vertices_[0].face_ids_[1]<<endl;




















return 0;

}
