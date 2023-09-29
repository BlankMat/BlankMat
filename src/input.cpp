#include "input.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
bool ProcessInput(Window* window, Scene* scene, Selection* sel, InputLocks* locks, Options* options, SpeedConsts* speeds, float deltaTime, int* prevX, int* prevY)
{
    // TODO: Replace mesh reference with proper reference
    Camera* camera = scene->GetCamera();
    Mesh* selMesh = sel->GetSelectedMesh();
    std::set<unsigned int> selVerts;
    sel->GetSelectedVerts(selVerts);
    bool didReceiveInput = false;

    // Get window width and height
    window->CalcWindowSize();
    int width = window->GetWidth();
    int height = window->GetHeight();
    GLFWwindow* glfwWindow = window->GetWindow();
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;



    /* ==================================================== Window Stuff ===================================================== */
    // Exit window
    if (ESC_PRESS) {
        glfwSetWindowShouldClose(glfwWindow, true);
        return true;
    }
    // Focus object
    if (F_PRESS && !CTRL_PRESS) {
        // Only focus when not locked
        if (!locks->lockF) {
            glm::vec3 targetPos = (selMesh != nullptr) ? selMesh->GetPos() : glm::vec3(0.0f);
            glm::vec3 targetDir = glm::normalize(targetPos - camera->GetPos());
            float rotY = asin(targetDir.y);
            float rotX = atan2(targetDir.x, targetDir.z);
            //camera->SetRot(glm::vec3(rotX, rotY, 0.0f));
            camera->LookAt(targetPos);

            locks->lockF = true;
        }
        didReceiveInput = true;
    }
    // Toggle perspective
    if (KEY4_PRESS && !CTRL_PRESS) {
        if (!locks->lockKey4) {
            options->isPerspective = !options->isPerspective;
            camera->SetPerspective(options->isPerspective);
            std::cout << "Enabled " << (options->isPerspective == 1 ? "perspective" : "orthographic") << " view\n";
            locks->lockKey4 = true;
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
    // Toggle disco light
    if (T_PRESS && ALT_PRESS && !CTRL_PRESS) {
        if (!locks->lockAltT) {
            options->isDiscoLight = !options->isDiscoLight;
            std::cout << "Turned disco light " << (options->isDiscoLight ? "on" : "off") << "\n";
            locks->lockAltT = true;
        }
        didReceiveInput = true;
    }
    // Toggle rotating light
    if (R_PRESS && ALT_PRESS && !CTRL_PRESS) {
        if (!locks->lockAltR) {
            options->isRotatingLight = !options->isRotatingLight;
            std::cout << "Turned rotating light " << (options->isRotatingLight ? "on" : "off") << "\n";
            locks->lockAltR = true;
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
            camera->Translate(deltaTime * speeds->cameraMoveSpeed * inputVec);
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
                if (selMesh == nullptr)
                    break;

                glm::vec3 moveVec = inputVec.x * selMesh->GetFront() + inputVec.y * selMesh->GetRight() + inputVec.z * selMesh->GetUp();
                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    selMesh->Translate(moveVec * deltaTime * speeds->modelMoveSpeed);
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    //selMesh->Translate(selVerts, moveVec * deltaTime * speeds->modelMoveSpeed);
                    locks->rerender = true;
                    break;
                default:
                    break;
                }
                break;
                /* ========== Handle scaling ================= */
            case Tool::SCALE:
                if (selMesh == nullptr)
                    break;

                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    if (SHIFT_PRESS)
                        selMesh->SetScale(selMesh->GetScale() + glm::clamp(inputVec.x + inputVec.y + inputVec.z, -1.0f, 1.0f) * speeds->modelScaleRate * deltaTime * selMesh->GetScale());
                    else
                        selMesh->SetScale(selMesh->GetScale() + inputVec * speeds->modelScaleRate * deltaTime * selMesh->GetScale());
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    //if (SHIFT_PRESS)
                    //    selMesh->Scale(selVerts, glm::vec3(1.0f) + glm::vec3(glm::clamp(inputVec.x + inputVec.y + inputVec.z, -1.0f, 1.0f) * speeds->modelScaleRate * deltaTime), sel->GetSelectionPivot());
                    //else
                    //    selMesh->Scale(selVerts, glm::vec3(1.0f) + inputVec * speeds->modelScaleRate * deltaTime, sel->GetSelectionPivot());
                    locks->rerender = true;
                    break;
                default:
                    break;
                }
                break;
                /* ========== Handle rotating ================ */
            case Tool::ROTATE:
                if (selMesh == nullptr)
                    break;

                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    selMesh->Rotate(glm::vec3(inputVec.y, inputVec.x, inputVec.z) * speeds->modelTurnSpeed * deltaTime);
                    break;
                case SelMode::FACE:
                case SelMode::VERT:
                    //selMesh->Rotate(selVerts, glm::vec3(inputVec.y, inputVec.x, inputVec.z) * speeds->modelTurnSpeed * deltaTime, sel->GetSelectionPivot());
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
        camera->Rotate(GetArrow(window) * speeds->cameraTurnSpeed * deltaTime);
        didReceiveInput = true;
    }

    /* =================================== Handle tools ================================== */
    /* ========== Handle saving ============== */
    if (S_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrlS) {
            // TODO: Save file
            locks->lockCtrlS = true;
        }
        didReceiveInput = true;
    }



    /* ==================================================== Mouse Input ===================================================== */
    // Mouse input
    if (MOUSE_PRESS) {
        // Track mouse movement
        double mouseX, mouseY;
        glfwGetCursorPos(glfwWindow, &mouseX, &mouseY);
        int xPos = (int)glm::floor(mouseX);
        int yPos = (int)glm::floor(mouseY);
        float u = (xPos + 0.5f) / width;
        float v = (yPos + 0.5f) / height;

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
            camera->Rotate(speeds->mouseTurnSpeed * deltaTime * glm::vec3(deltaX, deltaY, 0.0f));
        }
        // Alt + RMB to move
        else if (RIGHT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            camera->Translate(speeds->mouseMoveSpeed * deltaTime * glm::vec3(0.0f, deltaX, deltaY));
        }
        // Handle selection/deselection
        else if (LEFT_MOUSE_PRESS && !locks->lockLeftMouse) {
            locks->lockLeftMouse = true;

            // Handle selection
            Mesh* tempMesh;
            int tempFace;
            int tempVert;
            switch (sel->GetSelMode()) {
            case SelMode::MESH:
                tempMesh = Selection::GetNearestMesh(scene, u, v);
                if (tempMesh == sel->GetSelectedMesh())
                    sel->DeselectMesh();
                else
                    sel->SelectMesh(tempMesh);
                break;
            case SelMode::FACE:
                if (selMesh == nullptr)
                    break;

                tempFace = Selection::GetNearestFace(scene, u, v);
                if (sel->IsFaceSelected(tempFace))
                    sel->DeselectFace(tempFace);
                else
                    sel->SelectFace(tempFace, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            case SelMode::VERT:
                if (selMesh == nullptr)
                    break;

                tempVert = Selection::GetNearestVert(scene, u, v);
                if (sel->IsVertSelected(tempVert))
                    sel->DeselectVert(tempVert);
                else
                    sel->SelectVert(tempVert, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            }

            std::set<unsigned int> newSelVerts;
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
        glfwSetCursorPos(window->GetWindow(), (float)(*prevX), (float)(*prevY));
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
    // If no input was received, clear all input locks
    if (!didReceiveInput) {
        locks->ClearLocks();
    }

    // Recalculate the selected mesh if changes were made
    if (locks->rerender && selMesh != nullptr) {
        selMesh->CalcBasis();
    }

    return didReceiveInput;
}

// Returns the combined input vector of arrow keys
glm::vec3 GetArrow(Window* window)
{
    GLFWwindow* glfwWindow = window->GetWindow();
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);

    return output;
}

// Returns the combined input vector of WASDZX
glm::vec3 GetWASDZX(Window* window)
{
    GLFWwindow* glfwWindow = window->GetWindow();
    glm::vec3 output = glm::vec3(0, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        output += glm::vec3(1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        output += glm::vec3(-1, 0, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        output += glm::vec3(0, 1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        output += glm::vec3(0, -1, 0);
    if (glfwGetKey(glfwWindow, GLFW_KEY_Z) == GLFW_PRESS)
        output += glm::vec3(0, 0, 1);
    if (glfwGetKey(glfwWindow, GLFW_KEY_X) == GLFW_PRESS)
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
    lockKey4 = false;
    lockKey5 = false;

    lockAltT = false;       // Disco light
    lockAltR = false;       // Rotating light

    rerender = false;
}