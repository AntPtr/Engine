#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	glm::vec3 cameraRigth;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;
	float speed;
	float yaw;
	float pitch;
	float sens;
	Camera(glm::vec3 pos, glm::vec3 up, float sen = SENSITIVITY, float speeD = SPEED, float yaW = YAW, float pitcH = PITCH);
	glm::mat4 getViewMat();
	void processKey(Camera_Movement mov, float deltaTime);
	void processMouse(float xoffset, float yoffset, bool lock = true);
private:
	void cameraUpdate();
};



