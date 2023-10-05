#pragma once
#include "glIncludes.h"
#include "options.h"
#include "mathLib.h"

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

    glm::vec3 mBGColor;

    // Sets the camera's values from the options given
    void SetFromOptions(Options* options)
    {
        mFOV = options->camFov;
        mNearClip = options->camNearClip;
        mFarClip = options->camFarClip;
        mPos = options->camPos;
        mDir = options->camLookAt;
        mOrthSize = options->camSize;
        mIsPerspective = options->isPerspective;
        mBGColor = options->bgColor;

        LookAt(mDir + mPos);
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
    }
public:
    // Returns the position of the camera
    glm::vec3 GetPos() { return mPos; }
    // Returns the rotation of the camera
    glm::vec3 GetRot() { return mRot; }
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

    // Sets the position of the camera
    void SetPos(glm::vec3 pos) { mPos = pos; }
    // Sets the orthographic size of the camera
    void SetOrthSize(float size) { mOrthSize = size; }
    // Sets the far clip plane of the camera
    void SetFarClip(float farClip) { mFarClip = farClip; }
    // Sets the near clip plane of the plane
    void SetNearClip(float nearClip) { mNearClip = nearClip; }
    // Sets the FOV of the camera
    void SetFOV(float fov) { mFOV = fov; }

    // Returns the view matrix of the camera
    glm::mat4 GetView() { return glm::lookAt(mPos, mPos + mDir, mUp); }
    // Returns the camera's rotation matrix
    glm::mat4 GetRotationMatrix() { return glm::mat4(glm::vec4(mRight, 0), glm::vec4(mUp, 0), glm::vec4(-mDir, 0), glm::vec4(0, 0, 0, 1)); }
    // Returns the camera's rotation in degrees
    glm::vec3 GetRotationDegrees()
    {
        float rotX = asinf(-mDir.y);
        if (cosf(rotX) > 0.0001)
            return glm::degrees(glm::vec3(rotX, atan2f(-mDir.x, -mDir.z), atan2f(mRight.y, mUp.y)));
        return glm::degrees(glm::vec3(rotX, 0.0f, atan2f(-mUp.x, mRight.x)));
    }
    // Returns the projection matrix of the camera
    glm::mat4 GetProjection(float aspect)
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

        return projection;
    }

    // Sets the rotation of the camera to be in the given direction
    void LookAt(glm::vec3 targetPos)
    {
        mLookDist = glm::length(targetPos - mPos);
        glm::vec3 lookDir = glm::normalize(targetPos - mPos);
        mRot = glm::vec3(atan2(lookDir.x, lookDir.z), asin(lookDir.y), 0.0f);
        CalcBasis();
    }

    // Translates the camera by the given delta along the basis of the camera
    void Translate(glm::vec3 delta)
    {
        mPos += mDir * delta.x + mRight * delta.y + mUp * delta.z;
        CalcBasis();
    }

    // Rotates the camera by the given euler angles
    void Rotate(glm::vec3 delta)
    {
        mRot = glm::vec3(mRot.x + delta.x, glm::clamp(mRot.y + delta.y, -PI*0.499f, PI*0.499f), mRot.z);
        CalcBasis();
    }

    // Sets the camera to be perspective/orthographic
    void SetPerspective(bool isPerspective)
    {
        mIsPerspective = isPerspective;
        CalcBasis();
    }

    Camera(float _fov, float _nearClip, float _farClip, glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _up, float _orthSize, bool _isPerspective)
        : mFOV(_fov), mNearClip(_nearClip), mFarClip(_farClip), mPos(_pos), mDir(_dir), mUp(_up), mOrthSize(_orthSize), mIsPerspective(_isPerspective)
    {
        mBGColor = glm::vec3(80, 100, 100);
        LookAt(mDir + mPos);
    }

    Camera(Options* options) { SetFromOptions(options); }
};