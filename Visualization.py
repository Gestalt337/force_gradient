#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Oct 10 11:29:49 2024

@author: haoyuan
"""

import vtk

# Step 1: Read the VTK file
reader = vtk.vtkPolyDataReader()
reader.SetFileName("data.vtk")  # Replace with your VTK file name
reader.Update()  # Read the file

# Step 2: Create a mapper
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(reader.GetOutputPort())

# Step 3: Create an actor
actor = vtk.vtkActor()
actor.SetMapper(mapper)

# Step 4: Set up the renderer, render window, and interactor
renderer = vtk.vtkRenderer()
renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)

renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)

# Add the actor to the scene
renderer.AddActor(actor)
renderer.SetBackground(0.1, 0.2, 0.3)  # Set background color to white

# Step 5: Render and interact
renderWindow.Render()
renderWindowInteractor.Start()
