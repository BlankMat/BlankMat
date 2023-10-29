#pragma once
#include "iTool.h"
#include "glIncludes.h"

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