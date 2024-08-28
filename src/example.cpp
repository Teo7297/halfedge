#include <iostream>
#include "trimesh.h"
#include "ply_reader.h"

int main( int argc, char* argv[] )
{

    PlyReader ply("c:\\meshes\\pp.ply");
    

    ply.savePlyFile("c:\\meshes\\heyo.ply");

    std::vector<trimesh::triangle_t> triangles(ply.getFaces().size());
    std::vector<trimesh::vertex_t> vertices(ply.getVertices().size());
    std::vector< trimesh::edge_t > edges;
    
    triangles = ply.getFaces();
    vertices = ply.getVertices();
    trimesh::unordered_edges_from_triangles( triangles.size(), &triangles[0], edges );

    std::cout << "vertices count: " << vertices.size() << std::endl;
    std::cout << "faces count: " << triangles.size() << std::endl;
    std::cout << "edges count: " << edges.size() << std::endl;
    
    trimesh::trimesh_t mesh;
    mesh.build(vertices.size(), &vertices[0], triangles.size(), &triangles[0], edges.size(), &edges[0]);

    // Use 'mesh' to walk the connectivity.
    std::vector< trimesh::index_t > neighs;
    for (int vi = 0; vi < vertices.size(); ++vi)
    {
        mesh.vertex_vertex_neighbors( vi, neighs );
        
        std::cout << "neighbors of vertex " << vi << ": ";
        for( int i = 0; i < neighs.size(); ++i )
        {
            auto v = mesh.vertices_map()[neighs.at(i)];
            std::cout << ' ' << neighs.at(i) << "( " << v.x << ", " << v.y << ", " << v.z << " )";
        }
        std::cout << '\n';
    }

    return 0;
}
