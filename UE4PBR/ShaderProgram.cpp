#include "ShaderProgram.h"

void ShaderProgram::SetMatrix(Transform& t)
{
	world = translate(mat4(1.0), t.position);
	world = scale(world, t.scaler);
	if (t.rotation.x != 0)
		world = rotate(world, t.rotation.x, vec3(1.0, 0.0, 0.0));
	if (t.rotation.y != 0)
		world = rotate(world, t.rotation.y, vec3(0.0, 1.0, 0.0));
	if (t.rotation.z != 0)
		world = rotate(world, t.rotation.z, vec3(0.0, 0.0, 1.0));
	worldInvTranspose = transpose(inverse(world));
	worldViewProj = Camera::getMainCamera()->pro * Camera::getMainCamera()->view * world;
}

void ShaderProgram::InitTexture(GLuint& texID, string texPath)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);		//此时绑定到了默认纹理单元0处，在之后的代码中会指定绑定到具体哪个单元
	//指定贴图方法
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//文件读取
	int width, height;
	unsigned char* pResult = SOIL_load_image(texPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pResult);
	//生成一个mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//解除绑定并释放
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(pResult);
}

void ShaderProgram::InitVertex(MeshData& meshData)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//生成顶点buffer(VBO)并绑定
	//MyGeometryGenerator::GetInstance()->CreateBox(20.0f, 0.3f, 20.0f, box);
	//转换到一维数组中
	std::vector<float> vertexData;
	for (Mesh::FaceIter f_it = meshData.mesh.faces_begin(); f_it != meshData.mesh.faces_end(); f_it++)
	{
		for (Mesh::FaceVertexCCWIter fv_ccwit = meshData.mesh.fv_ccwbegin(*f_it); fv_ccwit != meshData.mesh.fv_ccwend(*f_it); fv_ccwit++)
		{
			vertexData.push_back(meshData.mesh.point(*fv_ccwit).data()[0]);
			vertexData.push_back(meshData.mesh.point(*fv_ccwit).data()[1]);
			vertexData.push_back(meshData.mesh.point(*fv_ccwit).data()[2]);
			vertexData.push_back(meshData.mesh.normal(*fv_ccwit).data()[0]);
			vertexData.push_back(meshData.mesh.normal(*fv_ccwit).data()[1]);
			vertexData.push_back(meshData.mesh.normal(*fv_ccwit).data()[2]);
			if (meshData.providedTex)
			{
				vertexData.push_back(meshData.mesh.texcoord2D(*fv_ccwit).data[0]);
				vertexData.push_back(meshData.mesh.texcoord2D(*fv_ccwit).data[1]);
			}
		}
	}

	//创建顶点buffer
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);					//先绑定，在用VAO传值时，就传送的是当前绑定的buffer
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);		//指定数据
																											//创建索引buffer
	//glGenBuffers(1, &IndexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderIndex.size() * sizeof(unsigned), &renderIndex[0], GL_STATIC_DRAW);

	//将顶点buffer中的数据指定到shader中
	//指定pos数据
	glEnableVertexAttribArray(0);										//开启索引为0的顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);		//第五个代表取数据时的步长，最后一个代表在当前的一个步长内跳过前面多长的信息
																						//指定normal数据
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));		//每8个步长中的中间三个是法向量数据
																										//指定纹理坐标数据
	if (meshData.providedTex)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(0);

}
