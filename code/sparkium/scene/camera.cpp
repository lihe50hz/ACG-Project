#include "sparkium/scene/camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace sparkium {
Camera::Camera() {
}

Camera::~Camera() {
}

glm::mat4 Camera::GetInverseView() const {
  return glm::translate(glm::mat4{1.0f}, position_) *
         glm::rotate(glm::mat4{1.0f}, euler_angles_.y, {0.0f, 1.0f, 0.0f}) *
         glm::rotate(glm::mat4{1.0f}, euler_angles_.x, {1.0f, 0.0f, 0.0f}) *
         glm::rotate(glm::mat4{1.0f}, euler_angles_.z, {0.0f, 0.0f, 1.0f});
}

glm::mat4 Camera::GetView() const {
  return glm::inverse(GetInverseView());
}

glm::mat4 Camera::GetProjection(float aspect) const {
  return glm::perspectiveRH_ZO(fov_, aspect, near_, std::sqrt(near_ * far_));
}

glm::mat4 Camera::GetProjectionFar(float aspect) const {
  return glm::perspectiveRH_ZO(fov_, aspect, std::sqrt(near_ * far_), far_);
}

}  // namespace sparkium
