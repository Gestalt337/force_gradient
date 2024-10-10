#include "Collection.h"

double kv=10., ka=1., V0=1., s0=5.4, gm=1.;

void compute_force_v(Collection& C){

    for (Cell& cl:C.cells_){
    double V = cl.get_volume();
    double P = 2.*kv*(V-V0);
    for (Vertex* vtx:cl.vertices_){
        double dvdx=0., dvdy=0., dvdz=0.;
        for (int ip:vtx->face_ids_[cl.id_]){
            Polygon* poly = cl.get_poly_from_id(ip);
            double Rx = poly->center_[0]-cl.center_[0];
            double Ry = poly->center_[1]-cl.center_[1];
            double Rz = poly->center_[2]-cl.center_[2];
            int lenp = size(poly->vertices_);
            for (int k=0;k<lenp;k++){
                if (poly->vertices_[k]==vtx){
                    dvdx=dvdx+(1./6) * (Ry*(poly->vertices_[(k-1+lenp)%lenp]->pos_[2]-poly->vertices_[(k+1)%lenp]->pos_[2])+
                    Rz*(poly->vertices_[(k+1)%lenp]->pos_[1]-poly->vertices_[(k-1+lenp)%lenp]->pos_[1]));
                    dvdy=dvdy-(1./6) * (Rx*(poly->vertices_[(k-1+lenp)%lenp]->pos_[2]-poly->vertices_[(k+1)%lenp]->pos_[2])+
                    Rz*(poly->vertices_[(k+1)%lenp]->pos_[0]-poly->vertices_[(k-1+lenp)%lenp]->pos_[0]));
                    dvdz=dvdz+(1./6) * (Rx*(poly->vertices_[(k-1+lenp)%lenp]->pos_[1]-poly->vertices_[(k+1)%lenp]->pos_[1])+
                    Ry*(poly->vertices_[(k+1)%lenp]->pos_[0]-poly->vertices_[(k-1+lenp)%lenp]->pos_[0]));
                }
            }
        }
    vtx->force_[0] +=-P*dvdx;
    vtx->force_[1] +=-P*dvdy;
    vtx->force_[2] +=-P*dvdz;
  }
  }
}

void compute_force_a(Collection& C){

    for (Cell& cl:C.cells_){
    double A = cl.get_area();
    double T = 2.*ka*(A-s0);
    for (Vertex* vtx:cl.vertices_){
        double dAdx=0., dAdy=0., dAdz=0.;
        double surf_tx=0.,surf_ty=0.,surf_tz=0.;
        for (int ip:vtx->face_ids_[cl.id_]){
            Polygon* poly = cl.get_poly_from_id(ip);
//            double xcj=poly->center_[0]-cl.center_[0];
//            double ycj=poly->center_[1]-cl.center_[1];
//            double zcj=poly->center_[2]-cl.center_[2];
            int lenp = size(poly->vertices_);
            for (int k=0;k<lenp;k++){
                if (poly->vertices_[k]==vtx){
                    int l = (k+1)%lenp;
                    int h = (k-1+lenp)%lenp;
                    Vertex* vk=poly->vertices_[k];
                    Vertex* vh=poly->vertices_[h];
                    Vertex* vl=poly->vertices_[l];
                    double xk=vk->pos_[0]-poly->center_[0];
                    double yk=vk->pos_[1]-poly->center_[1];
                    double zk=vk->pos_[2]-poly->center_[2];
                    double xl=vl->pos_[0]-poly->center_[0];
                    double yl=vl->pos_[1]-poly->center_[1];
                    double zl=vl->pos_[2]-poly->center_[2];
                    double xh=vh->pos_[0]-poly->center_[0];
                    double yh=vh->pos_[1]-poly->center_[1];
                    double zh=vh->pos_[2]-poly->center_[2];
                    double Sk = pow(yk*zl-zk*yl,2)+pow(-xk*zl+zk*xl,2)+pow(xk*yl-yk*xl,2);
                    double Skm1 = pow(yh*zk-zh*yk,2)+pow(-xh*zk+zh*xk,2)+pow(xh*yk-yh*xk,2);
                    double dSkm1dxk = -2*xh*yh*yk - 2*xh*zh*zk + 2*xk*pow(yh, 2) + 2*xk*pow(zh, 2);
                    double dSkm1dyk = 2*pow(xh, 2)*yk - 2*xh*xk*yh - 2*yh*zh*zk + 2*yk*pow(zh, 2);
                    double dSkm1dzk = 2*pow(xh, 2)*zk - 2*xh*xk*zh + 2*pow(yh, 2)*zk - 2*yh*yk*zh;
                    double dSkdxk = 2*xk*pow(yl, 2) + 2*xk*pow(zl, 2) - 2*xl*yk*yl - 2*xl*zk*zl;
                    double dSkdyk = -2*xk*xl*yl + 2*pow(xl, 2)*yk + 2*yk*pow(zl, 2) - 2*yl*zk*zl;
                    double dSkdzk = -2*xk*xl*zl + 2*pow(xl, 2)*zk - 2*yk*yl*zl + 2*pow(yl, 2)*zk;

                    double sumx = (1./4)*(dSkm1dxk * 1/sqrt(Skm1)+dSkdxk * 1/sqrt(Sk));
                    double sumy = (1./4)*(dSkm1dyk * 1/sqrt(Skm1)+dSkdyk * 1/sqrt(Sk));
                    double sumz = (1./4)*(dSkm1dzk * 1/sqrt(Skm1)+dSkdzk * 1/sqrt(Sk));
                    dAdx += sumx;
                    dAdy += sumy;
                    dAdz += sumz;
                    surf_tx += gm*sumx*poly->is_wall_;
                    surf_ty += gm*sumy*poly->is_wall_;
                    surf_tz += gm*sumz*poly->is_wall_;

                }
            }
            //cout<<poly->id_<<surf_tx<<surf_ty<<surf_tz<<endl;
        }
        vtx->force_[0] +=-T*dAdx-surf_tx;
        vtx->force_[1] +=-T*dAdy-surf_ty;
        vtx->force_[2] +=-T*dAdz-surf_tz;
    }
}
}

void dump_data(Collection& coll){

    int v_num = 0;
    int p_num = 0;
    for(Cell& c:coll.cells_){
        p_num+=c.polygons_.size();
        for(Polygon& poly:c.polygons_){
            for(Vertex* v:poly.vertices_){
                v_num+=1;
            }
        }
    }

ofstream outFile("data.vtk");
    outFile<<"# vtk DataFile Version 3.0\npolydata\nASCII\nDATASET POLYDATA\n";
    outFile<<"POINTS " + to_string(coll.vertices_.size())+" double\n";
    for(Vertex& v: coll.vertices_){
        outFile<<to_string(v.pos_[0])+" "+to_string(v.pos_[1])+" "+to_string(v.pos_[2])+"\n";
    }
    outFile<<"\n";
    outFile<<"POLYGONS "+to_string(p_num)+" "+to_string(p_num+v_num)+"\n";
    for(Cell& c:coll.cells_){
        p_num+=c.polygons_.size();
        for(Polygon& poly:c.polygons_){
            outFile<<to_string(poly.vertices_.size())+" ";
            for(Vertex* v:poly.vertices_){
                outFile<<to_string(v->id_)+" ";
            }
            outFile<<"\n";

        }
    }
    outFile.close();


}



int main(){

    Collection C("data/vertices.csv","data/cell.csv","data/polygon.csv");
    compute_force_v(C);
    compute_force_a(C);
    //for (auto v:C.vertices_){cout<<v.force_[0]<<","<<v.force_[1]<<","<<v.force_[2]<<endl;}
    cout<<C.cells_[0].volume_<<endl;
    dump_data(C);
    //2024/10/06


    return 0;

}
