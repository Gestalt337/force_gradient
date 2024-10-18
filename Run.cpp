#include "Collection.h"
#include <filesystem>

using namespace std;

long int max_iters=50;
int save_iters=1;

void dump_vtk(Collection& coll){
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
    ofstream outFile("data/output/vtk/topo_"+to_string(coll.num_iters_)+".vtk");
    outFile<<wrt;
    outFile.close();
}

void run(){
    filesystem::remove_all("data/output/vtk/");
    filesystem::create_directories("data/output/vtk/");
    //Set up collection
    Collection C(0);
    dump_vtk(C);
    ofstream volumeFile("data/output/volume.txt");
    ofstream areaFile("data/output/area.txt");
    ofstream forceFile("data/output/force.txt");
    for (int ic=0;ic<C.cells_.size();ic++){
        volumeFile<<"ITER "+to_string(C.num_iters_)<<endl;
        areaFile<<"ITER "+to_string(C.num_iters_)<<endl;
        forceFile<<"ITER "+to_string(C.num_iters_)<<endl;
        volumeFile<<"Cell"+to_string(C.cells_[ic].id_)+" "+to_string(C.cells_[ic].volume_)<<endl;
        areaFile<<"Cell"+to_string(C.cells_[ic].id_)+" "+to_string(C.cells_[ic].area_tot_)<<endl;
        forceFile<<"Cell"+to_string(C.cells_[ic].id_)+" "<<endl;
        for (Vertex vertex:C.vertices_){
            forceFile<<vertex.id_<<" "<<sqrt(pow(vertex.force_[0],2)+pow(vertex.force_[1],2)+pow(vertex.force_[2],2))<<endl;
        }
    }
    for (long int i=0; i<max_iters-1; ++i){
        C.compute_force();
        C.compute_velocity();
        C.update_cell();
        if (C.num_iters_%save_iters==0){
            dump_vtk(C);
            for (int ic=0;ic<C.cells_.size();ic++){
                volumeFile<<"ITER "+to_string(C.num_iters_)<<endl;
                areaFile<<"ITER "+to_string(C.num_iters_)<<endl;
                forceFile<<"ITER "+to_string(C.num_iters_)<<endl;
                volumeFile<<"Cell"+to_string(C.cells_[ic].id_)+" "+to_string(C.cells_[ic].volume_)<<endl;
                areaFile<<"Cell"+to_string(C.cells_[ic].id_)+" "+to_string(C.cells_[ic].area_tot_)<<endl;
                forceFile<<"Cell"+to_string(C.cells_[ic].id_)<<endl;
                for (Vertex vertex:C.vertices_){
                    forceFile<<vertex.id_<<" "<<sqrt(pow(vertex.force_[0],2)+pow(vertex.force_[1],2)+pow(vertex.force_[2],2))<<endl;
                }
            }
        }
    }
    volumeFile.close();
    areaFile.close();
    forceFile.close();
}

int main(){
    run();


    return 0;

}
