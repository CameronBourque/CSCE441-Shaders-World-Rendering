#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "MatrixStack.h"

Camera::Camera() :
	aspect(1.0f),
	fovy((float)(45.0*M_PI/180.0)),
	znear(0.1f),
	zfar(1000.0f),
	rotations(0.0, 0.0),
	translations(0.0f, 0.0f, -5.0f),
	rfactor(0.02f),
	tfactor(0.05f),
	sfactor(0.005f),
	position(0.0f, 0.2f, 0.0f),
	yaw(0),
	pitch(0)
{
}

Camera::~Camera()
{
}

void Camera::mouseClicked(float x, float y, bool shift, bool ctrl, bool alt)
{
	mousePrev.x = x;
	mousePrev.y = y;
}

void Camera::mouseMoved(float x, float y)
{
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - mousePrev;
	yaw -= rfactor * dv.x;
	pitch -= rfactor * dv.y;
	if(pitch > M_PI / 3)
    {
	    pitch = M_PI / 3;
    }
	else if(pitch < -M_PI / 3)
    {
	    pitch = -M_PI / 3;
    }
	mousePrev = mouseCurr;
}

void Camera::walk(bool decrement)
{
    glm::vec3 movement(std::sin(yaw) * tfactor, 0, std::cos(yaw) * tfactor);
    if(decrement)
    {
        position = position - movement;
    }
    else
    {
        position = position + movement;
    }
}

void Camera::strafe(bool decrement)
{
    glm::vec3 movement(glm::cross(glm::vec3(std::sin(yaw) * tfactor, 0, std::cos(yaw) * tfactor),
                                  glm::vec3(0.0f, 1.0f, 0.0f)));
    if(decrement)
    {
        position = position - movement;
    }
    else
    {
        position = position + movement;
    }
}

void Camera::zoom(bool decrement)
{
    // Which way are we zooming
    if(decrement && fovy > 4 * M_PI / 180)
    {
        fovy -= sfactor;
    }
    else if(fovy < 114 * M_PI / 180)
    {
        fovy += sfactor;
    }

}

void Camera::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const
{
	// Modify provided MatrixStack
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}

void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const
{
    MV->multMatrix(glm::lookAt(position, position + glm::vec3(std::sin(yaw), pitch, std::cos(yaw)), glm::vec3(0.0, 1.0, 0.0)));
}
