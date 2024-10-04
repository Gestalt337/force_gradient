#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 28 11:40:38 2024

@author: Haoyuan Shi
Contact: hshi254@uwo.ca
"""

import pyvoro

cells = pyvoro.compute_voronoi([[0,0,0]], [[-1,1],[-1,1],[-1,1]], 1)
save = False

print(cells)
def dumplist(l):
    s = ''
    l1 = l[1:]
    s+= str(l[0])
    for item in l1:
        s += ', '
        s += str(item)
    return s
        
if save:
    with open('cells.csv', 'w') as f:
        i=0
        for cell in cells:
            f.write('cell, '+str(i)+'\n')
            # writer = csv.writer(f)
            # for key, value in cell.items():
            #     writer.writerow([key, value])
            # Write origin
            f.write('original, '+ dumplist(cell['original'])+'\n')
            f.write('volume, '+str(cell['volume'])+'\n')

            i+=1

    for cell in cells:
        i = 0
        with open('origin.csv','w') as f:
            f.write('cell, '+str(i)+'\n')
            f.write('origin, '+ dumplist(cell['original'])+'\n')
        with open('volume.csv', 'w') as f:
            f.write('cell, '+str(i)+'\n')
            f.write('volume '+str(cell['volume'])+'\n')
        with open('vertices.csv', 'w') as f:
            f.write('cell, '+str(i)+'\n')
            j = 0
            for pos in cell['vertices']:
                f.write(str(j)+', ')
                f.write(dumplist(pos)+'\n')
                j+=1
            f.write('end')
        with open('faces.csv', 'w') as f:
            f.write('cell, '+str(i)+'\n')
            j = 0
            while j < len(cell['faces']):
                f.write(str(j)+', ')
                f.write(dumplist(cell['faces'][j]['vertices'])+'\n')
                j+=1
            f.write('end')


























