#include "indexStructs.h"

// Returns the index of the given vertex id in the vertex list
int FindVertIndex(std::vector<IndVertex>& verts, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return -1;

    // Find the given index in the vertices
    for (int i = 0; i < verts.size(); i++) {
        if (verts[i].id == id)
            return i;
    }

    // If the id is not found, return -1
    return -1;
}

// Returns the index of the given vertex id in the vertex list
int FindVertIndex(std::vector<IndVec3>& verts, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return -1;

    // Find the given index in the vertices
    for (int i = 0; i < verts.size(); i++) {
        if (verts[i].id == id)
            return i;
    }

    // If the id is not found, return -1
    return -1;
}

// Returns the face with the given id
int FindFaceIndex(std::vector<FaceData>& faces, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return -1;

    // Find the given index in the vertices
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].id == id)
            return i;
    }

    // If the id is not found, return -1
    return -1;
}

// Returns the vertex with the given id
IndVertex FindVert(std::vector<IndVertex>& verts, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return IndVertex();

    // Find the given index in the vertices
    for (int i = 0; i < verts.size(); i++) {
        if (verts[i].id == id)
            return verts[i];
    }

    // If the id is not found, return -1
    return IndVertex();
}

// Returns the vertex with the given id
IndVec3 FindVert(std::vector<IndVec3>& verts, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return IndVec3();

    // Find the given index in the vertices
    for (int i = 0; i < verts.size(); i++) {
        if (verts[i].id == id)
            return verts[i];
    }

    // If the id is not found, return -1
    return IndVec3();
}

// Returns the vertex with the given id
FaceData FindFace(std::vector<FaceData>& faces, int id)
{
    // If id is invalid, return null
    if (id < 0)
        return FaceData();

    // Find the given index in the vertices
    for (int i = 0; i < faces.size(); i++) {
        if (faces[i].id == id)
            return faces[i];
    }

    // If the id is not found, return -1
    return FaceData();
}
