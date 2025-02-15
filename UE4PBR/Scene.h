#pragma once
#include<map>
using namespace std;
#include"MeshObject.h"
#include"Geometry.h"
#include"Interaction.h"

struct DrawMode
{
	bool isLine;
};
class MyScene
{
private:
	//各种场景信息（相机、材质、灯光、各种物体的各种矩阵）
	map<string, Object> objects;
	//MeshObject cow;
	//灯光
	vec3 lightPos;
	vec3 lightColor;

public:
	//相机
	//MyCamera camera;
	Camera mainCamera;
	//vector<Key> keys;
	map<KEYNAME, Key> keys;

	Mouse mouse;

	DrawMode drawMode;
	//---------------------------------------------------------------------------------

private:

	//shader程序
	ShaderProgram p1;

	ShaderProgram pShadowTex;

public:
	void Init();			//初始化各种信息
	void InitKeys();

	void Update();			//需要动画时，计算各种矩阵（暂时不传入shader中）
	void Draw();			//绘制场景
private:
};