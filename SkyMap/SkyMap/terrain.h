#ifndef TERRAIN_H
#define TERRAIN_H

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
class Terrain
{
public:
	Terrain(int gridX, int gridZ, const char* texImagePath)
	{
		this->x = gridX * SIZE;
		this->z = gridZ * SIZE;
		//处理贴图
		
		loadTerrain(texImagePath);
	}
	void Draw(Shader &shader)
	{
		//meshes[0].Draw(shader);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	float SIZE = 800;
	int VERTEX_COUNT = 128;
	GLuint VAO;
	GLuint VBO, EBO;
	float x;
	float z;
	Texture texture;
	vector<Mesh>meshes;
	vector<Vertex>vertices;
	vector<unsigned int>indices;
	
	
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
	//void loadTerrain(const char* texImagePath)
	//{
	//	
	//	vector<Texture> textures;
	//	Texture texture;
	//	texture.id = getTextureID(texImagePath);
	//	texture.type = "texture_diffuse";
	//	texture.path = texImagePath;
	//	textures.push_back(texture);

	//	//处理顶点
	//	vector<Vertex>vertices;
	//	int totalVertex = VERTEX_COUNT * VERTEX_COUNT;
	//	float TexTRef = 0.0f;//y方向的分量
	//	for (int i = 0; i < VERTEX_COUNT; i++)
	//	{
	//		for (int j = 0; j < VERTEX_COUNT; j++)
	//		{
	//			//顶点位置赋值
	//			Vertex vertex;
	//			glm::vec3 vector;
	//			vector.x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
	//			vector.y = 0;
	//			vector.z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
	//			vertex.Position = vector;
	//			//顶点法线向量赋值
	//			vector.x = 0;
	//			vector.y = 1;
	//			vector.z = 0;
	//			vertex.Normal = vector;

	//			//贴图坐标赋值
	//			glm::vec2 texVec;

	//			texVec.x= (float)j / (float)(VERTEX_COUNT - 1);
	//			texVec.y= (float)i / (float)(VERTEX_COUNT - 1);
	//			vertex.TexCoords = texVec;
	//			/*vertex.TexCoords.x = (float)j / (float)(VERTEX_COUNT - 1);
	//			vertex.TexCoords.y = (float)i / (float)(VERTEX_COUNT - 1);*/
	//			/*float  TexSRef = 0.0f;
	//			vertex.TexCoords.x = TexSRef;
	//			vertex.TexCoords.y = TexTRef;
	//			TexSRef += 0.1f;*/
	//			//顶点入栈
	//			vertices.push_back(vertex);
	//		}
	//		//TexTRef += 0.1f;
	//		
	//	}

	//	//添加索引
	//	vector<unsigned int>indices;
	//	//int test_i = 0;
	//	for (int gz = 0; gz < VERTEX_COUNT-1; gz++)
	//	{
	//		for (int gx = 0; gx < VERTEX_COUNT-1; gx++)
	//		{
	//			int topLeft = (gz * VERTEX_COUNT) + gx;
	//			int topRight = topLeft + 1;
	//			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
	//			int bottomRight = bottomLeft + 1;
	//			
	//			indices.push_back(topLeft);
	//			indices.push_back(bottomLeft);
	//			indices.push_back(topRight);
	//			indices.push_back(topRight);
	//			indices.push_back(bottomLeft);
	//			indices.push_back(bottomRight);
	//			
	//		}
	//	}
	//	//for (int gz = 0; gz < VERTEX_COUNT ; gz++)
	//	//{
	//	//	for (int gx = 0; gx < VERTEX_COUNT ; gx++)
	//	//	{
	//	//		/*int topLeft = (gz * VERTEX_COUNT) + gx;
	//	//		int topRight = topLeft + 1;
	//	//		int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
	//	//		int bottomRight = bottomLeft + 1;

	//	//		indices.push_back(topLeft);
	//	//		indices.push_back(bottomLeft);
	//	//		indices.push_back(topRight);
	//	//		indices.push_back(topRight);
	//	//		indices.push_back(bottomLeft);
	//	//		indices.push_back(bottomRight);*/
	//	//		indices.push_back(test_i);
	//	//		test_i++;
	//	//	}
	//	//}
	//	//meshes.push_back(processMesh());
	//	Mesh mesh(vertices, indices, textures);
	//	meshes.push_back(mesh);
	//}



void loadTerrain(const char* texImagePath)
{

	//vector<Texture> textures;
	
	texture.id = getTextureID(texImagePath);
	texture.type = "texture_diffuse";
	texture.path = texImagePath;
	//textures.push_back(texture);

	//处理顶点
	
	int totalVertex = VERTEX_COUNT * VERTEX_COUNT;
	//float TexTRef = 0.0f;//y方向的分量
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			//顶点位置赋值
			Vertex vertex;
			glm::vec3 vector;
			vector.x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			vector.y = 0;
			vector.z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			vertex.Position = vector;
			//顶点法线向量赋值
			vector.x = 0;
			vector.y = 1;
			vector.z = 0;
			vertex.Normal = vector;

			//贴图坐标赋值
			glm::vec2 texVec;

			texVec.x = (float)j / (float)(VERTEX_COUNT - 1);
			texVec.y = (float)i / (float)(VERTEX_COUNT - 1);
			vertex.TexCoords = texVec;
			/*vertex.TexCoords.x = (float)j / (float)(VERTEX_COUNT - 1);
			vertex.TexCoords.y = (float)i / (float)(VERTEX_COUNT - 1);*/
			/*float  TexSRef = 0.0f;
			vertex.TexCoords.x = TexSRef;
			vertex.TexCoords.y = TexTRef;
			TexSRef += 0.1f;*/
			//顶点入栈
			vertices.push_back(vertex);
		}
		//TexTRef += 0.1f;

	}

	//添加索引

	//int test_i = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);

		}
	}
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
	//Mesh mesh(vertices, indices, textures);
	//meshes.push_back(mesh);

	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);


	//glBufferData(GL_ARRAY_BUFFER, sizeof(mesh), &mesh, GL_STATIC_DRAW);
}
};

#pragma once
