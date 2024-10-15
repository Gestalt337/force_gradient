#include "Collection.h"
#include "variables.h"

void Collection::compute_force(){
    // Reset forces
    for (Vertex &vtx:vertices_){
        vtx.force_={0.,0.,0.};
    }

    for (Cell& cl:cells_){
        double V = cl.volume_/V0;
        double P = 2.*kv*(V-V0);
        double A = cl.area_tot_/pow(V0,2./3);
        double T = 2.*ka*(A-s0);
        int lenv =cl.vertices_.size();
        for (Vertex* vtx:cl.vertices_){
            double dvdx=0., dvdy=0., dvdz=0.;
            double dadx=0., dady=0., dadz=0.;
            for (Polygon &poly:cl.polygons_){
                double Rx = poly.center_[0]-cl.center_[0];
                double Ry = poly.center_[1]-cl.center_[1];
                double Rz = poly.center_[2]-cl.center_[2];
                int lenp = size(poly.vertices_);
                double M= 1./lenp - 1./lenv;

                for (int i=0;i<lenp;++i){
                    double xim1 = poly.vertices_[(i-1+lenp)%lenp]->pos_[0];
                    double yim1 = poly.vertices_[(i-1+lenp)%lenp]->pos_[1];
                    double zim1 = poly.vertices_[(i-1+lenp)%lenp]->pos_[2];
                    double xi = poly.vertices_[i]->pos_[0];
                    double yi = poly.vertices_[i]->pos_[1];
                    double zi = poly.vertices_[i]->pos_[2];
                    double xip1 = poly.vertices_[(i+1)%lenp]->pos_[0];
                    double yip1 = poly.vertices_[(i+1)%lenp]->pos_[1];
                    double zip1 = poly.vertices_[(i+1)%lenp]->pos_[2];
                    double xip = poly.vertices_[i]->pos_[0]-cl.center_[0];
                    double yip = poly.vertices_[i]->pos_[1]-cl.center_[1];
                    double zip = poly.vertices_[i]->pos_[2]-cl.center_[2];
                    double xip1p = poly.vertices_[(i+1)%lenp]->pos_[0]-cl.center_[0];
                    double yip1p = poly.vertices_[(i+1)%lenp]->pos_[1]-cl.center_[1];
                    double zip1p = poly.vertices_[(i+1)%lenp]->pos_[2]-cl.center_[2];
                    // p and pm are essentially short for prime. But they are subtracted by different lengths.
                    double xipm = poly.vertices_[i]->pos_[0]-poly.center_[0];
                    double yipm = poly.vertices_[i]->pos_[1]-poly.center_[1];
                    double zipm = poly.vertices_[i]->pos_[2]-poly.center_[2];
                    double xip1pm = poly.vertices_[(i+1)%lenp]->pos_[0]-poly.center_[0];
                    double yip1pm = poly.vertices_[(i+1)%lenp]->pos_[1]-poly.center_[1];
                    double zip1pm = poly.vertices_[(i+1)%lenp]->pos_[2]-poly.center_[2];

                    double Si=pow((yipm*zip1pm-zipm*yip1pm),2)+pow((xipm*zip1pm-zipm*xip1pm),2)+pow((xipm*yip1pm-yipm*xip1pm),2);

                    if(find(vtx->face_ids_[cl.id_].begin(), vtx->face_ids_[cl.id_].end(), poly.id_) != vtx->face_ids_[cl.id_].end()){
                        if (poly.vertices_[i]==vtx){
                            dvdx+=(Ry*(zim1-zip1)+Rz*(yip1-yim1));
                            dvdy-=(Rx*(zim1-zip1)+Rz*(xip1-xim1));
                            dvdz+=(Rx*(yim1-yip1)+Ry*(xip1-xim1));

                            dadx+=1./sqrt(Si) * (xipm*(pow(yip1pm,2)+pow(zip1pm,2))-xip1pm*(yipm*yip1pm+zipm*zip1pm));
                            dady+=1./sqrt(Si) * (yipm*(pow(zip1pm,2)+pow(xip1pm,2))-yip1pm*(zipm*zip1pm+xipm*xip1pm));
                            dadz+=1./sqrt(Si) * (zipm*(pow(xip1pm,2)+pow(yip1pm,2))-zip1pm*(xipm*xip1pm+yipm*yip1pm));
                        }
                        else if (poly.vertices_[(i+1)%lenp]==vtx){

                            dadx+=1./sqrt(Si) * (xip1pm*(pow(yipm,2)+pow(zipm,2))-xipm*(yipm*yip1pm+zipm*zip1pm));
                            dady+=1./sqrt(Si) * (yip1pm*(pow(zipm,2)+pow(xipm,2))-yipm*(zipm*zip1pm+xipm*xip1pm));
                            dadz+=1./sqrt(Si) * (zip1pm*(pow(xipm,2)+pow(yipm,2))-zipm*(xipm*xip1pm+yipm*yip1pm));
                        }
                    }
                    dvdx+=(M*(yip*zip1p-zip*yip1p)+1./lenv * (Rz*(yi-yip1)-Ry*(zi-zip1)));
                    dvdy+=(M*(zip*xip1p-xip*zip1p)+1./lenv * (Rx*(zi-zip1)-Rz*(xi-xip1)));
                    dvdz+=(M*(xip*yip1p-yip*xip1p)+1./lenv * (Ry*(xi-xip1)-Rx*(yi-yip1)));

                    dadx-=(1./lenp)*(xipm*(pow(yip1pm,2)+pow(zip1pm,2))-(xipm+xip1pm)*(yipm*yip1pm+zipm*zip1pm)+xip1pm*(pow(yipm,2)+pow(zipm,2)));
                    dady-=(1./lenp)*(yipm*(pow(zip1pm,2)+pow(xip1pm,2))-(yipm+yip1pm)*(zipm*zip1pm+xipm*xip1pm)+yip1pm*(pow(zipm,2)+pow(xipm,2)));
                    dadz-=(1./lenp)*(zipm*(pow(xip1pm,2)+pow(yip1pm,2))-(zipm+zip1pm)*(xipm*xip1pm+yipm*yip1pm)+zip1pm*(pow(xipm,2)+pow(yipm,2)));
                }
            }
            vtx->force_[0] +=-T*dadx*0.5-P*dvdx*1./6;
            vtx->force_[1] +=-T*dady*0.5-P*dvdy*1./6;
            vtx->force_[2] +=-T*dadz*0.5-P*dvdz*1./6;
        }
    }
}

// I merged volume and area derivative into one function above,
// so this is no longer needed.
//void compute_force_a(Collection& C){
//
//    for (Cell& cl:C.cells_){
//        double A = cl.get_area();
//        double T = 2.*ka*(A-s0);
//    for (Vertex* vtx:cl.vertices_){
//        double dAdx=0., dAdy=0., dAdz=0.;
//        double surf_tx=0.,surf_ty=0.,surf_tz=0.;
//        for (int ip:vtx->face_ids_[cl.id_]){
//            Polygon* poly = cl.get_poly_from_id(ip);
////            double xcj=poly->center_[0]-cl.center_[0];
////            double ycj=poly->center_[1]-cl.center_[1];
////            double zcj=poly->center_[2]-cl.center_[2];
//            int lenp = size(poly->vertices_);
//            for (int k=0;k<lenp;k++){
//                if (poly->vertices_[k]==vtx){
//                    int l = (k+1)%lenp;
//                    int h = (k-1+lenp)%lenp;
//                    Vertex* vk=poly->vertices_[k];
//                    Vertex* vh=poly->vertices_[h];
//                    Vertex* vl=poly->vertices_[l];
//                    double xk=vk->pos_[0]-poly->center_[0];
//                    double yk=vk->pos_[1]-poly->center_[1];
//                    double zk=vk->pos_[2]-poly->center_[2];
//                    double xl=vl->pos_[0]-poly->center_[0];
//                    double yl=vl->pos_[1]-poly->center_[1];
//                    double zl=vl->pos_[2]-poly->center_[2];
//                    double xh=vh->pos_[0]-poly->center_[0];
//                    double yh=vh->pos_[1]-poly->center_[1];
//                    double zh=vh->pos_[2]-poly->center_[2];
//                    double Sk = pow(yk*zl-zk*yl,2)+pow(-xk*zl+zk*xl,2)+pow(xk*yl-yk*xl,2);
//                    double Skm1 = pow(yh*zk-zh*yk,2)+pow(-xh*zk+zh*xk,2)+pow(xh*yk-yh*xk,2);
//                    double dSkm1dxk = -2*xh*yh*yk - 2*xh*zh*zk + 2*xk*pow(yh, 2) + 2*xk*pow(zh, 2);
//                    double dSkm1dyk = 2*pow(xh, 2)*yk - 2*xh*xk*yh - 2*yh*zh*zk + 2*yk*pow(zh, 2);
//                    double dSkm1dzk = 2*pow(xh, 2)*zk - 2*xh*xk*zh + 2*pow(yh, 2)*zk - 2*yh*yk*zh;
//                    double dSkdxk = 2*xk*pow(yl, 2) + 2*xk*pow(zl, 2) - 2*xl*yk*yl - 2*xl*zk*zl;
//                    double dSkdyk = -2*xk*xl*yl + 2*pow(xl, 2)*yk + 2*yk*pow(zl, 2) - 2*yl*zk*zl;
//                    double dSkdzk = -2*xk*xl*zl + 2*pow(xl, 2)*zk - 2*yk*yl*zl + 2*pow(yl, 2)*zk;
//
//                    double sumx = (1./4)*(dSkm1dxk * 1/sqrt(Skm1)+dSkdxk * 1/sqrt(Sk));
//                    double sumy = (1./4)*(dSkm1dyk * 1/sqrt(Skm1)+dSkdyk * 1/sqrt(Sk));
//                    double sumz = (1./4)*(dSkm1dzk * 1/sqrt(Skm1)+dSkdzk * 1/sqrt(Sk));
//                    dAdx += sumx;
//                    dAdy += sumy;
//                    dAdz += sumz;
//                    //surf_tx += gm*sumx*poly->is_wall_;
//                    //surf_ty += gm*sumy*poly->is_wall_;
//                    //surf_tz += gm*sumz*poly->is_wall_;
//
//                }
//            }
//            //cout<<poly->id_<<surf_tx<<surf_ty<<surf_tz<<endl;
//        }
//        vtx->force_[0] +=-T*dAdx;//-surf_tx;
//        vtx->force_[1] +=-T*dAdy;//-surf_ty;
//        vtx->force_[2] +=-T*dAdz;//-surf_tz;
//    }
//}
//}

// I am neglecting Brownain motion for now.
void Collection::compute_velocity(){
//for (Vertex& vertex : vertices_) {
//        vertex.velocity_={0.,0.,0.}; //reset velocity
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; ++m) {
            vertex.velocity_[m] = mu * vertex.force_[m];
        }
    }
    // remove drift velocity
    double average_velo[3] = {0., 0., 0.};
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; ++m) {
            average_velo[m] += vertex.velocity_[m];
        }
    }
    for (int m = 0; m < 3; m++) {
        average_velo[m] /= vertices_.size();
    }
    for (Vertex& vertex : vertices_) {
        for (int m = 0; m < 3; m++) {
            vertex.velocity_[m] = vertex.velocity_[m] - average_velo[m];
        }
    }
};

void Collection::update_cell(){
    num_iters_ += 1;
    for (int i = 0; i < vertices_.size(); ++i) {
        for (int m = 0; m < 3; m++) {
            vertices_[i].pos_[m] += vertices_[i].velocity_[m]*dt;
        }
    }
    // Recalculate cell geometric.
    for (Cell &cell:cells_){
        cell.get_center();
        cell.get_volume();
        cell.get_area();
        for (Polygon& poly:cell.polygons_){
            poly.get_center();
        }
    }
};
