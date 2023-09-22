#pragma once
#include "glIncludes.h"

glm::vec4 RotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix);
bool IsCCW(glm::vec3 a, glm::vec3 b, glm::vec3 c);