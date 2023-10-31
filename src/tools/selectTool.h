#pragma once
#include "iTool.h"
//#include "glIncludes.h"

class SelectTool : public ITool
{
    public:
    void Select(State* state, IScene* scene)
    {

    }
	void Deselect(State* state, IScene* scene)
    {

    }
	void Interact(State* state, IScene* scene)
    {
        Selection* sel = state->GetSel();
        //switch(sel->GetSelMode())
        {/*
            case SelMode::FACE :
            {
                sel->SelectFace(mSel->GetNearestFace(,(float)cursorX,(float)cursorY));
            }
            case SelMode::VERT :
            {
                sel->SelectVert(mSel->GetNearestVert());
            }
            case SelMode::MESH :
            {
                sel->SelectMesh(mSel->GetNearestMesh());
            }
            */
        }
    }
};