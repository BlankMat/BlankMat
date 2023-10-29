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
        void Interact(double cursorX, double cursorY)
        {/*
            switch(mSel->GetSelMode())
            {
                case SelMode::FACE :
                {
                    mSel->SelectFace(mSel->GetNearestFace(,(float)cursorX,(float)cursorY));
                }
                case SelMode::VERT :
                {
                    mSel->SelectVert(mSel->GetNearestVert());
                }
                case SelMode::MESH :
                {
                    mSel->SelectMesh(mSel->GetNearestMesh());
                }
            }
            */
        }
        SelectTool(Selection* sel)
        {
            mSel = sel;
        }
};