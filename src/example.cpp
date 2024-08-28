#include <iostream>
#include "trimesh.h"
#include "ply_reader.h"

int main( int argc, char* argv[] )
{

    trimesh_t mesh;

    PlyReader::loadPlyFile("c:\\meshes\\testmesh.ply", mesh);

    PlyReader::savePlyFile("c:\\meshes\\heyo.ply", mesh);


    std::cout << "vertices count: " << mesh.vertices().size() << std::endl;
    std::cout << "faces count: " << mesh.triangles().size() << std::endl;
    std::cout << "edges count: " << mesh.halfEdges().size() / 2 << std::endl;
    
    // Use 'mesh' to walk the connectivity.
    auto triangles = mesh.triangles();
    for(const auto& triangle : triangles)
    {
        auto he1 = mesh.halfedge(triangle);
        auto he2 = mesh.halfedge(he1.next_he);
        auto he3 = mesh.halfedge(he2.next_he);
        std::cout << he1.to_vertex << " ";
        std::cout << he2.to_vertex << " ";
        std::cout << he3.to_vertex << std::endl;
    }

    return 0;
}
