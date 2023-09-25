#include "mathLib.h"

// Taken from https://gamedev.stackexchange.com/questions/115032/how-should-i-rotate-vertices-around-the-origin-on-the-cpu
glm::vec4 RotateAround(glm::vec4 aPointToRotate, glm::vec4 aRotationCenter, glm::mat4x4 aRotationMatrix)
{
	glm::mat4x4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(aRotationCenter.x, aRotationCenter.y, aRotationCenter.z));
	glm::mat4x4 invTranslate = glm::inverse(translate);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location

	glm::mat4x4 transform = translate * aRotationMatrix * invTranslate;

	return transform * aPointToRotate;
}

bool IsCCW(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) > 0;
}

glm::vec3 Vec3FromAssimp(aiVector3D vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec2 Vec2FromAssimp(aiVector2D vec)
{
	return glm::vec2(vec.x, vec.y);
}

glm::vec2 Vec2FromAssimp(aiVector3D vec)
{
	return glm::vec2(vec.x, vec.y);
}