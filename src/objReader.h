#pragma once
#include "fileHelper.h"
#include "mesh.h"
#include "materialStorage.h"
#include "indexStructs.h"

void ReadObjFromFile(Mesh* mesh, MaterialStorage* materials, std::string location, std::string fileName);
void BuildMesh(Mesh* mesh, std::vector<FaceData>& tempFaces, MaterialStorage* tempMaterials, std::vector<IndVertex>& tempVertices, float scale);
void ReadMtlFromFile(MaterialStorage* materials, std::string mtlLoc);