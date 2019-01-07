#include <cmath>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec3 UP(0.0, 1.0, 0.0);
static constexpr float SENSITIVITY = 0.05;

struct Camera {

    Camera(glm::vec3 pos, double p, double y)
        : position(pos), face(), pitch(p), yaw(y)
    {
        update(0.0, 0.0);
    }

    glm::vec3 position;
    glm::vec3 face;

    double pitch;
    double yaw;

    void update(double d_pitch, double d_yaw);
    glm::mat4 calculate_view_matrix();

};

void Camera::update(double d_pitch, double d_yaw)
{
    pitch -= d_pitch * SENSITIVITY;
    yaw += d_yaw * SENSITIVITY;

    if (pitch > 89.0) {
        pitch = 89.0;
    } else if (pitch < -89.0) {
        pitch = -89.0;
    }

    face.x = ::cos(glm::radians(pitch)) * ::cos(glm::radians(yaw));
    face.y = ::sin(glm::radians(pitch));
    face.z = ::cos(glm::radians(pitch)) * ::sin(glm::radians(yaw));

    face = glm::normalize(face);
}

glm::mat4 Camera::calculate_view_matrix()
{
    return glm::lookAt(position, position + face, UP);
}
