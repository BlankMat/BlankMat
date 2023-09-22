#include "objReader.h"

// Reads the mesh from the .obj (and potentially .mtl) file at the given location
// ------------------------------------------------------------------------------
void ReadObjFromFile(Mesh* mesh, MaterialStorage* materials, std::string location, std::string fileName)
{
    std::string objLoc = location + fileName + ".obj";
    std::string mtlLoc = "";

    // Generate vectors to store all data
    // Vector of vertices, which are a point
    std::vector<IndVec3> vertPosList;
    // Vector of vertex normals, which are a vec3
    std::vector<IndVec3> vertNormList;
    // Vector of vertex textures, which are a vec3
    std::vector<IndVec3> vertTextList;
    // Vector of vertices
    std::vector<IndVertex> builtVertexList;
    // Vector of faces, which are vectors of verts, which are vectors of potentially vert info, texture info, and normal info
    std::vector<FaceData> faceDataList;
    // Map of groups, with name tag and a vector of face indices
    std::unordered_map<std::string, std::vector<int>> tempGroups;

    // Read Object
    // -------------
    // Open the object
    std::ifstream objFile(objLoc);
    if (!objFile.is_open())
    {
        // If the given file could not be found, print an error message
        std::cout << "Error: Attempted to read mesh at location [" << objLoc << "], but no such file exists." << std::endl;
        return;
    }

    // Currently selected info
    std::string curGroup = "";
    std::string curMaterial = "default";
    int curShadingGroup = 0;

    // Index info
    int curVertIndex = 0;
    int curVertNormIndex = 0;
    int curVertTextIndex = 0;
    int curFaceIndex = 0;

    // Read each individual line of the file
    std::string line = "";
    while (std::getline(objFile, line))
    {
        // Store parsed data
        std::vector<std::string> tempParse;
        ParseStringByDelim(tempParse, line, " ");

        // Parse stored data
        // If only 0 or 1 tokens are stored, the line has no significance
        // Only interpret lines with more than 1 token
        if (tempParse.size() > 1) {
            // Check if line is a comment and skip it
            if (tempParse[0] == "#") {
                // Skip this line
                continue;
            }

            // Check if line is a group
            else if (tempParse[0] == "g" || tempParse[0] == "group") {
                curGroup = tempParse[1];

                // If the new group doesn't exist yet, add it
                std::unordered_map<std::string, std::vector<int>>::iterator iter = tempGroups.find(curGroup);
                if (iter == tempGroups.end()) {
                    std::vector<int> newList;
                    tempGroups.emplace(curGroup, newList);
                }
            }

            // Check if line is a smoothing group
            else if (tempParse[0] == "s") {
                // Turn off shading group
                if (tempParse[1] == "off") {
                    curShadingGroup = -1;
                }
                // Otherwise set the shading group number
                else {
                    curShadingGroup = std::stoi(tempParse[1]);
                }
            }

            // Check if line is a vertex
            else if (tempParse[0] == "v") {
                // Don't read an incomplete vertex
                if (tempParse.size() >= 4) {
                    vertPosList.push_back(IndVec3(curVertIndex, ReadVec3FromStrings(tempParse)));
                    curVertIndex++;
                }
            }

            // Check if line is a vertex normal
            else if (tempParse[0] == "vn") {
                // Don't read an incomplete normal
                if (tempParse.size() >= 4) {
                    vertNormList.push_back(IndVec3(curVertNormIndex, ReadVec3FromStrings(tempParse)));
                    curVertNormIndex++;
                }
            }

            // Check if line is a vertex texture
            else if (tempParse[0] == "vt") {
                // Read u
                float _u = std::stof(tempParse[1]);

                // Read v (default, 0)
                float _v = 0;
                if (tempParse.size() >= 3)
                    _v = std::stof(tempParse[2]);

                // Read w (default, 0)
                float _w = 0;
                if (tempParse.size() >= 4)
                    _w = std::stof(tempParse[3]);

                // Store results
                vertTextList.push_back(IndVec3(curVertTextIndex, glm::vec3(_u, _v, _w)));
                curVertTextIndex++;
            }

            // Check if line is a face
            else if (tempParse[0] == "f") {
                // Create a temporary face to store info into
                std::vector<IndVertex> tempFace;

                // Loop through the parse array and evaluate each vertex
                for (int i = 1; i < tempParse.size(); i++) {
                    std::vector<std::string> tempFaceData;
                    ParseStringByDelim(tempFaceData, tempParse[i], "/");

                    // Add all vertex data to the face (up to a max of 3 elements: coord, texture, normal)
                    std::vector<int> tempFaceVertData;
                    int vertPosIndex = -1;
                    for (int j = 0; j < std::min((int)tempFaceData.size(), 3); j++) {
                        // If the face vertex data is not empty, store it as an int
                        if (tempFaceData[j] != "") {
                            tempFaceVertData.push_back(std::stoi(tempFaceData[j]) - 1);
                        }
                        // If the face vertex data is empty, save it as -1 (no texture/normal)
                        else {
                            tempFaceVertData.push_back(-1);
                        }
                    }

                    // Find vertex position from face
                    if (tempFaceVertData.size() > 0) {
                        vertPosIndex = tempFaceVertData[0];
                    }

                    // Find vertex
                    int vertIndex = FindVertIndex(builtVertexList, vertPosIndex);
                    if (vertIndex < 0 && vertPosIndex >= 0) {
                        glm::vec3 vertPos = glm::vec3();
                        glm::vec3 vertNorm = glm::vec3();
                        glm::vec3 vertText = glm::vec3();

                        if (tempFaceVertData.size() > 0)
                            vertPos = FindVert(vertPosList, tempFaceVertData[0]).pos;
                        if (tempFaceVertData.size() > 1)
                            vertText = FindVert(vertTextList, tempFaceVertData[1]).pos;
                        if (tempFaceVertData.size() > 2)
                            vertNorm = FindVert(vertNormList, tempFaceVertData[2]).pos;

                        Vertex newVert = Vertex(vertPos, vertNorm, vertText);
                        builtVertexList.push_back(IndVertex(vertPosIndex, newVert));
                    }

                    vertIndex = FindVertIndex(builtVertexList, tempFaceVertData[0]);
                    if (vertIndex >= 0)
                        tempFace.push_back(IndVertex(builtVertexList[vertIndex]));
                }

                // Push face to the face storage
                faceDataList.push_back(FaceData(tempFace, curMaterial, curShadingGroup, curFaceIndex));

                // Add face index to its group
                std::unordered_map<std::string, std::vector<int>>::iterator iter = tempGroups.find(curGroup);
                if (iter != tempGroups.end()) {
                    iter->second.push_back(curFaceIndex);
                }

                // Increment face index
                curFaceIndex++;
            }

            // Check if line is a material

            else if (tempParse[0] == "mtllib") {
                mtlLoc = location + tempParse[1];
            }

            // Check if line picks a material
            else if (tempParse[0] == "usemtl") {
                curMaterial = tempParse[1];

                // Add material to all elements in the selected group
                std::unordered_map<std::string, std::vector<int>>::iterator iter = tempGroups.find(curGroup);
                if (iter != tempGroups.end()) {
                    // Set the material of each face in the group to the selected material
                    for (int i = 0; i < iter->second.size(); i++) {
                        int faceIndex = FindFaceIndex(faceDataList, iter->second[i]);
                        if (faceIndex >= 0)
                            faceDataList[faceIndex].matName = curMaterial;
                    }
                }
            }
        }
    }
    objFile.close();


    // Read Material
    // -------------
    // Don't read material file if it is not requested
    if (mtlLoc != "") {
        ReadMtlFromFile(materials, mtlLoc);
    }

    // Construct mesh
    // --------------
    // Clear previous mesh
    mesh->Clear();

    // Find the maximum size of vertices
    float maxSize = INT_MIN;
    for (int i = 0; i < vertPosList.size(); i++) {
        if (vertPosList[i].pos.x > maxSize)
            maxSize = vertPosList[i].pos.x;
        if (vertPosList[i].pos.y > maxSize)
            maxSize = vertPosList[i].pos.y;
        if (vertPosList[i].pos.z > maxSize)
            maxSize = vertPosList[i].pos.z;
    }
    float meshScale = 1 / maxSize;
    std::cout << "Scaling object down by factor of " << meshScale << ". Largest pos: " << maxSize << std::endl;
    mesh->SetScale(glm::vec3(1.0f));
    mesh->SetName(fileName);

    // Build faces from vertices
    // Build indexed mesh
    BuildMesh(mesh, faceDataList, materials, builtVertexList, meshScale);

    // If no normals were read, recalculate normals for the mesh
    if (vertNormList.size() <= 0)
        mesh->RecalculateNormals();

    std::cout << "Successfully read object: " << fileName << "." << std::endl;
    std::cout << "Faces: [" << faceDataList.size() << "], Indices: [" << mesh->GetIndexCount() << "], Verts: [" << builtVertexList.size() << "]" << std::endl;
}

// Builds an indexed triangle mesh from the provided data
void BuildMesh(Mesh* mesh, std::vector<FaceData>& tempFaces, MaterialStorage* tempMaterials, std::vector<IndVertex>& tempVertices, float scale)
{
    // Build faces from vertices
    for (int faceIndex = 0; faceIndex < tempFaces.size(); faceIndex++) {
        FaceData faceData = tempFaces[faceIndex];
        std::string currentMat = faceData.matName;
        std::vector<IndVertex> faceVertices;

        for (int vertexIndex = 0; vertexIndex < faceData.vertexInfo.size(); vertexIndex++) {
            IndVertex vertex = faceData.vertexInfo[vertexIndex];
            vertex.ver.pos *= scale;

            faceVertices.push_back(vertex);
            mesh->AddVert(vertex.id, vertex.ver);
        }

        mesh->AddFace(Face(faceVertices, currentMat, faceData.shadingGroup));
    }
}

void ReadMtlFromFile(MaterialStorage* materials, std::string mtlLoc)
{
    // Open the material, exit if it can't be opened
    std::ifstream mtlFile(mtlLoc);
    if (!mtlFile.is_open()) {
        std::cout << "Error: Attempted to read material at location [" << mtlLoc << "], but no such file exists." << std::endl;
        return;
    }

    // Store current material information
    std::string curMatName = "";
    Material mat = Material();

    // Read each individual line of the file
    std::string line = "";
    while (std::getline(mtlFile, line))
    {
        // Store parsed data
        std::vector<std::string> tempParse;
        ParseStringByDelim(tempParse, line, " ");

        // Parse stored data
        // If only 0 or 1 tokens are stored, the line has no significance
        // Only interpret lines with more than 1 token
        if (tempParse.size() > 1) {
            // Check if line is a comment and skip it
            if (tempParse[0] == "#") {
                // Skip this line
                continue;
            }

            // Check if line is newmtl
            else if (tempParse[0] == "newmtl") {
                // If the current material is not blank, store it in the map
                if (curMatName != "") {
                    materials->AddMat(curMatName, mat);
                }

                mat.Reset();
                curMatName = tempParse[1];
            }

            // Check if line is Ka
            else if (tempParse[0] == "Ka") {
                // Don't read an incomplete vector
                if (tempParse.size() >= 4) {
                    mat.ka = ReadVec3FromStrings(tempParse);
                }
            }

            // Check if line is Kd
            else if (tempParse[0] == "Kd") {
                // Don't read an incomplete vector
                if (tempParse.size() >= 4) {
                    mat.kd = ReadVec3FromStrings(tempParse);
                }
            }

            // Check if line is Ks
            else if (tempParse[0] == "Ks") {
                // Don't read an incomplete vector
                if (tempParse.size() >= 4) {
                    mat.ks = ReadVec3FromStrings(tempParse);
                }
            }

            // Check if line is Ns
            else if (tempParse[0] == "Ns") {
                mat.ns = std::stof(tempParse[1]);
            }

            // Check if line is Ni
            else if (tempParse[0] == "Ni") {
                mat.ni = std::stof(tempParse[1]);
            }

            // Check if line is d
            else if (tempParse[0] == "d") {
                mat.d = std::stof(tempParse[1]);
            }

            // Check if line is Tr
            else if (tempParse[0] == "Tr") {
                mat.d = 1 - std::stof(tempParse[1]);
            }
        }
    }
    materials->AddMat(curMatName, mat);
    mtlFile.close();
}