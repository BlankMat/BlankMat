#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "files/config.h"
#include "windows/guiVariableTypes.h"

class Camera
{
private:
    UIFloat mFOV;
    UIFloat mNearClip;
    UIFloat mFarClip;

    UIVec3 mPos;
    UIVec3 mRot;
    UIVec3 mPivot;
    UIVec3 mTarget;

    UIVec3 mDir;
    UIVec3 mUp;
    UIVec3 mRight;

    UIFloat mOrthSize;
    UIBool mIsPerspective;
    UIBool mIsWireframe;
    UIBool mRotateAroundPivot;

    UIColor mBGColor;

    glm::mat4 mRotation = glm::mat4(1.0f);
    glm::mat4 mView = glm::mat4(1.0f);
    glm::mat4 mProjection = glm::mat4(1.0f);
    bool mRecalcRotation = true;
    bool mRecalcView = true;
    bool mRecalcProjection = true;
    float mPrevAspect = 1.0f;

    // Calculates the basis of the camera
    void CalcBasis()
    {
        glm::vec3 rot = mRot.Get();
        mDir.SetNoUpdate(glm::normalize(glm::vec3(
            cos(rot.y) * sin(rot.x),
            sin(rot.y),
            cos(rot.y) * cos(rot.x)
        )));

        // Right vector
        mRight.SetNoUpdate(glm::normalize(glm::cross((glm::vec3)mDir, glm::vec3(0.0f, 1.0f, 0.0f))));

        // Up vector : perpendicular to both direction and right
        mUp.SetNoUpdate(glm::normalize(glm::cross((glm::vec3)mRight, (glm::vec3)mDir)));
        mTarget.SetNoUpdate(mPos + mDir);

        mRecalcRotation = true;
        mRecalcView = true;
    }
public:
    // Returns the position of the camera
    UIVec3& GetPos() { return mPos; }
    // Returns the rotation of the camera
    UIVec3& GetRot() { return mRot; }
    // Returns the direction of the camera
    UIVec3& GetDir() { return mDir; }
    // Returns the up direction of the camera
    UIVec3& GetUp() { return mUp; }
    // Returns the camera's current look target
    UIVec3& GetTarget() { return mTarget; }
    // Returns the camera's pivot
    UIVec3& GetPivot() { return mPivot; }
    // Returns the background color of the camera
    UIColor& GetBGColor() { return mBGColor; }
    // Returns the orthographic size of the camera
    UIFloat& GetOrthSize() { return mOrthSize; }
    // Returns the far clip plane of the camera
    UIFloat& GetFarClip() { return mFarClip; }
    // Returns the near clip plane of the plane
    UIFloat& GetNearClip() { return mNearClip; }
    // Returns the FOV of the camera
    UIFloat& GetFOV() { return mFOV; }
    // Returns whether the camera is in perspective
    UIBool& IsPerspective() { return mIsPerspective; }
    // Returns whether the camera is in wireframe
    UIBool& IsWireframe() { return mIsWireframe; }
    // Returns whether the camera is rotating around the pivot
    UIBool& IsRotatingAroundPivot() { return mRotateAroundPivot; }

    // Sets the position of the camera
    void SetPos(const glm::vec3& pos) { mPos.Set(pos); mRecalcView = true; }
    // Sets the orthographic size of the camera
    void SetOrthSize(float size) { mOrthSize.Set(size); }
    // Sets the far clip plane of the camera
    void SetFarClip(float farClip) { mFarClip.Set(farClip); }
    // Sets the near clip plane of the plane
    void SetNearClip(float nearClip) { mNearClip.Set(nearClip); }
    // Sets the FOV of the camera
    void SetFOV(float fov) { mFOV.Set(fov); }
    // Sets the camera's rotate mode to be around the pivot
    void SetPivotRotationMode(bool isPivotRotation) { mRotateAroundPivot.Set(isPivotRotation); }
    // Sets the pivot of the camera
    void SetPivot(const glm::vec3& pivot) { mPivot.Set(pivot); }

    // Sets the camera to be perspective/orthographic
    void SetPerspective(bool isPerspective)
    {
        if (mIsPerspective.Equals(isPerspective))
            return;
        mIsPerspective.Set(isPerspective);
        mRecalcProjection = true;
    }

    // Sets the camera to be/not be wireframe
    void SetWireframe(bool isWireframe)
    {
        if (mIsWireframe.Equals(isWireframe))
            return;
        mIsWireframe.Set(isWireframe);
        TriggerRecalcWireframe();
    }

    // Returns the view matrix of the camera
    const glm::mat4& GetView()
    {
        if (mRecalcView)
        {
            mTarget.SetNoUpdate(mPos + mDir);
            mView = glm::lookAt((glm::vec3)mPos, (glm::vec3)mTarget, (glm::vec3)mUp);
            mRecalcView = false;
        }
        return mView;
    }
    
    // Returns the camera's rotation matrix
    const glm::mat4& GetRotationMatrix()
    {
        if (mRecalcRotation)
        {
            mRotation = glm::mat4(glm::vec4((glm::vec3)mRight, 0), glm::vec4((glm::vec3)mUp, 0), glm::vec4(-(glm::vec3)mDir, 0), glm::vec4(0, 0, 0, 1));
            mRecalcProjection = false;
        }
        return mRotation;
    }
    
    // Returns the projection matrix of the camera
    const glm::mat4& GetProjection(float aspect)
    {
        // Only recalculate projection matrix if needed
        if (aspect != 0.0f && (mRecalcProjection || aspect != mPrevAspect))
        {
            // Projection
            glm::mat4 projection = glm::mat4(1.0f);

            // Perspective projection
            if (mIsPerspective) 
            {
                projection = glm::perspective(glm::radians(+mFOV), aspect, +mNearClip, +mFarClip);
            }
            // Orthographic projection
            else 
            {
                projection = glm::ortho(-mOrthSize * aspect, +mOrthSize * aspect, -mOrthSize, +mOrthSize, +mNearClip, +mFarClip);
            }
            mProjection = projection;
            mRecalcProjection = false;
            mPrevAspect = aspect;
        }
        return mProjection;
    }

    // Sets the rotation of the camera to be in the given direction
    void LookAt(const glm::vec3& targetPos)
    {
        mPivot.SetNoUpdate(targetPos);
        mTarget.Set(targetPos);
        glm::vec3 lookDir = glm::normalize(targetPos - (glm::vec3)mPos);
        mRot.SetNoUpdate(glm::vec3(atan2(lookDir.x, lookDir.z), asin(lookDir.y), 0.0f));
        CalcBasis();
    }

    // Translates the camera by the given delta along the basis of the camera
    void Translate(const glm::vec3& delta)
    {
        mPos.Set(mPos + mDir * delta.x + mRight * delta.y + mUp * delta.z);
        mRecalcView = true;
    }

    // Rotates the camera by the given euler angles
    void Rotate(const glm::vec3& delta)
    {
        // When rotating around pivot, change position instead of rotation
        glm::vec3 rot = mRot.Get();
        if (mRotateAroundPivot)
        {
            // TODO: Rotate around pivot
            mRot.Set(glm::vec3(rot.x + delta.x, glm::clamp(rot.y + delta.y, -PI * 0.499f, PI * 0.499f), rot.z));
        }
        // When rotating around camera, don't change position
        else
        {
            mRot.Set(glm::vec3(rot.x + delta.x, glm::clamp(rot.y + delta.y, -PI * 0.499f, PI * 0.499f), rot.z));
        }
        CalcBasis();
    }

    // Triggers a recalculation of the view matrix on the next frame
    void TriggerRecalcView() { mRecalcView = true; }
    // Triggers a recalculation of the projection matrix on the next frame
    void TriggerRecalcProjection() { mRecalcProjection = true; }
    // Triggers a recalculation of the rotation matrix on the next frame
    void TriggerRecalcRotation() { mRecalcRotation = true; }

    // Enables or disables wireframe
    void TriggerRecalcWireframe()
    {
        if (mIsWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // Disable culling
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            // Enable culling
            glEnable(GL_CULL_FACE);

            // Enable depth buffer
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }

    /// <summary>
    /// Constructs a camera from the given parameters.
    /// </summary>
    /// <param name="actionStack">Global action stack</param>
    /// <param name="fov">Field of View of perspective camera</param>
    /// <param name="nearClip">Near clip plane</param>
    /// <param name="farClip">Far clip plane</param>
    /// <param name="pos">Position of camera</param>
    /// <param name="dir">Direction that camera faces (not normalized)</param>
    /// <param name="up">Up vector of the camera</param>
    /// <param name="color">Background color displayed by the camera</param>
    /// <param name="orthSize">Orthogonal size of orth camera</param>
    /// <param name="isPerspective">Whether the camera is in perspective or orthogonal view mode</param>
    /// <param name="isWireframe">Whether the camera is rendering in wireframe or not</param>
    Camera(ActionStack* actionStack, float fov, float nearClip, float farClip, 
        const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& up, const glm::vec3& color, 
        float orthSize = 10.0f, bool isPerspective = true, bool isWireframe = false)
    {
        mFOV = UIFloat("FOV", fov, actionStack, [this]() { TriggerRecalcProjection(); });
        mNearClip = UIFloat("Near Clip", nearClip, actionStack, [this]() { TriggerRecalcProjection(); });
        mFarClip = UIFloat("Far Clip", farClip, actionStack, [this]() { TriggerRecalcProjection(); });
        mPos = UIVec3("Position", pos, actionStack, [this]() { TriggerRecalcView(); });
        mDir = UIVec3("Direction", dir, actionStack, [this]() { TriggerRecalcView(); });
        mUp = UIVec3("Up", up, actionStack, [this]() { TriggerRecalcView(); });
        mRot = UIVec3("Rotation", glm::vec3(0.0f), actionStack, [this]() { TriggerRecalcRotation(); });
        mPivot = UIVec3("Pivot", pos + dir, actionStack);
        mTarget = UIVec3("Target", pos + dir, actionStack, [this]() { TriggerRecalcView(); });
        mOrthSize = UIFloat("Size", orthSize, actionStack, [this]() { TriggerRecalcProjection(); });
        mIsPerspective = UIBool("Perspective", isPerspective, actionStack, [this]() { TriggerRecalcProjection(); });
        mIsWireframe = UIBool("Wireframe", isWireframe, actionStack, [this]() { TriggerRecalcView(); });
        mRotateAroundPivot = UIBool("Lock Rotation", false, actionStack);
        mBGColor = UIColor("Background", color, actionStack);

        LookAt(mPivot);
        TriggerRecalcWireframe();
    }

    /// <summary>
    /// Constructs a camera from the given config
    /// </summary>
    /// <param name="actionStack">Global action stack</param>
    /// <param name="config">Config to build camera from</param>
    Camera(ActionStack* actionStack, Config* config)
        : Camera(actionStack, config->GetFloat("fov"), config->GetFloat("nearClip"), config->GetFloat("farClip"),
            config->GetVec("pos"), config->GetVec("dir"), config->GetVec("up"), config->GetVec("bgColor"), 
            config->GetFloat("size"), config->GetBool("perspective"), config->GetBool("wireframe"))
    {}
};