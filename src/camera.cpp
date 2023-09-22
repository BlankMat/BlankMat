#include "camera.h"

// Calculates the basis of the camera
void Camera::CalcBasis()
{
    dir = glm::vec3(
        cos(rotation.y) * sin(rotation.x),
        sin(rotation.y),
        cos(rotation.y) * cos(rotation.x)
    );

    // Right vector
    right = glm::vec3(
        sin(rotation.x - HALF_PI),
        0,
        cos(rotation.x - HALF_PI)
    );

    // Up vector : perpendicular to both direction and right
    up = glm::cross(right, dir);
}

void Camera::SetFromOptions(Options* options)
{
    fov = options->camFov;
    nearClip = options->camNearClip;
    farClip = options->camFarClip;
    pos = options->camPos;
    dir = options->camLookAt;
    up = options->camUp;
    orthSize = options->camSize;
    isPerspective = options->isPerspective;
    rotation = glm::vec3(PI, 0.0f, 0.0f);

    CalcBasis();
}

Camera::Camera(float _fov, float _nearClip, float _farClip, glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _up, glm::vec2 _orthSize, bool _isPerspective)
    : fov(_fov), nearClip(_nearClip), farClip(_farClip), pos(_pos), dir(_dir), up(_up), orthSize(_orthSize), isPerspective(_isPerspective)
{
    rotation = glm::vec3(PI, 0.0f, 0.0f);
    CalcBasis();
}

Camera::Camera(Options* options)
{
    SetFromOptions(options);
}