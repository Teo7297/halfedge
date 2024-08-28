#include <iostream>
#include "trimesh.h"
#include "ply_reader.h"

int main( int argc, char* argv[] )
{

    trimesh::trimesh_t mesh;

    PlyReader::loadPlyFile("c:\\meshes\\pp.ply", mesh);

    std::cout << "vertices count: " << mesh.vertices().size() << std::endl;
    std::cout << "faces count: " << mesh.triangles().size() << std::endl;
    std::cout << "edges count: " << mesh.halfEdges().size() / 2 << std::endl;
    
    PlyReader::savePlyFile("c:\\meshes\\heyo.ply", mesh);
    return 0;
}
