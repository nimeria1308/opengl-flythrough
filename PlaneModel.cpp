#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "PlaneModel.h"

using namespace std;
using namespace glm;

PlaneModel::PlaneModel(unsigned int xRes, unsigned int zRes)
	: xRes(xRes), zRes(zRes), vbo(0), ebo(0), vao(0)
{
	init();
}

void PlaneModel::init()
{
	//procedurally generate vertices for a plane
	float* vertices = new float[xRes * zRes * 3];
	int i = 0;
	for (int row = 0; row < zRes; row++) {
		for (int col = 0; col < zRes; col++) {
			vertices[i++] = (float)col;
			vertices[i++] = 0.0f;
			vertices[i++] = (float)row;
		}
	}

	//generate the indices to draw the plane with triangle strips using multiple glDrawElements calls, see DrawPlane
	int* indices = new int[xRes * 2 * (zRes - 1)];
	i = 0;
	for (int row = 0; row < zRes - 1; row++) {
		if ((row & 1) == 0) { // even rows
			for (int col = 0; col < xRes; col++) {
				indices[i++] = col + row * xRes;
				indices[i++] = col + (row + 1) * xRes;
			}
		}
		else { // odd rows
			for (int col = xRes - 1; col > 0; col--) {
				indices[i++] = col + (row + 1) * xRes;
				indices[i++] = col - 1 + +row * xRes;
			}
		}
	}

	GLuint VAO;
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, xRes * zRes * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, xRes * 2 * (zRes - 1) * sizeof(int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //the data comes from the currently bound GL_ARRAY_BUFFER
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//the elements buffer must be unbound after the vertex array otherwise the vertex array will not have an associated elements buffer array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] vertices;
	delete[] indices;
}

void PlaneModel::render(const mat4& view, const mat4& proj)
{
	Model::render(view, proj);

	glBindVertexArray(vao);

	//glDrawElements uses the indices in the EBO to get to the vertices in the VBO
	//all indices are xRes * 2 * (zRes - 1)
	//a single row of triangle contains xRes * 2 indices (for triangle strip)
	for (int z = 0; z < zRes - 1; z++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, xRes * 2, GL_UNSIGNED_INT, (void*)(z * xRes * 2 * sizeof(int)));
	}

	//this uses multiple glDrawElements calls
	//another option is to use degenerate triangles - see http://www.learnopengles.com/tag/triangle-strips/

	glBindVertexArray(0);
}
