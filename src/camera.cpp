#include "camera.h"

glm::mat4 Camera::GetView()
{
    return glm::lookAt(pos, pos + dir, up);
}

glm::mat4 Camera::GetProjection(float aspect)
{
    // Projection
    glm::mat4 projection = glm::mat4(1.0f);

    // Perspective projection
    if (isPerspective) {
        projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    }
    // Orthographic projection
    else {
        glm::mat4 projection = glm::ortho(-orthSize.x, orthSize.x, -orthSize.y, orthSize.y, nearClip, farClip);
    }

    return projection;
}

// Calculates the basis of the camera
void Camera::CalcBasis()
{
    dir = glm::normalize(glm::vec3(
        cos(rotation.x) * cos(rotation.y),
        sin(rotation.y),
        sin(rotation.x) * cos(rotation.y)
    ));

    // Right vector
    right = glm::normalize(glm::cross(dir, worldUp));

    // Up vector : perpendicular to both direction and right
    up = glm::normalize(glm::cross(right, dir));
}

void Camera::SetFromOptions(Options* options)
{
    fov = options->camFov;
    nearClip = options->camNearClip;
    farClip = options->camFarClip;
    pos = options->camPos;
    dir = options->camLookAt;
    worldUp = options->camUp;
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