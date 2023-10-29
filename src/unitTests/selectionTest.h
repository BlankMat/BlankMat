#include "selection.h"

class selectionTest
{
    Selection* mSelection; 
    bool runTests()
    {
        bool success = false;
        for(Tool tool = Tool::SELECT; tool != Tool::LAST; tool = (Tool)((int)tool+1))
        {
            if(!setToolAndCheck(tool))
            {
                return false;
            }
        }
        for(SelMode mode = SelMode::MESH; mode != SelMode::LAST; mode = (SelMode)((int)mode+1))
        {
            if(setModeAndCheck(mode))
            {
                return false;
            }
        }
        return true;
    }
    bool setToolAndCheck(Tool tool)
    {
        mSelection->SetTool(tool);
        return (mSelection->GetTool() == tool);
    }
    bool setModeAndCheck(SelMode selMode)
    {
        mSelection->SetSelMode(selMode);
        return (mSelection->GetSelMode() == selMode);
    }
    selectionTest()
    {
        mSelection = new Selection();
    }
    ~selectionTest()
    {
        delete mSelection;
    }
};