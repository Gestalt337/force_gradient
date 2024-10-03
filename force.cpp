#include "readdata.h"

double kv=10., ka=1., V0=1., s0=5.4, gm=1.;

void compute_force_v(Cell& cl){

    double V = cl.get_volume();
    double P = 2.*kv*(V-V0);
    for (Vertex& vtx:cl.vertices_){
        double dvdx=0., dvdy=0., dvdz=0.;
        for (int ip:vtx.face_ids_){
            Polygon* poly = cl.get_poly_from_id(ip);
            double Rx = poly->center_[0]-cl.center_[0];
            double Ry = poly->center_[1]-cl.center_[1];
            double Rz = poly->center_[2]-cl.center_[2];
            int lenp = size(poly->vertices_);
            for (int k=0;k<lenp;k++){
                if (poly->vertices_[k]==vtx){
                    dvdx=dvdx+(1./6) * (Ry*(poly->vertices_[(k-1+lenp)%lenp].pos_[2]-poly->vertices_[k+1].pos_[2])+
                    Rz*(poly->vertices_[k+1].pos_[1]-poly->vertices_[(k-1+lenp)%lenp].pos_[1]));
                    dvdy=dvdy-(1./6) * (Rx*(poly->vertices_[(k-1+lenp)%lenp].pos_[2]-poly->vertices_[k+1].pos_[2])+
                    Rz*(poly->vertices_[k+1].pos_[0]-poly->vertices_[(k-1+lenp)%lenp].pos_[0]));
                    dvdz=dvdz+(1./6) * (Rx*(poly->vertices_[(k-1+lenp)%lenp].pos_[1]-poly->vertices_[k+1].pos_[1])+
                    Ry*(poly->vertices_[k+1].pos_[0]-poly->vertices_[(k-1+lenp)%lenp].pos_[0]));
                }
            }
        vtx.force_[0] =P*dvdx;
        vtx.force_[1] =P*dvdy;
        vtx.force_[2] =P*dvdz;
        }
    }
};

void compute_force_a(Cell cl){

    double A = cl.get_area();
    double T = 2.*ka*(A-s0);

};










int main(){

    Cell a = read_faces(read_vertices())[0];
//    cout<< a.get_area()<<endl;
//    for (auto i:a.vertices_[2].face_ids_){
//        cout<< i<<endl;
//    };
    compute_force_v(a);
    for (auto v:a.vertices_){
        cout<<v.id_<<endl;
        cout<<v.force_[0]<<","<<v.force_[1]<<","<<v.force_[2]<<endl;
    }



















return 0;

}
