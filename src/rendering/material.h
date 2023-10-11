#pragma once
#include "iMaterial.h"
#include "texture.h"
#include "tools/state.h"
#include "files/config.h"

struct Material : public IMaterial<Texture*>
{
protected:
	std::vector<Texture*> mTextures;
public:
    // Updates the given shader with this material's properties
	void UpdateShader(Shader* shader, State* state, Material* defaultMat)
	{
        shader->SetVec3("material.diffuse", kd);
        shader->SetVec3("material.ambient", ka);
        shader->SetVec3("material.specular", ks);
		shader->SetVec3("material.emissive", ke);
		shader->SetFloat("material.shininess", ns);
		shader->SetFloat("material.refraction", ni);
		shader->SetFloat("material.alpha", d);
		shader->SetInt("material.mode", illum);
		
		// Bind all textures
        unsigned int diffuseNum = 1;
        unsigned int ambientNum = 1;
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
            std::string name = mTextures[i]->type;
            unsigned int loadID = 0;
            if (name == "texture_diffuse")
            {
                number = std::to_string(diffuseNum++);
                loadID = (state->enableDiffuseMap) ? mTextures[i]->id : defaultMat->map_kd->id;
                shader->SetVec3("material.diffuse", (state->enableDiffuseMap) ? glm::vec3(1.0f) : kd);
            }
            else if (name == "texture_ambient")
            {
                number = std::to_string(ambientNum++);
                loadID = (state->enableAmbientMap) ? mTextures[i]->id : defaultMat->map_ka->id;
                shader->SetVec3("material.ambient", (state->enableAmbientMap) ? glm::vec3(1.0f) : ka);
            }
            else if (name == "texture_specular")
            {
                number = std::to_string(specularNum++);
                loadID = (state->enableSpecularMap) ? mTextures[i]->id : defaultMat->map_ks->id;
                shader->SetVec3("material.specular", (state->enableSpecularMap) ? glm::vec3(1.0f) : ks);
            }
            else if (name == "texture_normal")
            {
                number = std::to_string(normalNum++);
                loadID = (state->enableNormalMap) ? mTextures[i]->id : defaultMat->map_bump->id;
            }
            else if (name == "texture_height")
            {
                number = std::to_string(heightNum++);
                loadID = (state->enableHeightMap) ? mTextures[i]->id : defaultMat->map_ns->id;
            }
            else if (name == "texture_alpha")
            {
                number = std::to_string(alphaNum++);
                loadID = (state->enableAlphaMap) ? mTextures[i]->id : defaultMat->map_d->id;
            }

            // Set the sampler to the correct texture unit
            shader->SetInt(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, loadID);
        }
        glActiveTexture(GL_TEXTURE0);
	}

    Material(Config* config, Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
        Texture* _map_bump, Texture* _map_ns, Texture* _map_d)
    {
        // Load values from config
        kd = config->GetVec("kd");
        ka = config->GetVec("ka");
        ks = config->GetVec("ks");
        ke = config->GetVec("ke");
        ns = config->GetFloat("ns");
        ni = config->GetFloat("ni");
        d = config->GetFloat("d");
        illum = config->GetInt("illum");

        // Load textures from pointers
        map_kd = _map_kd;
        map_ka = _map_ka;
        map_ks = _map_ks;
        map_bump = _map_bump;
        map_ns = _map_ns;
        map_d = _map_d;

        // Push textures to list
        mTextures.push_back(map_kd);
        mTextures.push_back(map_ka);
        mTextures.push_back(map_ks);
        mTextures.push_back(map_bump);
        mTextures.push_back(map_ns);
        mTextures.push_back(map_d);
    }

	Material(Texture* _map_kd, Texture* _map_ka, Texture* _map_ks,
        Texture* _map_bump, Texture* _map_ns, Texture* _map_d,
        glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
		float _ns = 0, float _ni = 1, float _d = 1,
        glm::vec3 _ke = glm::vec3(), int _illum = 2)
		: IMaterial(_ka, _kd, _ks, _ns, _ni, _d, _ke, _illum)
    {
        map_kd = _map_kd;
        map_ka = _map_ka;
        map_ks = _map_ks;
        map_bump = _map_bump;
        map_ns = _map_ns;
        map_d = _map_d;
        mTextures.push_back(map_kd);
        mTextures.push_back(map_ka);
        mTextures.push_back(map_ks);
        mTextures.push_back(map_bump);
        mTextures.push_back(map_ns);
        mTextures.push_back(map_d);
    }

    Material(std::vector<Texture*>& _map_kd, std::vector<Texture*>& _map_ka, std::vector<Texture*>& _map_ks,
        std::vector<Texture*>& _map_bump, std::vector<Texture*>& _map_ns, std::vector<Texture*>& _map_d,
        glm::vec3 _ka = glm::vec3(), glm::vec3 _kd = glm::vec3(), glm::vec3 _ks = glm::vec3(),
        float _ns = 0, float _ni = 1, float _d = 1,
        glm::vec3 _ke = glm::vec3(), int _illum = 2)
        : IMaterial(_ka, _kd, _ks, _ns, _ni, _d, _ke, _illum)
    {
        map_kd = _map_kd[0];
        map_ka = _map_ka[0];
        map_ks = _map_ks[0];
        map_bump = _map_bump[0];
        map_ns = _map_ns[0];
        map_d = _map_d[0];
        mTextures.insert(mTextures.end(), _map_kd.begin(), _map_kd.end());
        mTextures.insert(mTextures.end(), _map_ka.begin(), _map_ka.end());
        mTextures.insert(mTextures.end(), _map_ks.begin(), _map_ks.end());
        mTextures.insert(mTextures.end(), _map_bump.begin(), _map_bump.end());
        mTextures.insert(mTextures.end(), _map_ns.begin(), _map_ns.end());
        mTextures.insert(mTextures.end(), _map_d.begin(), _map_d.end());
    }
};