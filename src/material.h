#pragma once
#include "iMaterial.h"
#include "texture.h"

struct Material : public IMaterial<Texture>
{
protected:
	std::vector<Texture> mTextures;
public:
    // Updates the given shader with this material's properties
	void UpdateShader(Shader* shader) override
	{
		shader->SetVec3("material.ambient", ka);
		shader->SetVec3("material.diffuse", kd);
		shader->SetVec3("material.specular", ks);
		shader->SetVec3("material.emissive", ke);
		shader->SetFloat("material.shininess", ns);
		shader->SetFloat("material.refraction", ni);
		shader->SetFloat("material.alpha", d);
		shader->SetInt("material.mode", illum);
		
		// Bind all textures
        unsigned int diffuseNum = 1;
        unsigned int specularNum = 1;
        unsigned int normalNum = 1;
        unsigned int heightNum = 1;
        unsigned int alphaNum = 1;
        for (unsigned int i = 0; i < mTextures.size(); i++)
        {
            // Activate proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);
            // Retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = mTextures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNum++);
            else if (name == "texture_specular")
                number = std::to_string(specularNum++);
            else if (name == "texture_normal")
                number = std::to_string(normalNum++);
            else if (name == "texture_height")
                number = std::to_string(heightNum++);
            else if (name == "texture_alpha")
                number = std::to_string(alphaNum++);

            // Set the sampler to the correct texture unit
            shader->SetInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);
	}

    // Add all textures to texture list
    void LoadTextures()
    {
        mTextures.clear();
        mTextures.push_back(map_kd);
        mTextures.push_back(map_ka);
        mTextures.push_back(map_ks);
        mTextures.push_back(map_bump);
        mTextures.push_back(map_ns);
        mTextures.push_back(map_d);
    }

	Material(Texture defaultTexture, 
        glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1,
		Texture _map_kd = Texture(0, "texture_diffuse", ""), 
        Texture _map_ka = Texture(0, "texture_ambient", ""),
        Texture _map_ks = Texture(0, "texture_specular", ""),
        Texture _map_bump = Texture(0, "texture_normal", ""),
		Texture _map_ns = Texture(0, "texture_height", ""),
        Texture _map_d = Texture(0, "texture_alpha", ""),
        glm::vec3 _ke = glm::vec3(), int _illum = 2)
		: IMaterial(_ka, _kd, _ks, _ns, _ni, _d, _ke, _illum)
    {
        map_kd = _map_kd;
        map_ka = _map_ka;
        map_ks = _map_ks;
        map_bump = _map_bump;
        map_ns = _map_ns;
        map_d = _map_d;

        // If textures were not provided, use default texture
        if (map_kd.id == 0)
            map_kd = Texture(defaultTexture.id, "texture_diffuse", defaultTexture.path);
        if (map_ka.id == 0)
            map_ka = Texture(defaultTexture.id, "texture_ambient", defaultTexture.path);
        if (map_ks.id == 0)
            map_ks = Texture(defaultTexture.id, "texture_specular", defaultTexture.path);
        if (map_bump.id == 0)
            map_bump = Texture(defaultTexture.id, "texture_normal", defaultTexture.path);
        if (map_ns.id == 0)
            map_ns = Texture(defaultTexture.id, "texture_height", defaultTexture.path);
        if (map_d.id == 0)
            map_d = Texture(defaultTexture.id, "texture_alpha", defaultTexture.path);
        LoadTextures();
    }

    Material(IMaterial<std::string>* material)
        : IMaterial(material->ka, material->kd, material->ks, material->ns, material->ni, material->d, material->ke, material->illum)
    {
        // TODO: Load material textures
        LoadTextures();
    }
};