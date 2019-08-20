#pragma once
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
using namespace glm;
class Camera
{
private:
	static Camera* MainCamera;
	Camera();
public:
	vec3 eyePos;
	vec3 lookAtPoint;
	vec3 up;
	mat4 view;
	mat4 pro;
	//相机空间的三个向量
	vec3 lookLeft;
	vec3 lookDir;
	//vec3 lookRight;
	//vec3 lookUp;
public:
	static Camera* getMainCamera()
	{
		if (MainCamera == NULL)
		{
			MainCamera = new Camera();
		}
		return MainCamera;
	}
	void Init(vec3 pos, vec3 point);
	void SetView();
	void SetPro();																				//设置透视投影矩阵
	void SetOrtho(float left, float right, float bottom, float up, float near, float far);	//设置正交投影矩阵
public:
	void Walk(float dis);
	void LRMove(float dis);
	void LRRotate(float dis);
};

Camera* Camera::MainCamera = NULL;
