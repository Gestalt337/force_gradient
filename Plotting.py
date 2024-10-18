#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 16 13:34:25 2024

@author: Haoyuan Shi
Contact: hshi254@uwo.ca
"""

import matplotlib.pyplot as plt
import numpy as np

def get_cell_area():
    area_d = {}
    with open("data/output/area.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        while line != "":
            while lst[0] != "ITER" and line!="":
                if lst[0] not in area_d:
                    area_d[lst[0]]=[float(lst[1])]
                else:
                    area_d[lst[0]].append(float(lst[1]))
                line = file.readline()
                lst = line.strip("\n").split(" ")
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return area_d

def get_cell_volume():
    volume_d = {}
    with open("data/output/volume.txt") as file:
        line = file.readline()
        lst = line.strip("\n").split(" ")
        while line != "":
            while lst[0] != "ITER" and line!="":
                if lst[0] not in volume_d:
                    volume_d[lst[0]]=[float(lst[1])]
                else:
                    volume_d[lst[0]].append(float(lst[1]))
                line = file.readline()
                lst = line.strip("\n").split(" ")
            line = file.readline()
            lst = line.strip("\n").split(" ")
    return volume_d

def get_var():
    var_d = {}
    with open("variables.h") as file:
        line = file.readline()
        while line != "":
            if line[0:6] == "double":
                lst = line[7:].strip(";\n").split(",")
                for eqn in lst:
                    var,val = eqn.strip().split("=")
                    var_d[var]=float(val)
            line = file.readline()
    return var_d
                        
                
            

area_d = get_cell_area()
volume_d = get_cell_volume()
var_d = get_var()

length = len(area_d["Cell0"])
dt = float(var_d["dt"])
v0 = float(var_d["V0"])
v0_l = np.full((1,length),v0)[0]
s0 = float(var_d["s0"])
s0_l = np.full((1,length),s0)[0]
t = np.arange(0,length,1)

fig, ax=plt.subplots(2,1,sharex=True)
ax[0].set_title("Change in Volume (Cube)")
#plt.xlabel("time(s)")
ax[0].set_ylabel("rel. volume")
ax[0].plot(t,volume_d["Cell0"],label="Cell 0")
ax[0].plot(t,v0_l,label="$v_0$",ls="dashed")
ax[0].legend()

ax[1].set_title("Change in Surface Area (Cube)")
ax[1].set_xlabel("N")
ax[1].set_ylabel("rel. surf. area")
plt.plot(t,area_d["Cell0"],label="Cell 0")
plt.plot(t,s0_l,label="$s_0$",ls="dashed")
ax[1].legend()

fig.savefig("data/output/plot0.png")





