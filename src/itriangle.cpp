#include "itriangle.h"
#include "ray.h"

void ITriangle::SetMat(std::string _mat)
{
	mat = _mat;
}

void ITriangle::CalcNormal()
{
	normal = glm::normalize(glm::cross(vertices[1].ver.pos - vertices[0].ver.pos, vertices[2].ver.pos - vertices[0].ver.pos));
	center = (vertices[0].ver.pos + vertices[1].ver.pos + vertices[2].ver.pos) / 3.0f;
}

void ITriangle::Reorient(std::vector<ITriangle>& tris)
{
	// Don't reorient confirmed correct tris
	if (tag == TriTag::CONFIRMED_CORRECT)
		return;

	// Shoot ray just off of the surface of the tri into the whole mesh
	Ray normRay = Ray(center + normal * 0.0001f, normal);

	// If there are odd intersections, flip the tri
	if (normRay.GetNumIntersects(tris) % 2 != 0) {
		IndVertex tempVert = vertices[1];
		vertices[1] = vertices[2];
		vertices[2] = tempVert;
		CalcNormal();
	}
	tag = TriTag::CONFIRMED_CORRECT;
}

ITriangle::ITriangle()
{
	mat = "default";
	shadingGroup = -1;
	vertices[0] = IndVertex(-1, Vertex());
	vertices[1] = IndVertex(-1, Vertex());
	vertices[2] = IndVertex(-1, Vertex());
	CalcNormal();
}

ITriangle::ITriangle(IndVertex i0, IndVertex i1, IndVertex i2, std::string _mat, int _sg)
{
	mat = _mat;
	shadingGroup = _sg;
	vertices[0] = i0;
	vertices[1] = i1;
	vertices[2] = i2;
	CalcNormal();
}

ITriangle::ITriangle(Triangle _tri, std::unordered_map<int, Vertex>& _verts, int _triIndex, int _meshIndex, TriTag _tag)
{
	tag = _tag;
	triIndex = _triIndex;
	meshIndex = _meshIndex;
	mat = _tri.mat;
	shadingGroup = _tri.shadingGroup;
	vertices[0] = IndVertex(_tri.vertices[0], _verts[_tri.vertices[0]]);
	vertices[1] = IndVertex(_tri.vertices[1], _verts[_tri.vertices[1]]);
	vertices[2] = IndVertex(_tri.vertices[2], _verts[_tri.vertices[2]]);
	CalcNormal();
}