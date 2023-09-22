#include "input.h"
#include "openGLHelper.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
bool ProcessInput(GLFWwindow* window, Scene* scene, Selection* sel, InputLocks* locks, Options* options, SpeedConsts* speeds, float deltaTime, int* prevX, int* prevY)
{
    // TODO: Replace mesh reference with proper reference
    Camera* camera = scene->GetCamera();
    OldMesh* mesh = scene->GetMeshes()->GetAll().begin()->second;
    std::set<int> selVerts;
    sel->GetSelectedVerts(selVerts);
    bool didReceiveInput = false;

    // Get window width and height
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    double halfWidth = width * 0.5f;
    double halfHeight = height * 0.5f;



    /* ==================================================== Window Stuff ===================================================== */
    // Exit window
    if (ESC_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return true;
    }
    // Focus object
    if (F_PRESS && !CTRL_PRESS) {
        // Only focus when not locked
        if (!locks->lockF) {
            glm::vec3 targetDir = glm::normalize(mesh->GetPos() - camera->pos);
            float rotY = asin(targetDir.y);
            float rotX = atan2(targetDir.x, targetDir.z);
            camera->rotation = glm::vec3(rotX, rotY, 0.0f);
            camera->CalcBasis();

            locks->lockF = true;
        }
        didReceiveInput = true;
    }
    // Toggle wireframe
    if (KEY5_PRESS && !CTRL_PRESS) {
        if (!locks->lockKey5) {
            options->wireframe = (options->wireframe == 1 ? 0 : 1);
            OpenGLEnableWireframe(options->wireframe == 1);
            std::cout << "Turned wireframe " << (options->wireframe == 1 ? "on" : "off") << "\n";
            locks->lockKey5 = true;
        }
        didReceiveInput = true;
    }



    /* ==================================================== Tool Selection ===================================================== */
    // Select selection tool
    if (Q_PRESS && !CTRL_PRESS && !locks->lockQ) {
        sel->SetTool(Tool::SELECT);
        locks->LockTool(Tool::SELECT);
        didReceiveInput = true;
    }
    // Select scale tool
    else if (E_PRESS && !CTRL_PRESS && !locks->lockE) {
        sel->SetTool(Tool::SCALE);
        locks->LockTool(Tool::SCALE);
        didReceiveInput = true;
    }
    // Select extrude tool
    else if (E_PRESS && CTRL_PRESS && !locks->lockCtrlE) {
        sel->SetTool(Tool::EXTRUDE);
        locks->LockTool(Tool::EXTRUDE);
        didReceiveInput = true;
    }
    // Select rotate tool
    else if (R_PRESS && !CTRL_PRESS && !locks->lockR) {
        sel->SetTool(Tool::ROTATE);
        locks->LockTool(Tool::ROTATE);
        didReceiveInput = true;
    }
    // Select move tool
    else if (T_PRESS && !CTRL_PRESS && !locks->lockT) {
        sel->SetTool(Tool::MOVE);
        locks->LockTool(Tool::MOVE);
        didReceiveInput = true;
    }
    // Select mesh selection mode
    if (KEY1_PRESS && !CTRL_PRESS) {
        if (sel->GetSelMode() != SelMode::MESH)
            sel->SetSelMode(SelMode::MESH);
        didReceiveInput = true;
    }
    // Select vertex selection mode
    else if (KEY2_PRESS && !CTRL_PRESS) {
        if (sel->GetSelMode() != SelMode::VERT)
            sel->SetSelMode(SelMode::VERT);
        didReceiveInput = true;
    }
    // Select face selection mode
    else if (KEY3_PRESS && !CTRL_PRESS) {
        if (sel->GetSelMode() != SelMode::FACE)
            sel->SetSelMode(SelMode::FACE);
        didReceiveInput = true;
    }



    /* ==================================================== Keyboard Input ===================================================== */
    /* =================================== Handle movement input ================================== */
    if (WASDZX_PRESS && !CTRL_PRESS) {
        glm::vec3 inputVec = GetWASDZX(window);

        // Only move camera on ALT
        if (ALT_PRESS) {
            camera->pos += inputVec.x * camera->dir * deltaTime * speeds->cameraMoveSpeed;
            camera->pos += inputVec.y * camera->right * deltaTime * speeds->cameraMoveSpeed;
            camera->pos += inputVec.z * camera->up * deltaTime * speeds->cameraMoveSpeed;
        }
        // Handle tools on any other input
        else {
            switch (sel->GetTool()) {
                /* ========== Handle extruding ============== */
            case Tool::EXTRUDE:
                // TODO:
                // Add extrude tool
                break;
                /* ========== Handle moving ================= */
            case Tool::MOVE:
                glm::vec3 moveVec = inputVec.x * mesh->GetForward() + inputVec.y * mesh->GetRight() + inputVec.z * mesh->GetUp();
                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    mesh->Translate(moveVec * deltaTime * speeds->modelMoveSpeed);
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    mesh->Translate(selVerts, moveVec * deltaTime * speeds->modelMoveSpeed);
                    locks->rerender = true;
                    break;
                default:
                    break;
                }
                break;
                /* ========== Handle scaling ================= */
            case Tool::SCALE:
                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    if (SHIFT_PRESS)
                        mesh->SetScale(mesh->GetScale() + glm::clamp(inputVec.x + inputVec.y + inputVec.z, -1.0f, 1.0f) * speeds->modelScaleRate * deltaTime * mesh->GetScale());
                    else
                        mesh->SetScale(mesh->GetScale() + inputVec * speeds->modelScaleRate * deltaTime * mesh->GetScale());
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    if (SHIFT_PRESS)
                        mesh->Scale(selVerts, glm::vec3(1.0f) + glm::vec3(glm::clamp(inputVec.x + inputVec.y + inputVec.z, -1.0f, 1.0f) * speeds->modelScaleRate * deltaTime), sel->GetSelectionPivot());
                    else
                        mesh->Scale(selVerts, glm::vec3(1.0f) + inputVec * speeds->modelScaleRate * deltaTime, sel->GetSelectionPivot());
                    locks->rerender = true;
                    break;
                default:
                    break;
                }
                break;
                /* ========== Handle rotating ================ */
            case Tool::ROTATE:
                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    mesh->Rotate(glm::vec3(inputVec.y, inputVec.x, inputVec.z) * speeds->modelTurnSpeed * deltaTime);
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    mesh->Rotate(selVerts, glm::vec3(inputVec.y, inputVec.x, inputVec.z) * speeds->modelTurnSpeed * deltaTime, sel->GetSelectionPivot());
                    locks->rerender = true;
                    break;
                default:
                    break;
                }
                break;
                /* ========== Handle defaults ================ */
            case Tool::SELECT:
            case Tool::NONE:
            default:
                break;
            }
        }
        didReceiveInput = true;
    }

    /* =================================== Handle camera ================================== */
    if (ARROW_PRESS && !CTRL_PRESS && ALT_PRESS) {
        // Rotate camera
        camera->rotation += GetArrow(window) * speeds->cameraTurnSpeed * deltaTime;
        didReceiveInput = true;
    }

    /* =================================== Handle tools ================================== */
    /* ========== Handle saving ============== */
    if (S_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrlS) {
            WriteObjToFile(scene, "../models/", mesh->GetName());
            locks->lockCtrlS = true;
        }
        didReceiveInput = true;
    }



    /* ==================================================== Mouse Input ===================================================== */
    // Mouse input
    if (MOUSE_PRESS) {
        // Track mouse movement
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        int xPos = (int)glm::floor(mouseX);
        int yPos = (int)glm::floor(mouseY);

        // Reset previous positions
        if (*prevX < 0)
            *prevX = xPos;
        if (*prevY < 0)
            *prevY = yPos;

        float deltaX = float(*prevX - xPos);
        float deltaY = float(*prevY - yPos);

        // Handle moving with mouse
        // Alt + LMB to rotate
        if (LEFT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            camera->rotation.x += speeds->mouseTurnSpeed * deltaTime * deltaX;
            camera->rotation.y += speeds->mouseTurnSpeed * deltaTime * deltaY;
        }
        // Alt + RMB to move
        else if (RIGHT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            camera->pos += camera->right * speeds->mouseMoveSpeed * deltaTime * deltaX;
            camera->pos += camera->up * speeds->mouseMoveSpeed * deltaTime * deltaY;
        }
        // Handle selection/deselection
        else if (LEFT_MOUSE_PRESS && !locks->lockLeftMouse) {
            locks->lockLeftMouse = true;

            // Handle selection
            OldMesh* tempMesh;
            int tempFace;
            int tempVert;
            switch (sel->GetSelMode()) {
            case SelMode::MESH:
                tempMesh = Selection::GetNearestMesh(scene, *prevX, *prevY);
                if (tempMesh == sel->GetSelectedMesh())
                    sel->DeselectMesh();
                else
                    sel->SelectMesh(tempMesh);
                break;
            case SelMode::FACE:
                if (sel->GetSelectedMesh() == nullptr)
                    break;

                tempFace = Selection::GetNearestFace(scene, *prevX, *prevY);
                if (sel->IsFaceSelected(tempFace))
                    sel->DeselectFace(tempFace);
                else
                    sel->SelectFace(tempFace, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            case SelMode::VERT:
                if (sel->GetSelectedMesh() == nullptr)
                    break;

                tempVert = Selection::GetNearestVert(scene, *prevX, *prevY);
                if (sel->IsVertSelected(tempVert))
                    sel->DeselectVert(tempVert);
                else
                    sel->SelectVert(tempVert, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            }

            std::set<int> newSelVerts;
            sel->GetSelectedVerts(newSelVerts);

            for (auto iter = newSelVerts.begin(); iter != newSelVerts.end(); ++iter) {
                if (selVerts.find(*iter) == selVerts.end())
                    sel->newSelVerts.emplace(*iter);
            }
            for (auto iter = selVerts.begin(); iter != selVerts.end(); ++iter) {
                if (newSelVerts.find(*iter) == newSelVerts.end())
                    sel->removedSelVerts.emplace(*iter);
            }
        }

        // Keep mouse where it was clicked
        glfwSetCursorPos(window, (float)(*prevX), (float)(*prevY));
        didReceiveInput = true;
    }
    // Reset previous click location when there 
    else {
        locks->lockLeftMouse = false;
        locks->lockRightMouse = false;
        *prevX = -1;
        *prevY = -1;
    }



    /* ==================================================== Process Input ===================================================== */
    // Recalculate camera basis if any input was received
    if (didReceiveInput) {
        camera->CalcBasis();
        mesh->CalcBasis();
    }
    // If no input was received, clear all input locks
    else {
        locks->ClearLocks();
    }

    return didReceiveInput;
}

// Returns the combined input vector of arrow keys
glm::vec3 GetArrow(GLFWwindow* window)
{
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);

    return output;
}

// Returns the combined input vector of WASDZX
glm::vec3 GetWASDZX(GLFWwindow* window)
{
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        output += glm::vec3(0, 0, 1);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        output += glm::vec3(0, 0, -1);

    return output;
}

// Locks only the selected tool, releasing locks on other tools
void InputLocks::LockTool(Tool _tool)
{
    lockQ = (_tool == Tool::SELECT);
    lockE = (_tool == Tool::SCALE);
    lockR = (_tool == Tool::ROTATE);
    lockT = (_tool == Tool::MOVE);
    lockCtrlE = (_tool == Tool::EXTRUDE);
}

// Resets all locks on keys
void InputLocks::ClearLocks()
{
    // Don't unlock tool selections
    //lockQ = false;			// Select
    //lockE = false;			// Scale
    //lockR = false;			// Rotate
    //lockT = false;			// Translate
    //lockCtrlE = false;		// Extrude

    lockF = false;
    lockCtrlR = false;		// Render
    lockCtrlT = false;		// Triangulate
    lockCtrlS = false;		// Save
    lockDel = false;        // Delete
    lockKey5 = false;

    rerender = false;
}