#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "trimesh_types.h"
#include "trimesh.h"

class PlyReader
{
public:

    static void loadPlyFile(const std::string& filename, trimesh::trimesh_t& outMesh)
    {
        using namespace trimesh;

        std::vector<vertex_t> vertices;
        std::vector<triangle_t> triangles;
        std::vector<edge_t> edges;

        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open the file " << filename << std::endl;
            return;
        }

        std::string line;
        bool header = true;
        int vertexCount = 0, faceCount = 0;
        bool hasColor = false, hasNormals = false, hasCurvature = false;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);

            if (header)
            {
                if (line.find("element vertex") != std::string::npos)
                {
                    iss.ignore(15);
                    iss >> vertexCount;
                }
                else if (line.find("element face") != std::string::npos)
                {
                    iss.ignore(13);
                    iss >> faceCount;
                }
                else if (line.find("property uchar red") != std::string::npos ||
                         line.find("property uchar green") != std::string::npos ||
                         line.find("property uchar blue") != std::string::npos)
                {
                    hasColor = true;
                }
                else if (line.find("property float nx") != std::string::npos ||
                         line.find("property float ny") != std::string::npos ||
                         line.find("property float nz") != std::string::npos)
                {
                    hasNormals = true;
                }
                else if (line.find("property float curvature") != std::string::npos)
                {
                    hasCurvature = true;
                }
                else if (line == "end_header")
                {
                    header = false;
                }
            }
            else if (vertexCount > 0)
            {
                vertex_t vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;

                if (hasColor)
                {
                    int r, g, b;
                    iss >> r >> g >> b;
                    vertex.r = static_cast<unsigned char>(r);
                    vertex.g = static_cast<unsigned char>(g);
                    vertex.b = static_cast<unsigned char>(b);
                }

                if (hasNormals)
                {
                    iss >> vertex.nx >> vertex.ny >> vertex.nz;
                }

                if (hasCurvature)
                {
                    iss >> vertex.curvature;
                }

                vertices.push_back(vertex);
                vertexCount--;
            }
            else if (faceCount > 0)
            {
                triangle_t face;
                int vertexIndices;
                iss >> vertexIndices; // number of vertices in this face
                for (int i = 0; i < vertexIndices; i++)
                {
                    int index;
                    iss >> index;
                    face.v[i] = index;
                }
                triangles.push_back(face);
                faceCount--;
            }
        }

        file.close();

        trimesh::unordered_edges_from_triangles(triangles.size(), &triangles[0], edges);

        outMesh.build(vertices.size(), &vertices[0], triangles.size(), &triangles[0], edges.size(), &edges[0]);
    }

    static void savePlyFile(const std::string &filename, const trimesh::trimesh_t& mesh)
    {
        using namespace trimesh;

        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open the file " << filename << " for writing." << std::endl;
            return;
        }

        // Write the PLY header
        file << "ply\n";
        file << "format ascii 1.0\n";
        file << "element vertex " << mesh.vertices().size() << "\n";
        file << "property float x\n";
        file << "property float y\n";
        file << "property float z\n";
        file << "property uchar red\n";
        file << "property uchar green\n";
        file << "property uchar blue\n";
        file << "property float nx\n";
        file << "property float ny\n";
        file << "property float nz\n";
        file << "property float curvature\n";
        file << "element face " << mesh.triangles().size() << "\n";
        file << "property list uchar int vertex_indices\n";
        file << "end_header\n";

        // Write vertex data
        for (const auto& [id, vertex] : mesh.vertices_data())
        {        
            file 
                << vertex.x << " "
                << vertex.y << " "
                << vertex.z << " "
                << static_cast<int>(vertex.r) << " "
                << static_cast<int>(vertex.g) << " "
                << static_cast<int>(vertex.b) << " "
                << vertex.nx << " "
                << vertex.ny << " "
                << vertex.nz << " "
                << vertex.curvature << "\n";
        }

        // Write face data
        auto triangles = mesh.triangles();
        for (const auto &triangle : triangles)
        {
            auto he1 = mesh.halfedge(triangle);
            auto he2 = mesh.halfedge(he1.next_he);
            auto he3 = mesh.halfedge(he2.next_he);
            file << 3;
            file << " " << he1.to_vertex;
            file << " " << he2.to_vertex;
            file << " " << he3.to_vertex;
            file << "\n";
        }

        file.close();
    }
};