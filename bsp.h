#ifndef BSP_H
#define BSP_H

#include <vector>
#include <cstring>

#define CHARS_TO_I32(raw, shift) (raw[(shift + 3)] << 24 | raw[(shift) + 2] << 16 | raw[(shift) + 1] << 8 | raw[(shift)])
#define CHARS_TO_I16(raw, shift) (raw[(shift) + 2] << 16 | raw[(shift) + 1] << 8 | raw[(shift)])

enum Lumps
{
    ENTITIES = 0,
    PLANES,
    TEXTURES,
    VERTICES,
    VISIBILITY,
    NODES,
    TEXINFO,
    FACES,
    LIGHTING,
    CLIPNODES,
    LEAVES,
    MARKSURFACES,
    EDGES,
    SURFEDGES,
    MODELS,
    HEADER_LUMPS,
    LUMPS_LENGTH
};

struct Vector3D { float x, y, z; };
struct BSP 
{
    std::vector<Vector3D> vertices;
    std::vector<std::vector<short>> edges;
};

BSP parseBSP(std::vector<unsigned char> buffer);

#endif /* BSP_H */
