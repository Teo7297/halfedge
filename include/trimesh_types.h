#pragma once

#include <map>

namespace trimesh
{
    typedef long index_t;

    struct edge_t
    {
        index_t v[2];
        
        index_t& start() { return v[0]; }
        const index_t& start() const { return v[0]; }
        
        index_t& end() { return v[1]; }
        const index_t& end() const { return v[1]; }
        
        edge_t()
        {
            v[0] = v[1] = -1;
        }
    };
    
    struct triangle_t
    {
        index_t v[3];
        
        index_t& i() { return v[0]; }
        const index_t& i() const { return v[0]; }
        
        index_t& j() { return v[1]; }
        const index_t& j() const { return v[1]; }
        
        index_t& k() { return v[2]; }
        const index_t& k() const { return v[2]; }
        
        triangle_t()
        {
            v[0] = v[1] = v[2] = -1;
        }
    };

    struct vertex_t
    {
        float x, y, z;
        unsigned char r,g,b;
        float nx, ny, nz;
        float curvature;

        vertex_t() : 
        x{0.f}, y{0.f}, z{0.f},
        r{'0'}, g{'0'}, b{'0'}, 
        nx{0.f}, ny{0.f}, nz{ 0.f }, 
        curvature{0.f}{}
    };

    struct halfedge_t
    {
        // Index into the vertex array.
        index_t to_vertex;
        // Index into the face array.
        index_t face;
        // Index into the edges array.
        index_t edge;
        // Index into the halfedges array.
        index_t opposite_he;
        // Index into the halfedges array.
        index_t next_he;

        halfedge_t() : to_vertex(-1),
                       face(-1),
                       edge(-1),
                       opposite_he(-1),
                       next_he(-1)
        {
        }
    };

    typedef std::map<index_t, vertex_t> vertices_data_map;
}
