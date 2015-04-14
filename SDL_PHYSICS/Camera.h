#ifndef _CAMERA_H_
#define _CAMERA_H_

/// 
/// Camera.h
/// SDL Assignment 2
/// Class to control view matrices for objects and stuff
/// Created by Joshua Cook -- 2015
/// 

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "SDL_Input.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	inline glm::vec3 getPosition() { return position; };
	inline glm::mat4 getViewMatrix() { return viewMatrix; };
	inline glm::mat4 getProjectionMatrix() { return projectionMatrix; };
	void Update(SDL_Input* input, float dt);

	inline void setFOV(int i) { fov = i; };
	inline void setPosition(glm::vec3 pos) { position = pos; };
	inline void setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); };

private:
	glm::vec3 position;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	
	float speed;
	float mouseSpeed;

	float pitch, yaw;

	float fov;
};

#endif