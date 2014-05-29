RealTimeMeshSlicing
===================

Real time mesh slicing demo - Visual Studio 2012/Win32

Video of this application in action: https://www.youtube.com/watch?v=rpomnVPVl4M

The interesting part of this application is Mesh::cut. This is where a mesh is divided into two other meshes along a plane, with polygons intersecting the plane being reconstructed.

Although this CPU implementation is quite efficient, it may be worth porting this to the GPU in the form of a geometry shader.
