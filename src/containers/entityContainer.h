#pragma once
#include "interfaces/iEntity.h"
#include "interfaces/iContainer.h"

class EntityContainer : public IContainer<IEntity>
{
public:
	// Draws the contents of the container
	void Draw(Shader* shader, State* state, Material* defaultMat, glm::mat4 viewProj, bool drawMats = false)
	{
		for (auto iter = mData.begin(); iter != mData.end(); ++iter)
		{
			iter->second->Draw(shader, state, defaultMat, viewProj, drawMats);
		}
	}
};