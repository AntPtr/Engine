#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float sen, float speeD, float yaW, float pitcH)
	:cameraPos{ pos }, worldUp{ up }, sens{ sen }, speed{ speeD }, yaw{ yaW }, pitch{ pitcH }, cameraDir{ glm::vec3(0.0f, 0.0f, 1.0f) }
{
	cameraUpdate();
}

void Camera::cameraUpdate() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDir = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	cameraRigth = glm::normalize(glm::cross(cameraDir, worldUp));  
	cameraUp = glm::normalize(glm::cross(cameraRigth, cameraDir));
}

glm::mat4 Camera::getViewMat() {
	return glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
}

void Camera::processKey(Camera_Movement mov, float deltaTime) {
	float cameraSpeed = speed * deltaTime;
	if (mov == FORWARD) {
		cameraPos += cameraDir * cameraSpeed;
	}
	else if (mov == BACKWARD) {
		cameraPos -= cameraDir * cameraSpeed;
	}
	else if (mov == LEFT) {
		cameraPos -= cameraRigth * cameraSpeed;
	}
	else if (mov == RIGHT) {
		cameraPos += cameraRigth * cameraSpeed;
	}
}

void Camera::processMouse(float xoffset, float yoffset, bool lock) {
	xoffset *= sens;
	yoffset *= sens;

	yaw += xoffset;
	pitch += yoffset;

	if (lock) {
		if (pitch > 89.0)
			pitch = 89.0;
		if (pitch < -89.0)
			pitch = -89.0;
	}

	cameraUpdate();
}
