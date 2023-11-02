#pragma once
#include "glIncludes.h"
#include "interfaces/iTool.h"

class SelectTool : public ITool
{
    protected:
        //State mState;
    public:
        void Select()
        {

        }
        void Deselect()
        {

        }
        void Interact()
        {
            ImGui::GetMousePos();
        }
        SelectTool()
        {

        }
};