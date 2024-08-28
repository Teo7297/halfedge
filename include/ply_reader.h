#ifndef PLYREADER_H
#define PLYREADER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "trimesh_types.h"

using namespace trimesh;

class PlyReader
{
public:
    // Constructor that takes the filename
    PlyReader(const std::string &filename)
    {
        loadPlyFile(filename);
    }

    void savePlyFile(const std::string &filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open the file " << filename << " for writing." << std::endl;
            return;
        }

        // Write the PLY header
        file << "ply\n";
        file << "format ascii 1.0\n";
        file << "element vertex " << vertices.size() << "\n";
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
        file << "element face " << faces.size() << "\n";
        file << "property list uchar int vertex_indices\n";
        file << "end_header\n";

        // Write vertex data
        for (const auto &vertex : vertices)
        {
            file << vertex.x << " " << vertex.y << " " << vertex.z << " "
                 << static_cast<int>(vertex.r) << " " << static_cast<int>(vertex.g) << " " << static_cast<int>(vertex.b) << " "
                 << vertex.nx << " " << vertex.ny << " " << vertex.nz << " "
                 << vertex.curvature << "\n";
        }

        // Write face data
        for (const auto &face : faces)
        {
            file << 3;
            for (int index : face.v)
            {
                file << " " << index;
            }
            file << "\n";
        }

        file.close();
    }

    // Getters for vertices and faces
    const std::vector<vertex_t> &getVertices() const { return vertices; }
    const std::vector<triangle_t> &getFaces() const { return faces; }

private:
    std::vector<vertex_t> vertices;
    std::vector<triangle_t> faces;

    void loadPlyFile(const std::string &filename)
    {
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
                faces.push_back(face);
                faceCount--;
            }
        }

        file.close();
    }
};

#endif // PLYREADER_H
