#pragma once
#include "fileHelper.h"
#include "oldmesh.h"
#include "materialStorage.h"
#include "indexStructs.h"

void ReadObjFromFile(OldMesh* mesh, MaterialStorage* materials, std::string location, std::string fileName);
void BuildMesh(OldMesh* mesh, std::vector<FaceData>& tempFaces, MaterialStorage* tempMaterials, std::vector<IndVertex>& tempVertices, float scale);
void ReadMtlFromFile(MaterialStorage* materials, std::string mtlLoc);