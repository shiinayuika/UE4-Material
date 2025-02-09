#pragma once
//与OpenGLshader配置相关的各种变量和方法
#include<GL/glew.h>
#include<SOIL.h>
#include<OpenMesh/Core/IO/MeshIO.hh>
#include<OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include<string>

#include"Transform.h"
#include"Camera.h"
using namespace std;
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

struct MeshData
{
	Mesh mesh;			//网格数据
	bool providedTex;	//记录文件是否含有纹理坐标
};


class ShaderData
{
private:


public:
	//物体的坐标属性
	mat4x4 world;					//世界矩阵
	mat4x4 worldViewProj;			//最终坐标转换矩阵
	mat4x4 worldInvTranspose;		//用来将法向量转换到世界空间

	//物体的VAO、VBO编号
	GLuint VAO;
	GLuint VertexBuffer;
	GLuint IndexBuffer;

	//物体的贴图编号（不一定全都需要）
	bool bUseTexture;
	GLuint tAlbedo;					//反射贴图（基础颜色）
	bool bAlbedo;
	GLuint tMetallic;				//金属度贴图
	bool bMetallic;
	GLuint tRoughness;				//粗糙贴图
	bool bRoughness;
	GLuint tAo;						//环境贴图
	bool bAo;
	GLuint tNormal;					//法线贴图
	bool bNormal;
	GLuint tShadowTex;				//阴影贴图
	bool bShadowTex;

	float u, v;

public:
	ShaderData()
	{
		world = mat4(0);
		worldViewProj = mat4(0);
		worldInvTranspose = mat4(0);

		bUseTexture = false;
		bAlbedo = false;
		bMetallic = false;
		bRoughness = false;
		bAo = false;
		bNormal = false;
		bShadowTex = false;

		u = 1;
		v = 1;
	}

	void UpdateMatrix(Transform& t, Camera& camera);
	void InitTexture(GLuint& texID, string texPath);		//生成纹理对象并绑定数据(将图片转化为纹理数据，根据不同的ID设置相应的纹理)
	void InitVertexBuffer(MeshData& mesh);		
	void SetUV(float _u, float _v) { u = _u; v = _v; }
};