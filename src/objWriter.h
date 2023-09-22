#pragma once
#include "scene.h"
#include "fileHelper.h"

void WriteObjToFile(Scene* scene, std::string location, std::string fileName);
void WriteMtlToFile(MaterialStorage* mats, std::string location, std::string fileName);