#pragma once
#include "glIncludes.h"
#include "utils.h"
#include "files/config.h"

class Camera
{
private:
    float mFOV;
    float mNearClip;
    float mFarClip;
    float mLookDist;

    glm::vec3 mPos;
    glm::vec3 mRot;

    glm::vec3 mDir;
    glm::vec3 mUp;
    glm::vec3 mRight;

    float mOrthSize;
    bool mIsPerspective;
    bool mIsWireframe;

    glm::vec3 mBGColor;

    glm::mat4 mRotation;
    glm::mat4 mView;
    glm::mat4 mProjection;
    bool mRecalcRotation;
    bool mRecalcView;
    bool mRecalcProjection;
    float mPrevAspect;

    // Sets the camera's values from the config given
    void UseConfig(Config* config)
    {
        if (config->GetName() != "camera")
            config = config->GetConfig("camera");
        mFOV = config->GetFloat("fov");
        mNearClip = config->GetFloat("nearClip");
        mFarClip = config->GetFloat("farClip");
        mPos = config->GetVec("pos");
        mDir = config->GetVec("dir");
        mUp = config->GetVec("up");
        mBGColor = config->GetVec("bgColor");
        mOrthSize = config->GetFloat("size");
        mIsPerspective = config->GetBool("perspective");
        mIsWireframe = config->GetBool("wireframe");

        mRecalcProjection = true;
        mRecalcView = true;
        mRecalcRotation = true;
        mIsWireframe = false;
        mProjection = glm::mat4(1.0f);
        mView = glm::mat4(1.0f);
        mRotation = glm::mat4(1.0f);
        mPrevAspect = 1.0f;

        LookAt(mDir + mPos);
        EnableWireframe(mIsWireframe);
    }

    // Calculates the basis of the camera
    void CalcBasis()
    {
        mDir = glm::normalize(glm::vec3(
            cos(mRot.y) * sin(mRot.x),
            sin(mRot.y),
            cos(mRot.y) * cos(mRot.x)
        ));

        // Right vector
        mRight = glm::normalize(glm::cross(mDir, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Up vector : perpendicular to both direction and right
        mUp = glm::normalize(glm::cross(mRight, mDir));

        mRecalcRotation = true;
        mRecalcView = true;
    }
public:
    // Returns the position of the camera
    glm::vec3 GetPos() { return mPos; }
    // Returns the rotation of the camera
    glm::vec3 GetRot() { return mRot; }
    // Returns the direction of the camera
    glm::vec3 GetDir() { return mDir; }
    // Returns the camera's (estimated) look position
    glm::vec3 GetLookAt() { return mPos + mDir * mLookDist; }
    // Returns the background color of the camera
    glm::vec3 GetBGColor() { return mBGColor; }
    // Returns the orthographic size of the camera
    float GetOrthSize() { return mOrthSize; }
    // Returns the far clip plane of the camera
    float GetFarClip() { return mFarClip; }
    // Returns the near clip plane of the plane
    float GetNearClip() { return mNearClip; }
    // Returns the FOV of the camera
    float GetFOV() { return mFOV; }
    // Returns whether the camera is in perspective
    bool IsPerspective() { return mIsPerspective; }
    // Returns whether the camera is in wireframe
    bool IsWireframe() { return mIsWireframe; }

    // Sets the position of the camera
    void SetPos(const glm::vec3& pos) { mPos = pos; mRecalcView = true; }
    // Sets the orthographic size of the camera
    void SetOrthSize(float size) { mOrthSize = size; }
    // Sets the far clip plane of the camera
    void SetFarClip(float farClip) { mFarClip = farClip; }
    // Sets the near clip plane of the plane
    void SetNearClip(float nearClip) { mNearClip = nearClip; }
    // Sets the FOV of the camera
    void SetFOV(float fov) { mFOV = fov; }

    // Sets the camera to be perspective/orthographic
    void SetPerspective(bool isPerspective)
    {
        if (mIsPerspective == isPerspective)
            return;

        mIsPerspective = isPerspective;
        mRecalcProjection = true;
    }

    // Sets the camera to be/not be wireframe
    void SetWireframe(bool isWireframe)
    {
        // Don't update back to original value
        if (mIsWireframe == isWireframe)
            return;

        mIsWireframe = isWireframe;
        EnableWireframe(isWireframe);
    }

    // Returns the view matrix of the camera
    const glm::mat4& GetView()
    {
        if (mRecalcView)
        {
            mView = glm::lookAt(mPos, mPos + mDir, mUp);
            mRecalcView = false;
        }
        return mView;
    }
    
    // Returns the camera's rotation matrix
    const glm::mat4& GetRotationMatrix()
    {
        if (mRecalcRotation)
        {
            mRotation = glm::mat4(glm::vec4(mRight, 0), glm::vec4(mUp, 0), glm::vec4(-mDir, 0), glm::vec4(0, 0, 0, 1));
            mRecalcProjection = false;
        }
        return mRotation;
    }
    
    // Returns the projection matrix of the camera
    const glm::mat4& GetProjection(float aspect)
    {
        // Only recalculate projection matrix if needed
        if (mRecalcProjection || aspect != mPrevAspect)
        {
            // Projection
            glm::mat4 projection = glm::mat4(1.0f);

            // Perspective projection
            if (mIsPerspective) {
                projection = glm::perspective(glm::radians(mFOV), aspect, mNearClip, mFarClip);
            }
            // Orthographic projection
            else {
                projection = glm::ortho(-mOrthSize * aspect, mOrthSize * aspect, -mOrthSize, mOrthSize, mNearClip, mFarClip);
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
        mLookDist = glm::length(targetPos - mPos);
        glm::vec3 lookDir = glm::normalize(targetPos - mPos);
        mRot = glm::vec3(atan2(lookDir.x, lookDir.z), asin(lookDir.y), 0.0f);
        CalcBasis();
    }

    // Translates the camera by the given delta along the basis of the camera
    void Translate(const glm::vec3& delta)
    {
        mPos += mDir * delta.x + mRight * delta.y + mUp * delta.z;
        mRecalcView = true;
    }

    // Rotates the camera by the given euler angles
    void Rotate(const glm::vec3& delta)
    {
        mRot = glm::vec3(mRot.x + delta.x, glm::clamp(mRot.y + delta.y, -PI * 0.499f, PI * 0.499f), mRot.z); CalcBasis();
        CalcBasis();
    }

    // Enables or disables wireframe
    static void EnableWireframe(bool enable)
    {
        if (enable) {
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

    Camera(float _fov, float _nearClip, float _farClip, const glm::vec3& _pos, const glm::vec3& _dir, const glm::vec3& _up, float _orthSize, bool _isPerspective)
        : mFOV(_fov), mNearClip(_nearClip), mFarClip(_farClip), mPos(_pos), mDir(_dir), mUp(_up), mOrthSize(_orthSize), mIsPerspective(_isPerspective)
    {
        mBGColor = glm::vec3(80, 100, 100);
        mRecalcProjection = true;
        mRecalcView = true;
        mRecalcRotation = true;
        mIsWireframe = false;
        mProjection = glm::mat4(1.0f);
        mView = glm::mat4(1.0f);
        mRotation = glm::mat4(1.0f);
        mPrevAspect = 1.0f;

        LookAt(mDir + mPos);
        EnableWireframe(mIsWireframe);
    }

    Camera(Config* config)
    {
        UseConfig(config);
    }
};