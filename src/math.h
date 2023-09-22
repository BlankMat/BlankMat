#pragma once
#include "glIncludes.h"

glm::vec4 RotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix);
bool IsCCW(glm::vec3 a, glm::vec3 b, glm::vec3 c);
glm::vec3 Vec3FromAssimp(aiVector3D vec);
glm::vec2 Vec2FromAssimp(aiVector2D vec);
glm::vec2 Vec2FromAssimp(aiVector3D vec);