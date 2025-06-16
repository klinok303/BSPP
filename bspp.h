#ifndef BSPP_H
#define BSPP_H

#include <stdint.h>

typedef uint16_t bspp_edge[2];

typedef enum {
    BSPP_LUMP_TYPE_ENTITIES = 0,
    BSPP_LUMP_TYPE_PLANES,
    BSPP_LUMP_TYPE_MIPTEX,
    BSPP_LUMP_TYPE_VERTICES,
    BSPP_LUMP_TYPE_VISIBILITY,
    BSPP_LUMP_TYPE_NODES,
    BSPP_LUMP_TYPE_TEXINFO,
    BSPP_LUMP_TYPE_FACES,
    BSPP_LUMP_TYPE_LIGHTMAPS,
    BSPP_LUMP_TYPE_CLIPNODES,
    BSPP_LUMP_TYPE_LEAVES,
    BSPP_LUMP_TYPE_LFACE,
    BSPP_LUMP_TYPE_EDGES,
    BSPP_LUMP_TYPE_LEDGES,
    BSPP_LUMP_TYPE_MODELS,
    BSPP_LUMP_TYPES_COUNT,
} bspp_lump_type;

typedef struct {
    float x, y, z;
} bspp_vec3;

typedef struct {
    bspp_vec3 min;
    bspp_vec3 max;
} bspp_bbox;

typedef struct {
    int16_t min;
    int16_t max;
} bspp_bbox32;

typedef struct {
    int32_t offset;
    int32_t size;
} bspp_lump;

typedef struct {
    int32_t id;
    bspp_lump entities;
    bspp_lump planes;
    bspp_lump miptex;
    bspp_lump vertices;
    bspp_lump visilist;
    bspp_lump nodes;
    bspp_lump texinfo;
    bspp_lump faces;
    bspp_lump lightmaps;
    bspp_lump clipnodes;
    bspp_lump leaves;
    bspp_lump lface;
    bspp_lump edges;
    bspp_lump ledges;
    bspp_lump models;
} bspp_header;

typedef struct {
    int32_t plane;
    uint16_t front;
    uint16_t back;
    bspp_bbox32 bbox;
    uint16_t face;
    uint16_t faces;
} bspp_node;

typedef struct {
    int32_t type;
    int32_t vislist;
    bspp_bbox32 bbox;
    uint16_t face;
    uint16_t faces;
    uint8_t ambient[4];
} bspp_leaf;

typedef struct {
    uint16_t plane;
    uint16_t side;
    int32_t first_edge;
    uint16_t edges;
    uint16_t styles;
    uint8_t typelight;
    uint8_t baselight;
    uint8_t light[2];
    int32_t lightmap;
} bspp_face;

typedef struct {
    bspp_vec3 normal;
    float dist;
    int32_t type;
} bspp_plane;

typedef struct {
    char* key;
    char* value;
} bspp_entity_property;

typedef struct {
    bspp_entity_property* properties;
    uint32_t count;
} bspp_entity;

typedef struct {
    bspp_bbox bbox;
    bspp_vec3 origin;
    int32_t nodes[4];
    int32_t numleafs;
    int32_t face_id;
    int32_t faces;
} bspp_model;

typedef struct {
    int32_t *offset;
    int32_t numtex;
} bspp_mipheader;

typedef struct {
    char name[16];
    uint32_t width;
    uint32_t height;
    uint32_t offset1; // offset to uint8_t pix[width   * height]
    uint32_t offset2; // offset to uint8_t pix[width/2 * height/2]
    uint32_t offset4; // offset to uint8_t pix[width/4 * height/4]
    uint32_t offset8; // offset to uint8_t pix[width/8 * height/8]
} bspp_miptex;

typedef struct {
    bspp_vec3 vectorS;   // S vector, horizontal in texture space
    float distS;         // horizontal offset in texture space
    bspp_vec3 vectorT;   // T vector, vertical in texture space
    float distT;         // vertical offset in texture space
    uint32_t texture_id; // Index of Mip Texture
    uint32_t animated;   // 0 for ordinary textures, 1 for water 
} bspp_surface;

typedef struct {
    uint8_t *light_map;
    uint32_t numlightmap;
} bspp_lightmap;

typedef struct { 
    uint32_t plane_id;
    int16_t front;
    int16_t back;
} bspp_clipnode;

typedef struct {
    uint16_t *faces;
    uint32_t numlface;
} bspp_lfaces;

typedef struct {
    uint16_t *edge;
    uint32_t numlstedge;
} bspp_lstedges;

typedef struct {
    bspp_header header;
    bspp_entity* entities;
    bspp_plane* planes;
    bspp_mipheader mipheader;
    bspp_miptex textures;
    bspp_vec3* vertices;
    uint8_t* visibility;
    bspp_node* nodes;
    bspp_surface* surfaces;
    bspp_face* faces;
    bspp_lightmap lighting;
    bspp_clipnode clipnodes;
    bspp_leaf* leaves;
    bspp_lfaces lfaces;
    bspp_edge* edges;
    bspp_lstedges ledges;
    bspp_model* models;
} bspp_bsp;

bspp_bsp* parse_bsp(const char* buffer);

#endif // BSPP_H

#ifdef BSPP_IMPLEMENTATION

void parse_header(bspp_header* header, const char* buffer) {
    header->id = *(int32_t*)buffer;
    for (int8_t i = 0; i < BSPP_LUMP_TYPES_COUNT; i++) {
        header->lumps[i].offset = *(int32_t*)(buffer + 4 + i * 8);
        header->lumps[i].size = *(int32_t*)(buffer + 8 + i * 8);
    }
    
}

void parse_bsp(bspp_bsp* bsp, const char* buffer) {
    if (!bsp) return;
    parse_header(&bsp->header, buffer);
}

#endif
