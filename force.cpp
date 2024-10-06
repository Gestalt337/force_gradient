#include "readdata.h"

double kv=10., ka=1., V0=1., s0=5.4, gm=1.;

void compute_force_v(Cell& cl){

    double V = cl.get_volume();
    double P = 2.*kv*(V-V0);
    for (Vertex* vtx:cl.vertices_){
        double dvdx=0., dvdy=0., dvdz=0.;
        for (int ip:vtx->face_ids_){
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
};

void compute_force_a(Cell& cl){

    double A = cl.get_area();
    double T = 2.*ka*(A-s0);
    for (Vertex* vtx:cl.vertices_){
        double dAdx=0., dAdy=0., dAdz=0.;
        for (int ip:vtx->face_ids_){
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

//                    double Axk=yk*zl-zk*yl+ycj*(zk-zl)+zcj*(yl-yk);
//                    double Ayk=-(xk*zl-zk*xl+xcj*(zk-zl)+zcj*(xl-xk));
//                    double Azk=xk*yl-yk*xl+xcj*(yk-yl)+ycj*(xl-xk);
//                    double Axkm1=yh*zk-zh*yk+ycj*(zh-zk)+zcj*(yk-yh);
//                    double Aykm1=-(xh*zk-zh*xk+xcj*(zh-zk)+zcj*(xk-xh));
//                    double Azkm1=xh*yk-yh*xk+xcj*(yh-yk)+zcj*(xk-xh);
//                    double Sk=pow(Axk,2)+pow(Ayk,2)+pow(Azk,2);
//                    double Skm1=pow(Axkm1,2)+pow(Aykm1,2)+pow(Azkm1,2);
//                    double dSkm1dxk=2*xcj*ycj*yh - 2*xcj*ycj*yk - 2*xcj*pow(yh, 2) + 2*xcj*yh*yk + 2*xcj*zcj*zh - 2*xcj*zcj*zk - 2*xcj*pow(zh, 2) + 2*xcj*zh*zk - 2*xh*pow(ycj, 2) + 2*xh*ycj*yh + 2*xh*ycj*yk - 2*xh*yh*yk - 2*xh*pow(zcj, 2) + 2*xh*zcj*zh + 2*xh*zcj*zk - 2*xh*zh*zk + 2*xk*pow(ycj, 2) - 4*xk*ycj*yh + 2*xk*pow(yh, 2) + 2*xk*pow(zcj, 2) - 4*xk*zcj*zh + 2*xk*pow(zh, 2);
//                    double dSkm1dyk=-2*pow(xcj, 2)*yh + 2*pow(xcj, 2)*yk + 2*xcj*xh*ycj + 2*xcj*xh*yh - 4*xcj*xh*yk - 2*xcj*xk*ycj + 2*xcj*xk*yh - 2*pow(xh, 2)*ycj + 2*pow(xh, 2)*yk + 2*xh*xk*ycj - 2*xh*xk*yh + 2*ycj*zcj*zh - 2*ycj*zcj*zk - 2*ycj*pow(zh, 2) + 2*ycj*zh*zk - 2*yh*pow(zcj, 2) + 2*yh*zcj*zh + 2*yh*zcj*zk - 2*yh*zh*zk + 2*yk*pow(zcj, 2) - 4*yk*zcj*zh + 2*yk*pow(zh, 2);
//                    double dSkm1dzk=-2*pow(xcj, 2)*zh + 2*pow(xcj, 2)*zk + 2*xcj*xh*zcj + 2*xcj*xh*zh - 4*xcj*xh*zk - 2*xcj*xk*zcj + 2*xcj*xk*zh - 2*pow(xh, 2)*zcj + 2*pow(xh, 2)*zk + 2*xh*xk*zcj - 2*xh*xk*zh - 2*pow(ycj, 2)*zh + 2*pow(ycj, 2)*zk + 2*ycj*yh*zcj + 2*ycj*yh*zh - 4*ycj*yh*zk - 2*ycj*yk*zcj + 2*ycj*yk*zh - 2*pow(yh, 2)*zcj + 2*pow(yh, 2)*zk + 2*yh*yk*zcj - 2*yh*yk*zh;
//                    double dSkdxk=-2*xcj*ycj*yk + 2*xcj*ycj*yl + 2*xcj*yk*yl - 2*xcj*pow(yl, 2) - 2*xcj*zcj*zk + 2*xcj*zcj*zl + 2*xcj*zk*zl - 2*xcj*pow(zl, 2) + 2*xk*pow(ycj, 2) - 4*xk*ycj*yl + 2*xk*pow(yl, 2) + 2*xk*pow(zcj, 2) - 4*xk*zcj*zl + 2*xk*pow(zl, 2) - 2*xl*pow(ycj, 2) + 2*xl*ycj*yk + 2*xl*ycj*yl - 2*xl*yk*yl - 2*xl*pow(zcj, 2) + 2*xl*zcj*zk + 2*xl*zcj*zl - 2*xl*zk*zl;
//                    double dSkdyk=2*pow(xcj, 2)*yk - 2*pow(xcj, 2)*yl - 2*xcj*xk*ycj + 2*xcj*xk*yl + 2*xcj*xl*ycj - 4*xcj*xl*yk + 2*xcj*xl*yl + 2*xk*xl*ycj - 2*xk*xl*yl - 2*pow(xl, 2)*ycj + 2*pow(xl, 2)*yk - 2*ycj*zcj*zk + 2*ycj*zcj*zl + 2*ycj*zk*zl - 2*ycj*pow(zl, 2) + 2*yk*pow(zcj, 2) - 4*yk*zcj*zl + 2*yk*pow(zl, 2) - 2*yl*pow(zcj, 2) + 2*yl*zcj*zk + 2*yl*zcj*zl - 2*yl*zk*zl;
//                    double dSkdzk=2*pow(xcj, 2)*zk - 2*pow(xcj, 2)*zl - 2*xcj*xk*zcj + 2*xcj*xk*zl + 2*xcj*xl*zcj - 4*xcj*xl*zk + 2*xcj*xl*zl + 2*xk*xl*zcj - 2*xk*xl*zl - 2*pow(xl, 2)*zcj + 2*pow(xl, 2)*zk + 2*pow(ycj, 2)*zk - 2*pow(ycj, 2)*zl - 2*ycj*yk*zcj + 2*ycj*yk*zl + 2*ycj*yl*zcj - 4*ycj*yl*zk + 2*ycj*yl*zl + 2*yk*yl*zcj - 2*yk*yl*zl - 2*pow(yl, 2)*zcj + 2*pow(yl, 2)*zk;
                    dAdx += (1./4)*(dSkm1dxk * 1/sqrt(Skm1)+dSkdxk * 1/sqrt(Sk));
                    dAdy += (1./4)*(dSkm1dyk * 1/sqrt(Skm1)+dSkdyk * 1/sqrt(Sk));
                    dAdz += (1./4)*(dSkm1dzk * 1/sqrt(Skm1)+dSkdzk * 1/sqrt(Sk));

                }
            }
        }
        vtx->force_[0] +=-T*dAdx;
        vtx->force_[1] +=-T*dAdy;
        vtx->force_[2] +=-T*dAdz;
    }


};










int main(){

    Cell a = read_faces(read_vertices())[0];
    cout<< a.vertices_[0]->pos_[0]<<endl;
//    compute_force_v(a);
    compute_force_a(a);
//    for (auto v:a.vertices_){
//        cout<<v->id_<<endl;
//        cout<<v->force_[0]<<","<<v->force_[1]<<","<<v->force_[2]<<endl;
//    }

//2024/10/04/11:30














return 0;

}
