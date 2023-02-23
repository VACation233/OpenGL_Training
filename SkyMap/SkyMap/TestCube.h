#ifndef TESTCUBE_H
#define TESTCUBE_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include<SOIL2/soil2.h>

#include "shader.h"
#include"mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
#endif // !TERRAIN_H
class TestCube
{
public:

	float cubeVertices[72] = {
		// positions          
		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 //0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f, //3 
		//-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		// 0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  //7
		//-0.5f, -0.5f,  0.5f,  

		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		//-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,//11  
		//-0.5f,  0.5f,  0.5f,  

		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		// 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  //15
		// 0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		// 0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  //19
		//-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 //0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  //23
		//-0.5f,  0.5f, -0.5f,  
	};
	float textureCoor[48] = {
		// texture Coord
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//1.0f, 1.0f,
		0.0f, 1.0f,
		//0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//1.0f, 1.0f,
		0.0f, 1.0f,
		//0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//0.0f, 1.0f,
		0.0f, 0.0f,
		//1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		//0.0f, 1.0f,
		0.0f, 0.0f,
		//1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		//1.0f, 0.0f,
		0.0f, 0.0f,
		//0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		//1.0f, 0.0f,
		0.0f, 0.0f,
		//0.0f, 1.0f
	};
	unsigned int cubeIndices[36] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		8,9,10,
		10,11,8,

		12,13,14,
		14,15,12,

		16,17,18,
		18,19,16,

		20,21,22,
		22,23,20
	};
	unsigned int cubeVAO, cubeVBO,TexVBO,cubeEBO, cubeTexture;
	
	TestCube(const char* texImagePath)
	{
		
		//loadTerrain(texImagePath);
		cubeTexture = getTextureID(texImagePath);
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeEBO);
		glGenBuffers(1, &TexVBO);
		glGenBuffers(1, &cubeVBO);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, TexVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoor), &textureCoor, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices, GL_STATIC_DRAW);


		
		


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//vector<Texture> textures;
		//Texture texture;
		//texture.id = getTextureID(texImagePath);
		//texture.type = "texture_diffuse";
		//texture.path = texImagePath;
		//textures.push_back(texture);

		////处理顶点
		//vector<Vertex>vertices;
		//int texIndex = 0;
		//for (int i = 0; i < sizeof(cubeVertices)/sizeof(cubeVertices[0]); )
		//{
		//	Vertex vertex;
		//	glm::vec3 vector;
		//	vector.x = (float)cubeVertices[i++];
		//	vector.y = (float)cubeVertices[i++];
		//	vector.z = (float)cubeVertices[i++];
		//	vertex.Position = vector;

		//	//法线向量赋值
		//	vector = glm::vec3(0.0f, 1.0f, 0.0f);
		//	vertex.Normal = vector;

		//	//贴图坐标赋值
		//	vertex.TexCoords.x = textureCoor[texIndex++];
		//	vertex.TexCoords.y = textureCoor[texIndex++];

		//	vertices.push_back(vertex);
		//}
		//
		////添加索引
		//vector<unsigned int>indices;
		//for (int i = 0; i < sizeof(cubeIndices)/sizeof(cubeIndices[0]); i++)
		//{
		//	indices.push_back(cubeIndices[i]);
		//}
		//int test_i = 0;
		//for (int gz = 0; gz < VERTEX_COUNT ; gz++)
		//{
		//	for (int gx = 0; gx < VERTEX_COUNT ; gx++)
		//	{
		//		/*int topLeft = (gz * VERTEX_COUNT) + gx;
		//		int topRight = topLeft + 1;
		//		int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
		//		int bottomRight = bottomLeft + 1;

		//		indices.push_back(topLeft);
		//		indices.push_back(bottomLeft);
		//		indices.push_back(topRight);
		//		indices.push_back(topRight);
		//		indices.push_back(bottomLeft);
		//		indices.push_back(bottomRight);*/
		//		indices.push_back(test_i);
		//		test_i++;
		//	}
		//}
		//meshes.push_back(processMesh());
		/*Mesh mesh(vertices, indices, textures);
		meshes.push_back(mesh);*/
		
	}
	void Draw(Shader& shader)
	{
		// cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
		glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

		//meshes[0].Draw(shader);
	}
private:
	vector<Mesh>meshes;
	
	



	//处理贴图

	unsigned int getTextureID(const char* texImagePath)
	{
		GLuint terrainTexID;


		terrainTexID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		//是否读取到贴图图片
		if (terrainTexID == 0)
		{
			cout << "didnt find texture file " << texImagePath << endl;
		}

		glBindTexture(GL_TEXTURE_2D, terrainTexID);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return terrainTexID;
	}


	////用于处理网格
	//Mesh processMesh()
	//{

	//}
	void loadTerrain(const char* texImagePath)
	{
		vector<Texture> textures;
		Texture texture;
		texture.id = getTextureID(texImagePath);
		texture.type = "texture_diffuse";
		texture.path = texImagePath;
		textures.push_back(texture);

		

		

		
		
	}
};

#pragma once
