#include "input.h"
#include "interaction/addValueCommand.h"
#include "interaction/changeValueCommand.h"
#include "interaction/undoCommand.h"
#include "interaction/redoCommand.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
bool ProcessInput(Window* window, IScene* scene, State* state, InputLocks* locks, float deltaTime, int* prevX, int* prevY)
{
    // TODO: Replace mesh reference with proper reference
    Camera* camera = scene->GetCamera();
    Selection* sel = state->GetSel();
    Config* config = state->GetConfig();
    ActionStack* actions = state->GetActionStack();
    IEntity* selEntity = sel->GetSelectedEntity();
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
            glm::vec3 targetPos = (selEntity != nullptr) ? selEntity->GetWorldPos() : glm::vec3(0.0f);
            camera->LookAt(targetPos);

            locks->lockF = true;
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


    // Debug run fake commands
    if (KEY1_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrl1)
        {
            actions->Execute(new ChangeValueCommand<int>(&state->DEBUG_fakeNumber, 0));
            locks->lockCtrl1 = true;
        }
        didReceiveInput = true;
    }
    else if (KEY2_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrl2)
        {
            actions->Execute(new AddValueCommand<int>(&state->DEBUG_fakeNumber, 1));
            locks->lockCtrl2 = true;
        }
        didReceiveInput = true;
    }
    else if (KEY3_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrl3)
        {
            actions->Execute(new AddValueCommand<int>(&state->DEBUG_fakeNumber, -1));
            locks->lockCtrl3 = true;
        }
        didReceiveInput = true;
    }
    else if (KEY4_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrl4)
        {
            actions->Execute(new ChangeValueCommand<bool>(&state->enableDiffuseMap, true));
            locks->lockCtrl4 = true;
        }
        didReceiveInput = true;
    }
    else if (KEY5_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrl5)
        {
            actions->Execute(new ChangeValueCommand<bool>(&state->enableDiffuseMap, false));
            locks->lockCtrl5 = true;
        }
        didReceiveInput = true;
    }
    else if (Y_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrlY)
        {
            actions->Redo();
            locks->lockCtrlY = true;
        }
        didReceiveInput = true;
    }
    else if (Z_PRESS && CTRL_PRESS) {
        if (!locks->lockCtrlZ)
        {
            actions->Undo();
            locks->lockCtrlZ = true;
        }
        didReceiveInput = true;
    }


    /* ==================================================== Keyboard Input ===================================================== */
    /* =================================== Handle movement input ================================== */
    if (WASDZX_PRESS && !CTRL_PRESS) {
        glm::vec3 inputVec = GetWASDZX(window);

        // Only move camera on ALT
        if (ALT_PRESS) {
            camera->Translate(deltaTime * config->GetFloat("speeds.camMove") * inputVec);
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
                if (selEntity == nullptr)
                    break;

                glm::vec3 moveVec = inputVec.x * selEntity->GetFront() + inputVec.y * selEntity->GetRight() + inputVec.z * selEntity->GetUp();
                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    selEntity->Translate(moveVec * deltaTime * config->GetFloat("speeds.modelMove"));
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
                if (selEntity == nullptr)
                    break;

                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    if (SHIFT_PRESS)
                        selEntity->SetScale(selEntity->GetScale() + glm::clamp(inputVec.x + inputVec.y + inputVec.z, -1.0f, 1.0f) * config->GetFloat("speeds.modelScale") * deltaTime * selEntity->GetScale());
                    else
                        selEntity->SetScale(selEntity->GetScale() + inputVec * config->GetFloat("speeds.modelScale") * deltaTime * selEntity->GetScale());
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
                if (selEntity == nullptr)
                    break;

                switch (sel->GetSelMode()) {
                case SelMode::MESH:
                    selEntity->Rotate(glm::vec3(inputVec.y, inputVec.x, inputVec.z) * config->GetFloat("speeds.modelTurn") * deltaTime);
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
        camera->Rotate(GetArrow(window) * config->GetFloat("speeds.camTurn") * deltaTime);
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

    // Disable UI
    if (SPACE_PRESS)
    {
        if (!locks->lockSpace) {
            state->drawGUI.Set(!state->drawGUI);
            locks->lockSpace = true;
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
            camera->Rotate(config->GetFloat("speeds.mouseTurn")* deltaTime * glm::vec3(deltaX, deltaY, 0.0f));
            locks->lockLeftMouse = true;

            // Keep mouse where it was clicked
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            glfwSetCursorPos(window->GetWindow(), (float)(*prevX), (float)(*prevY));
        }
        // Alt + RMB to move
        else if (RIGHT_MOUSE_PRESS && ALT_PRESS) {
            // Apply changes to camera
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            camera->Translate(config->GetFloat("speeds.mouseMove")* deltaTime * glm::vec3(0.0f, deltaX, deltaY));
            // Keep mouse where it was clicked
            glfwSetCursorPos(window->GetWindow(), (float)(*prevX), (float)(*prevY));
        }
        // Handle selection/deselection
        else if (LEFT_MOUSE_PRESS && !locks->lockLeftMouse) {
            locks->lockLeftMouse = true;
            IEntity* transformHandle = scene->GetEntity(TRANSFORM_HANDLE);
            if (transformHandle != nullptr)
                transformHandle->ToggleEnabled();

            // Handle selection
            IMesh* tempMesh;
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
                if (selEntity == nullptr)
                    break;

                tempFace = Selection::GetNearestFace(scene, u, v);
                if (sel->IsFaceSelected(tempFace))
                    sel->DeselectFace(tempFace);
                else
                    sel->SelectFace(tempFace, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            case SelMode::VERT:
                if (selEntity == nullptr)
                    break;

                tempVert = Selection::GetNearestVert(scene, u, v);
                if (sel->IsVertSelected(tempVert))
                    sel->DeselectVert(tempVert);
                else
                    sel->SelectVert(tempVert, !SHIFT_PRESS);
                locks->reselect = true;
                break;
            }
        }

        didReceiveInput = true;
    }
    // Reset previous click location when there 
    else {
        locks->lockLeftMouse = false;
        locks->lockRightMouse = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
    if (locks->rerender && selEntity != nullptr) {
        selEntity->CalcBasis();
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

    lockCtrl1 = false;
    lockCtrl2 = false;
    lockCtrl3 = false;
    lockCtrl4 = false;
    lockCtrl5 = false;
    lockCtrlY = false;
    lockCtrlZ = false;

    lockSpace = false;
    lockAltT = false;       // Disco light
    lockAltR = false;       // Rotating light

    rerender = false;
}