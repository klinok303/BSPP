#include <iostream>
#include <utility>
#include <string>

#include "bsp.h"

BSP ParseBSP(std::vector<unsigned char> buffer)
{
    // Parse header 
    // DEBUG: display header
    int header = CHARS_TO_I32(buffer, 0);
    std::cout << "Header = " << header << std::endl;

    std::vector<std::pair<int, int>> lumpData;

    // Parse lumps offsets and lengths
    for (int i = 0; i < Lumps::LUMPS_LENGTH; i++)
    {
        int offset = CHARS_TO_I32(buffer, (i * 8) + 4);
        int lumpLength = CHARS_TO_I32(buffer, (i * 8) + 8);
        lumpData.push_back({offset, lumpLength});
    }

    // DEBUG: Lump Data Display
    for (int i = 0; i < lumpData.size(); i++) 
    {
        int lumpType = i;
        int offset = lumpData[i].first;
        int length = lumpData[i].second;
        std::cout << lumpType << ": " << offset << ", " << length << std::endl;
    }

    // Parse vertices
    std::pair<int, int> vertexData = lumpData[Lumps::VERTICES];
    std::vector<Vector3D> vertices;
    for (int offset = 0; offset < vertexData.second; offset += 12)
    {
        float x, y, z;
        memcpy(&x, &buffer[vertexData.first + offset], sizeof(x));
        memcpy(&y, &buffer[vertexData.first + offset + 4], sizeof(y));
        memcpy(&z, &buffer[vertexData.first + offset + 8], sizeof(z));
        vertices.push_back({x, y, z});
    }

    // Parse edges
    std::pair<int, int> edgeData = lumpData[Lumps::EDGES];
    std::vector<std::vector<short>> edges;
    for (int offset = 0; offset < edgeData.second; offset += 4)
    {
        short a = CHARS_TO_I16(buffer, offset);
        short b = CHARS_TO_I16(buffer, offset + 2);
        edges.push_back({a, b});
    }

    // Return bsp object
    BSP bsp{vertices, edges};
    return bsp;
}