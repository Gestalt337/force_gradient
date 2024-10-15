#include "Collection.h"
#include <filesystem>

using namespace std;

long int max_iters=50;
int save_iters=1;

void dump_data(Collection& coll){
    int v_num = 0;
    int p_num = 0;
    string wrt="";
    wrt.append("# vtk DataFile Version 3.0\npolydata\nASCII\nDATASET POLYDATA\n");
    wrt.append("POINTS " + to_string(coll.vertices_.size())+" double\n");
    for(Vertex& v: coll.vertices_){
        wrt.append(to_string(v.pos_[0])+" "+to_string(v.pos_[1])+" "+to_string(v.pos_[2])+"\n");
    }
    wrt.append("\n");
    string to_find = "PLACEHOLDER";
    wrt.append(to_find);
    //s+="POLYGONS "+to_string(p_num)+" "+to_string(p_num+v_num)+"\n";
    for(Cell& c:coll.cells_){
        p_num+=c.polygons_.size();
        for(Polygon& poly:c.polygons_){
            wrt.append(to_string(poly.vertices_.size())+" ");
            for(Vertex* v:poly.vertices_){
                wrt.append(to_string(v->id_)+" ");
                v_num+=1;
            }
            wrt.append("\n");
        }
    }
    size_t pos = wrt.find(to_find);
    wrt.replace(pos, to_find.size(), "POLYGONS "+to_string(p_num)+" "+to_string(p_num+v_num)+"\n");
    ofstream outFile("data/output/"+to_string(coll.num_iters_)+".vtk");
    outFile<<wrt;
    outFile.close();
}

void run(){
    filesystem::remove_all("data/output/");
    filesystem::create_directories("data/output/");
    //Set up collection
    Collection C(0);
    dump_data(C);
    for (long int i=0; i<max_iters; ++i){
        C.compute_force();
        C.compute_velocity();
        C.update_cell();
        if (C.num_iters_%save_iters==0){
            dump_data(C);
        }
    }
}

int main(){

    //for (auto v:C.vertices_){cout<<v.force_[0]<<","<<v.force_[1]<<","<<v.force_[2]<<endl;}
    //cout<<C.cells_[0].area_tot_<<endl;
    //cout<<C.cells_[0].area_tot_<<endl;
    //cout<<C.cells_[0].volume_<<endl;
    //for (auto v:C.vertices_){cout<<v.pos_[0]<<","<<v.pos_[1]<<","<<v.pos_[2]<<endl;}
    //cout<<C.cells_[0].volume_<<endl;
    //dump_data(C);
    //2024/10/14
    run();


    return 0;

}
