#include "light.h"

Light::Light(glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _color, float _ka, float _ks)
	: pos(_pos), dir(_dir), color(_color), ka(_ka), ks(_ks) {}