#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 28 11:40:38 2024

@author: Haoyuan Shi
Contact: hshi254@uwo.ca
Credit: Tao Zhang zhangtao.scholar@sjtu.edu.cn &
        Jennifer Schwarz jschwarz@physics.syr.edu
"""

import pyvoro
import numpy as np

class Vertex:
    def __init__(self,idn,pos):
        self.idn_ = idn
        self.pos_ = pos
    def __str__(self):
        return f'{self.idn_}, {self.pos_[0]}, {self.pos_[1]}, {self.pos_[2]}'

class Polygon:
    def __init__(self, idn,vertices, is_wall):
        self.id_ = idn
        self.vertices_ = vertices
        self.is_wall_ = is_wall
    def __str__(self):
        s=str(self.id_)+','
        for vertex_id in self.vertices_:
            s+=str(vertex_id)
            s+=','
        #s+=str(self.is_wall_)
        return s

class Cell:
    def __init__(self, idc, polygons):
        self.id_ = idc
        self.vt_id_ = []
        self.polygons_ = polygons
    def get_vt_id(self):
        for p in self.polygons_:
            for vertex_id in p.vertices_:
                if vertex_id not in self.vt_id_:
                    self.vt_id_.append(vertex_id)
                    self.vt_id_.sort()
    def show_vts(self):
        s=f'Cell, {self.id_}\n'
        s+=str(self.vt_id_)[1:-1]
        return s

    def __str__(self):
        s=f'Cell, {self.id_}'
        for poly in self.polygons_:
            s+='\n'
            s+=str(poly)
        return s

def generatePoints(Lx, Ly, Lz):
    from datetime import datetime
    np.random.seed(int(datetime.utcnow().timestamp()))
    # np.random.seed(2161133)
    Nvertices = int(Lx*Ly*Lz)
    points = []
    for i in range(Nvertices):
        point = []
        point.append(np.random.uniform(-Lx, Lx))
        point.append(np.random.uniform(-Ly, Ly))
        point.append(np.random.uniform(-Lz, Lz))
        points.append(point)

    return points 

def voronoi():
    Lx = int(input("Lx: ")or 8)
    Ly = int(input("Ly: ")or 8)
    Lz = int(input("Lz: ")or 8)
    points = generatePoints(Lx, Ly, Lz)
    #points = [[0, 0, 0],[0.5,0.5,0.5]]
    voroDict = pyvoro.compute_voronoi(
        points,  # point positions
        [[-Lx, Lx], [-Ly, Ly], [-Lz, Lz]],  # limits
        2.0,  # block size
        periodic = [True, True, True])
    return voroDict

def create_data(voroDict):
    vertex_l, cell_l = [], []
    nc, i = 0, 0
    for cell in voroDict:
        l=0
        vs = np.round(cell['vertices'],2).tolist()
        rep_d, nrep_d = {}, {}
        for position in vs:
            if len(vertex_l) !=0:
                eq_vtx = next((v.idn_ for v in vertex_l if v.pos_ == position), -1)
                if eq_vtx==-1:
                    vertex_l.append(Vertex(i,position))
                    nrep_d[l]=i
                    i+=1
                    l+=1
                else:
                    rep_d[l]=eq_vtx
                    l+=1
            else:
                vertex_l.append(Vertex(i,position))
                nrep_d[l]=i
                i+=1
                l+=1
        
        fs = cell['faces']
        polys = []
        poly_id=0
        for f in fs:
            k = 0
            L = f['vertices']
            while k<len(L):
                if L[k] in nrep_d:
                    L[k]=nrep_d[L[k]]
                    k+=1
                elif L[k] in rep_d:
                    L[k]=rep_d[L[k]]
                    k+=1
                else:
                    print('error!')
                    k+=1
            if f['adjacent_cell']<=0:
                polys.append(Polygon(poly_id, L, 1))
            else:
                polys.append(Polygon(poly_id, L, 0))
            poly_id+=1
        C = Cell(nc, polys)
        C.get_vt_id()
        cell_l.append(C)
        nc+=1
    return vertex_l,cell_l

#v = voronoi()

def save_data():
    vertex_l,cell_l=create_data(voronoi())
    with open('data/vertices.csv', 'w') as vtc:
        for v in vertex_l:
            vtc.write(str(v))
            vtc.write('\n')
            
    with open('data/polygon.csv','w') as poly:
        for cell in cell_l:
            poly.write(str(cell))
            poly.write('\n')
    
    with open('data/cell.csv','w') as cvtc:
        for cell in cell_l:
            cvtc.write(cell.show_vts())
            cvtc.write('\n')
            
save_data()
        




















       
# if save:
#     with open('cells.csv', 'w') as f:
#         i=0
#         for cell in cells:
#             f.write('cell, '+str(i)+'\n')
#             # writer = csv.writer(f)
#             # for key, value in cell.items():
#             #     writer.writerow([key, value])
#             # Write origin
#             f.write('original, '+ dumplist(cell['original'])+'\n')
#             f.write('volume, '+str(cell['volume'])+'\n')

#             i+=1

#     for cell in cells:
#         i = 0
#         with open('origin.csv','w') as f:
#             f.write('cell, '+str(i)+'\n')
#             f.write('origin, '+ dumplist(cell['original'])+'\n')
#         with open('volume.csv', 'w') as f:
#             f.write('cell, '+str(i)+'\n')
#             f.write('volume '+str(cell['volume'])+'\n')
#         with open('vertices.csv', 'w') as f:
#             f.write('cell, '+str(i)+'\n')
#             j = 0
#             for pos in cell['vertices']:
#                 f.write(str(j)+', ')
#                 f.write(dumplist(pos)+'\n')
#                 j+=1
#             f.write('end')
#         with open('faces.csv', 'w') as f:
#             f.write('cell, '+str(i)+'\n')
#             j = 0
#             while j < len(cell['faces']):
#                 f.write(str(j)+', ')
#                 f.write(dumplist(cell['faces'][j]['vertices'])+'\n')
#                 j+=1
#             f.write('end')
