///////////////////////////////////////////////////////////////////////////////
// ShapeMeshes.cpp
// ========
// create meshes for various 3D primitives: 
//		box, cone, cylinder, plane, prism, sphere, taperedcylinder, torus
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 7th, 2022
///////////////////////////////////////////////////////////////////////////////

#include "shapemeshes.h"

// GLM Math Header inclusions
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array> // Required for std::array
#include <vector> // Required for std::vector
#include <cmath>  // Required for math functions like sqrt and cos

#include <iostream>

namespace Constants
{
	constexpr double Pi = 3.141592653589793;       // Use constexpr for compile-time evaluation
	constexpr double PiHalf = Pi / 2.0;           // Use computed value for better accuracy
	constexpr GLuint FloatsPerVertex = 3;         // Number of coordinates per vertex
	constexpr GLuint FloatsPerNormal = 3;         // Number of components per normal vector
	constexpr GLuint FloatsPerUV = 2;             // Number of texture coordinate values
}

using namespace Constants;

ShapeMeshes::ShapeMeshes()
{
	m_bMemoryLayoutDone = false;
}

//**************************************************************************
// The following set of methods are called to load the vertices, normals, texture
// coordinates for the various basic 3D shapes into memory in preparation of
// drawing the shapes in OpenGL.
//**************************************************************************

///////////////////////////////////////////////////
// LoadBoxMesh()
//
// Creates a box mesh by specifying the vertices and 
// stores it in a VAO/VBO. Normals and texture
// coordinates are also set.
//
// Correct triangle drawing command:
//
// glDrawElements(GL_TRIANGLES, meshes.gBoxMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
///////////////////////////////////////////////////
void ShapeMeshes::LoadBoxMesh()
{
	// Box vertex and index data
	const std::vector<GLfloat> verts = {
		// Positions           // Normals          // Texture Coords
		// Back Face
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,  // 0
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,  // 1
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,  // 2
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,  // 3
		// Bottom Face
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,  // 4
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,  // 5
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,  // 6
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,  // 7
		 // Left Face
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,  // 8
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,  // 9
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,  // 10
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,  // 11
		// Right Face
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,  // 12
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,  // 13
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,  // 14
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,  // 15
		// Top Face
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,  // 16
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,  // 17
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,  // 18
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,  // 19
		// Front Face
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,  // 20
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,  // 21
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,  // 22
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f   // 23
	};

	// Index data
	const std::vector<GLuint> indices = {
			0, 1, 2, 0, 3, 2,       // Back Face
			4, 5, 6, 4, 7, 6,       // Bottom Face
			8, 9, 10, 8, 11, 10,    // Left Face
			12, 13, 14, 12, 15, 14, // Right Face
			16, 17, 18, 16, 19, 18, // Top Face
			20, 21, 22, 20, 23, 22  // Front Face
	};

	// Calculate vertex and index counts
	m_BoxMesh.nVertices = verts.size() / (FloatsPerVertex + FloatsPerNormal + FloatsPerUV);
	m_BoxMesh.nIndices = indices.size();

	// Generate VAO and VBOs
	glGenVertexArrays(1, &m_BoxMesh.vao);
	glBindVertexArray(m_BoxMesh.vao);

	glGenBuffers(2, m_BoxMesh.vbos);

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_BoxMesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);


	// Upload index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BoxMesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Ensure shader memory layout is set
	if (!m_bMemoryLayoutDone) {
		SetShaderMemoryLayout();
	}
}

///////////////////////////////////////////////////
//	LoadConeMesh()
//
//	Create a cone mesh by specifying the vertices and 
//  store it in a VAO/VBO. The normals and texture
//  coordinates are also set.
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, numSlices + 2);	// bottom
//	glDrawArrays(GL_TRIANGLE_STRIP, numSlices + 2, numSlices * 2);	// sides
///////////////////////////////////////////////////
void ShapeMeshes::LoadConeMesh(float radius, float height, int numSlices) {
	// Validate inputs
	if (numSlices < 3) numSlices = 3;
	m_ConeMesh.numSlices = numSlices; // Store number of slices in the mesh structure

	std::vector<GLfloat> vertices;

	// Generate bottom circle vertices
	float angleStep = 2.0f * Pi / numSlices;

	// Center vertex of bottom circle
	vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f });

	for (int i = 0; i <= numSlices; ++i) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		vertices.insert(vertices.end(), { x, 0.0f, z, 0.0f, -1.0f, 0.0f, 0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle) });
	}

	// Generate side vertices
	for (int i = 0; i <= numSlices; ++i) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		float nx = cos(angle);
		float nz = sin(angle);

		// Bottom vertex
		vertices.insert(vertices.end(), { x, 0.0f, z, nx, 0.0f, nz, static_cast<float>(i) / numSlices, 1.0f });
		// Apex vertex
		vertices.insert(vertices.end(), { 0.0f, height, 0.0f, nx, 0.0f, nz, static_cast<float>(i) / numSlices, 0.0f });
	}

	// Store vertex count
	m_ConeMesh.nVertices = static_cast<GLsizei>(vertices.size() / (FloatsPerVertex + FloatsPerNormal + FloatsPerUV));
	m_ConeMesh.nIndices = 0; // Not used since we're drawing with glDrawArrays

	// Generate VAO and VBO
	glGenVertexArrays(1, &m_ConeMesh.vao);
	glBindVertexArray(m_ConeMesh.vao);

	glGenBuffers(1, m_ConeMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ConeMesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	if (!m_bMemoryLayoutDone) {
		SetShaderMemoryLayout();
	}

	// Unbind VAO for safety
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	LoadCylinderMesh()
//
//	Create a cylinder mesh by specifying the vertices and 
//  store it in a VAO/VBO.  The normals and texture
//  coordinates are also set.
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_FAN, 36, 36);		//top
//	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
///////////////////////////////////////////////////

void ShapeMeshes::LoadCylinderMesh(float radius, float height, int numSlices) {
	// Validate inputs
	if (numSlices < 3) numSlices = 3;
	m_CylinderMesh.numSlices = numSlices; // Store number of slices in the mesh structure

	std::vector<GLfloat> vertices;

	// Generate bottom circle vertices
	float angleStep = 2.0f * Pi / numSlices;

	// Center vertex of bottom circle
	vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f });

	for (int i = 0; i <= numSlices; ++i) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		vertices.insert(vertices.end(), { x, 0.0f, z, 0.0f, -1.0f, 0.0f, 0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle) });
	}

	// Generate top circle vertices
	vertices.insert(vertices.end(), { 0.0f, height, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f });

	for (int i = 0; i <= numSlices; ++i) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		vertices.insert(vertices.end(), { x, height, z, 0.0f, 1.0f, 0.0f, 0.5f + 0.5f * cos(angle), 0.5f + 0.5f * sin(angle) });
	}

	// Generate side vertices
	for (int i = 0; i <= numSlices; ++i) {
		float angle = i * angleStep;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		float nx = cos(angle);
		float nz = sin(angle);

		// Bottom vertex
		vertices.insert(vertices.end(), { x, 0.0f, z, nx, 0.0f, nz, static_cast<float>(i) / numSlices, 0.0f });
		// Top vertex
		vertices.insert(vertices.end(), { x, height, z, nx, 0.0f, nz, static_cast<float>(i) / numSlices, 1.0f });
	}

	// Store vertex count
	m_CylinderMesh.nVertices = static_cast<GLsizei>(vertices.size() / (FloatsPerVertex + FloatsPerNormal + FloatsPerUV));
	m_CylinderMesh.nIndices = 0; // Not used since we're drawing with glDrawArrays

	// Generate VAO and VBO
	glGenVertexArrays(1, &m_CylinderMesh.vao);
	glBindVertexArray(m_CylinderMesh.vao);

	glGenBuffers(1, m_CylinderMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_CylinderMesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	if (!m_bMemoryLayoutDone) {
		SetShaderMemoryLayout();
	}

	// Unbind VAO for safety
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	LoadPlaneMesh()
//
//	Create a plane mesh by specifying the vertices and 
//  store it in a VAO/VBO.  The normals and texture
//  coordinates are also set.
// 
//  Correct triangle drawing command:
//
//	glDrawElements(GL_TRIANGLES, meshes.gPlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
///////////////////////////////////////////////////

void ShapeMeshes::LoadPlaneMesh(float width, float height) {
	// Half dimensions for centering the plane
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	// Vertex data: Positions, Normals, Texture Coords
	GLfloat verts[] = {
		// Vertex Positions       // Normals           // Texture Coords
		-halfWidth, 0.0f, halfHeight,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // Bottom-left
		 halfWidth, 0.0f, halfHeight,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
		 halfWidth, 0.0f, -halfHeight, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // Top-right
		-halfWidth, 0.0f, -halfHeight, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f   // Top-left
	};

	// Index data
	GLuint indices[] = {
		0, 1, 2,  // First triangle
		0, 2, 3   // Second triangle
	};

	// Store vertex and index count
	m_PlaneMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (FloatsPerVertex + FloatsPerNormal + FloatsPerUV));
	m_PlaneMesh.nIndices = sizeof(indices) / sizeof(indices[0]);

	// Generate the VAO for the mesh
	glGenVertexArrays(1, &m_PlaneMesh.vao);
	glBindVertexArray(m_PlaneMesh.vao); // Activate the VAO

	// Create VBOs for the mesh
	glGenBuffers(2, m_PlaneMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_PlaneMesh.vbos[0]); // Activate the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Send data to the GPU

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_PlaneMesh.vbos[1]); // Activate the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	if (!m_bMemoryLayoutDone) {
		SetShaderMemoryLayout();
	}

	// Unbind the VAO for safety
	glBindVertexArray(0);
}

void ShapeMeshes::LoadPrismMesh()
{
	// Vertex data
	GLfloat verts[] = {
		//Positions				//Normals
		// ------------------------------------------------------

		//Back Face				//Negative Z Normal  
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,

		//Bottom Face			//Negative Y Normal
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,

		//Left Face/slanted		//Normals
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, 0.5f,  -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 1.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,
		0.0f, -0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 0.0f,
		0.0f, 0.5f,  0.5f,		0.894427180f,  0.0f,  -0.447213590f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.894427180f,  0.0f,  -0.447213590f,	0.0f, 0.0f,

		//Right Face/slanted	//Normals
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,
		0.0f, -0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		-0.894427180f,  0.0f,  -0.447213590f,		0.0f, 1.0f,

		//Top Face				//Positive Y Normal		//Texture Coords.
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		0.0f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		0.5f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		0.0f, 0.0f,

	};

	m_PrismMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (FloatsPerVertex + FloatsPerNormal + FloatsPerUV));

	glGenVertexArrays(1, &m_PrismMesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(m_PrismMesh.vao);

	// Create 2 buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, m_PrismMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_PrismMesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	if (m_bMemoryLayoutDone == false)
	{
		SetShaderMemoryLayout();
	}
}


///////////////////////////////////////////////////
// LoadPyramid3Mesh()
//
// Dynamically create a 3-sided pyramid mesh by specifying
// vertices and store it in a VAO/VBO. The normals and
// texture coordinates are also set.
//
// Correct triangle drawing command:
//
// glDrawArrays(GL_TRIANGLE_STRIP, 0, gPyramid3Mesh.nVertices);
///////////////////////////////////////////////////
void ShapeMeshes::LoadPyramid3Mesh()
{
	constexpr float halfBase = 0.5f; // Half the length of the base
	constexpr float height = 0.5f;  // Height of the pyramid

	// Define vertices programmatically
	std::vector<GLfloat> verts;

	// Helper for normals
	auto calculateNormal = [](float x1, float y1, float z1, float x2, float y2, float z2) -> std::array<float, 3> {
		float nx = y1 * z2 - z1 * y2;
		float ny = z1 * x2 - x1 * z2;
		float nz = x1 * y2 - y1 * x2;
		float length = sqrt(nx * nx + ny * ny + nz * nz);
		return { nx / length, ny / length, nz / length };
		};

	// Define the pyramid faces with vertices and normals
	struct Face {
		std::array<float, 3> top;      // Top vertex
		std::array<float, 3> bottom1; // First base vertex
		std::array<float, 3> bottom2; // Second base vertex
		std::array<float, 3> normal;  // Normal vector
	};

	std::vector<Face> faces = {
		// Left face
		{{0.0f, height, 0.0f}, {-halfBase, -height, halfBase}, {0.0f, -height, -halfBase},
		 calculateNormal(-halfBase, -height - height, halfBase - 0.0f, 0.0f, -height - height, -halfBase - halfBase)},
		 // Right face
		 {{0.0f, height, 0.0f}, {0.0f, -height, -halfBase}, {halfBase, -height, halfBase},
		  calculateNormal(0.0f, -height - height, -halfBase - 0.0f, halfBase, -height - height, halfBase - -halfBase)},
		  // Front face
		  {{0.0f, height, 0.0f}, {halfBase, -height, halfBase}, {-halfBase, -height, halfBase},
		   calculateNormal(halfBase, -height - height, halfBase - 0.0f, -halfBase, -height - height, halfBase - halfBase)} };

	for (const auto& face : faces)
	{
		// Top point
		verts.insert(verts.end(), {
			face.top[0], face.top[1], face.top[2],
			face.normal[0], face.normal[1], face.normal[2],
			0.5f, 1.0f });

		// First base vertex
		verts.insert(verts.end(), {
			face.bottom1[0], face.bottom1[1], face.bottom1[2],
			face.normal[0], face.normal[1], face.normal[2],
			0.0f, 0.0f });

		// Second base vertex
		verts.insert(verts.end(), {
			face.bottom2[0], face.bottom2[1], face.bottom2[2],
			face.normal[0], face.normal[1], face.normal[2],
			1.0f, 0.0f });
	}

	// Base (bottom face)
	verts.insert(verts.end(), {
		-halfBase, -height, halfBase, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		halfBase, -height, halfBase, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, -height, -halfBase, 0.0f, -1.0f, 0.0f, 0.5f, 0.0f });

	// Store vertex count
	m_Pyramid3Mesh.nVertices = verts.size() / (FloatsPerVertex + FloatsPerNormal + FloatsPerUV);

	// Create VAO
	glGenVertexArrays(1, &m_Pyramid3Mesh.vao);
	glBindVertexArray(m_Pyramid3Mesh.vao);

	// Create VBO
	glGenBuffers(1, m_Pyramid3Mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_Pyramid3Mesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);

	if (!m_bMemoryLayoutDone)
	{
		SetShaderMemoryLayout();
	}
}

///////////////////////////////////////////////////
// LoadPyramid4Mesh()
//
// Dynamically create a 4-sided pyramid mesh by
// specifying vertices, normals, and texture coordinates.
//
///////////////////////////////////////////////////
void ShapeMeshes::LoadPyramid4Mesh(float baseSize, float height)
{
	constexpr int FloatsPerVertex = 3;
	constexpr int FloatsPerNormal = 3;
	constexpr int FloatsPerUV = 2;

	float halfBase = baseSize / 2.0f;

	// Vertex data container
	std::vector<GLfloat> verts;

	// Helper lambda to add vertex data
	auto addVertex = [&verts](float px, float py, float pz, float nx, float ny, float nz, float u, float v) {
		verts.insert(verts.end(), { px, py, pz, nx, ny, nz, u, v });
		};

	// Helper for normal calculation
	auto calculateNormal = [](float x1, float y1, float z1, float x2, float y2, float z2) -> std::array<float, 3> {
		float nx = y1 * z2 - z1 * y2;
		float ny = z1 * x2 - x1 * z2;
		float nz = x1 * y2 - y1 * x2;
		float length = std::sqrt(nx * nx + ny * ny + nz * nz);
		return { nx / length, ny / length, nz / length };
		};

	// Bottom face (flat quad)
	addVertex(-halfBase, -halfBase, halfBase, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);  // Front-left
	addVertex(-halfBase, -halfBase, -halfBase, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f); // Back-left
	addVertex(halfBase, -halfBase, -halfBase, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);  // Back-right
	addVertex(halfBase, -halfBase, halfBase, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);   // Front-right

	// Pyramid faces (triangular sides)
	struct Face {
		std::array<float, 3> top;
		std::array<float, 3> bottomLeft;
		std::array<float, 3> bottomRight;
	};

	std::vector<Face> faces = {
		{{0.0f, height / 2.0f, 0.0f}, {-halfBase, -halfBase, -halfBase}, {-halfBase, -halfBase, halfBase}},  // Left face
		{{0.0f, height / 2.0f, 0.0f}, {halfBase, -halfBase, -halfBase}, {-halfBase, -halfBase, -halfBase}}, // Back face
		{{0.0f, height / 2.0f, 0.0f}, {halfBase, -halfBase, halfBase}, {halfBase, -halfBase, -halfBase}},   // Right face
		{{0.0f, height / 2.0f, 0.0f}, {-halfBase, -halfBase, halfBase}, {halfBase, -halfBase, halfBase}}    // Front face
	};

	for (const auto& face : faces)
	{
		// Calculate normal for the face
		auto normal = calculateNormal(
			face.bottomRight[0] - face.bottomLeft[0], face.bottomRight[1] - face.bottomLeft[1],
			face.bottomRight[2] - face.bottomLeft[2], face.top[0] - face.bottomLeft[0],
			face.top[1] - face.bottomLeft[1], face.top[2] - face.bottomLeft[2]);

		// Add vertices for the triangular face
		addVertex(face.top[0], face.top[1], face.top[2], normal[0], normal[1], normal[2], 0.5f, 1.0f);         // Top vertex
		addVertex(face.bottomLeft[0], face.bottomLeft[1], face.bottomLeft[2], normal[0], normal[1], normal[2], 0.0f, 0.0f); // Bottom-left
		addVertex(face.bottomRight[0], face.bottomRight[1], face.bottomRight[2], normal[0], normal[1], normal[2], 1.0f, 0.0f); // Bottom-right
	}

	// Store vertex count
	m_Pyramid4Mesh.nVertices = verts.size() / (FloatsPerVertex + FloatsPerNormal + FloatsPerUV);

	// Generate VAO and VBO
	glGenVertexArrays(1, &m_Pyramid4Mesh.vao);
	glBindVertexArray(m_Pyramid4Mesh.vao);

	glGenBuffers(1, m_Pyramid4Mesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_Pyramid4Mesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);

	// Set shader memory layout if not done
	if (!m_bMemoryLayoutDone)
	{
		SetShaderMemoryLayout();
	}

	// Unbind VAO for safety
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// LoadSphereMesh()
//
// Dynamically generate a sphere mesh with the given
// latitude and longitude segment counts. Store it in
// a VAO/VBO, including normals and texture coordinates.
///////////////////////////////////////////////////
void ShapeMeshes::LoadSphereMesh(int latitudeSegments, int longitudeSegments, float radius)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	// Generate vertices, normals, and texture coordinates
	for (int lat = 0; lat <= latitudeSegments; ++lat)
	{
		float theta = lat * Pi / latitudeSegments; // Latitude angle [0, PI]
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon <= longitudeSegments; ++lon)
		{
			float phi = lon * 2 * Pi / longitudeSegments; // Longitude angle [0, 2*PI]
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			// Compute vertex position
			float x = radius * sinTheta * cosPhi;
			float y = radius * cosTheta;
			float z = radius * sinTheta * sinPhi;

			// Compute normal
			float nx = sinTheta * cosPhi;
			float ny = cosTheta;
			float nz = sinTheta * sinPhi;

			// Compute texture coordinates
			float u = 1.0f - (float)lon / longitudeSegments;
			float v = 1.0f - (float)lat / latitudeSegments;

			// Push vertex data
			vertices.push_back(x); // Position
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(nx); // Normal
			vertices.push_back(ny);
			vertices.push_back(nz);
			vertices.push_back(u); // Texture coordinate
			vertices.push_back(v);
		}
	}

	// Generate indices for GL_TRIANGLES
	for (int lat = 0; lat < latitudeSegments; ++lat)
	{
		for (int lon = 0; lon < longitudeSegments; ++lon)
		{
			int first = lat * (longitudeSegments + 1) + lon;
			int second = first + longitudeSegments + 1;

			// Triangle 1
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			// Triangle 2
			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	// Store vertex and index count
	m_SphereMesh.nVertices = static_cast<GLuint>(vertices.size() / 8); // 8 floats per vertex
	m_SphereMesh.nIndices = static_cast<GLuint>(indices.size());

	// Create VAO
	glGenVertexArrays(1, &m_SphereMesh.vao);
	glBindVertexArray(m_SphereMesh.vao);

	// Create VBO for vertices
	glGenBuffers(1, &m_SphereMesh.vbos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_SphereMesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	// Create EBO for indices
	glGenBuffers(1, &m_SphereMesh.vbos[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SphereMesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Ensure shader memory layout is set
	if (!m_bMemoryLayoutDone)
	{
		SetShaderMemoryLayout();
	}

	// Unbind VAO for safety
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	LoadTaperedCylinderMesh()
//
//	Create a tapered cylinder mesh by specifying the 
//  vertices and store it in a VAO/VBO.  The normals 
//  and texture coordinates are also set.
//
//  Correct triangle drawing commands:
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 36);		//bottom
//	glDrawArrays(GL_TRIANGLE_FAN, 36, 72);		//top
//	glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
///////////////////////////////////////////////////
void ShapeMeshes::LoadTaperedCylinderMesh()
{
	GLfloat verts[] = {
		// cylinder bottom		// normals			// texture coords
		1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f,1.0f,
		.98f, 0.0f, -0.17f,		0.0f, -1.0f, 0.0f,	0.41f, 0.983f,
		.94f, 0.0f, -0.34f,		0.0f, -1.0f, 0.0f,	0.33f, 0.96f,
		.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.92f,
		.77f, 0.0f, -0.64f,		0.0f, -1.0f, 0.0f,	0.17f, 0.87f,
		.64f, 0.0f, -0.77f,		0.0f, -1.0f, 0.0f,	0.13f, 0.83f,
		.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.77f,
		.34f, 0.0f, -0.94f,		0.0f, -1.0f, 0.0f,	0.04f, 0.68f,
		.17f, 0.0f, -0.98f,		0.0f, -1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 0.0f, -1.0f,		0.0f, -1.0f, 0.0f,	0.0f,0.5f,
		-.17f, 0.0f, -0.98f,	0.0f, -1.0f, 0.0f,	0.017f, 0.41f,
		-.34f, 0.0f, -0.94f,	0.0f, -1.0f, 0.0f,	0.04f, 0.33f,
		-.5f, 0.0f, -0.87f,		0.0f, -1.0f, 0.0f,	0.08f, 0.25f,
		-.64f, 0.0f, -0.77f,	0.0f, -1.0f, 0.0f,	0.13f, 0.17f,
		-.77f, 0.0f, -0.64f,	0.0f, -1.0f, 0.0f,	0.17f, 0.13f,
		-.87f, 0.0f, -0.5f,		0.0f, -1.0f, 0.0f,	0.25f, 0.08f,
		-.94f, 0.0f, -0.34f,	0.0f, -1.0f, 0.0f,	0.33f, 0.04f,
		-.98f, 0.0f, -0.17f,	0.0f, -1.0f, 0.0f,	0.41f, 0.017f,
		-1.0f, 0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	0.5f, 0.0f,
		-.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.017f,
		-.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.04f,
		-.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.08f,
		-.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.13f,
		-.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.17f,
		-.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.25f,
		-.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.33f,
		-.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 0.0f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.5f,
		.17f, 0.0f, 0.98f,		0.0f, -1.0f, 0.0f,	0.983f, 0.6f,
		.34f, 0.0f, 0.94f,		0.0f, -1.0f, 0.0f,	0.96f, 0.68f,
		.5f, 0.0f, 0.87f,		0.0f, -1.0f, 0.0f,	0.92f, 0.77f,
		.64f, 0.0f, 0.77f,		0.0f, -1.0f, 0.0f,	0.87f, 0.83f,
		.77f, 0.0f, 0.64f,		0.0f, -1.0f, 0.0f,	0.83f, 0.87f,
		.87f, 0.0f, 0.5f,		0.0f, -1.0f, 0.0f,	0.77f, 0.92f,
		.94f, 0.0f, 0.34f,		0.0f, -1.0f, 0.0f,	0.68f, 0.96f,
		.98f, 0.0f, 0.17f,		0.0f, -1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder top			// normals			// texture coords
		0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f,1.0f,
		.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.983f,
		.47f, 1.0f, -0.17f,		0.0f, 1.0f, 0.0f,	0.33f, 0.96f,
		.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.92f,
		.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.87f,
		.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.83f,
		.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.77f,
		.17f, 1.0f, -0.47f,		0.0f, 1.0f, 0.0f,	0.04f, 0.68f,
		.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.6f,
		0.0f, 1.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f,0.5f,
		-.085f, 1.0f, -0.49f,	0.0f, 1.0f, 0.0f,	0.017f, 0.41f,
		-.17f, 1.0f, -0.47f,	0.0f, 1.0f, 0.0f,	0.04f, 0.33f,
		-.25f, 1.0f, -0.435f,	0.0f, 1.0f, 0.0f,	0.08f, 0.25f,
		-.32f, 1.0f, -0.385f,	0.0f, 1.0f, 0.0f,	0.13f, 0.17f,
		-.385f, 1.0f, -0.32f,	0.0f, 1.0f, 0.0f,	0.17f, 0.13f,
		-.435f, 1.0f, -0.25f,	0.0f, 1.0f, 0.0f,	0.25f, 0.08f,
		-.47f, 1.0f, -0.17f,	0.0f, 1.0f, 0.0f,	0.33f, 0.04f,
		-.49f, 1.0f, -0.085f,	0.0f, 1.0f, 0.0f,	0.41f, 0.017f,
		-0.5f, 1.0f, 0.0f,		0.0f, 1.0f, 0.0f,	0.5f, 0.0f,
		-.49f, 1.0f, 0.085f,	0.0f, 1.0f, 0.0f,	0.6f, 0.017f,
		-.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.04f,
		-.435f, 1.0f, 0.25f,	0.0f, 1.0f, 0.0f,	0.77f, 0.08f,
		-.385f, 1.0f, 0.32f,	0.0f, 1.0f, 0.0f,	0.83f, 0.13f,
		-.32f, 1.0f, 0.385f,	0.0f, 1.0f, 0.0f,	0.87f, 0.17f,
		-.25f, 1.0f, 0.435f,	0.0f, 1.0f, 0.0f,	0.92f, 0.25f,
		-.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.33f,
		-.085f, 1.0f, 0.49f,	0.0f, 1.0f, 0.0f,	0.983f, 0.41f,
		0.0f, 1.0f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.5f,
		.085f, 1.0f, 0.49f,		0.0f, 1.0f, 0.0f,	0.983f, 0.6f,
		.17f, 1.0f, 0.47f,		0.0f, 1.0f, 0.0f,	0.96f, 0.68f,
		.25f, 1.0f, 0.435f,		0.0f, 1.0f, 0.0f,	0.92f, 0.77f,
		.32f, 1.0f, 0.385f,		0.0f, 1.0f, 0.0f,	0.87f, 0.83f,
		.385f, 1.0f, 0.32f,		0.0f, 1.0f, 0.0f,	0.83f, 0.87f,
		.435f, 1.0f, 0.25f,		0.0f, 1.0f, 0.0f,	0.77f, 0.92f,
		.47f, 1.0f, 0.17f,		0.0f, 1.0f, 0.0f,	0.68f, 0.96f,
		.49f, 1.0f, 0.085f,		0.0f, 1.0f, 0.0f,	0.6f, 0.983f,

		// cylinder body		// normals							// texture coords
		0.5f, 1.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f,	0.25,1.0,
		1.0f, 0.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f,	0.0,0.0,
		.98f, 0.0f, -0.17f,		0.993150651, 0.5f, -0.116841137f,	0.0277,0.0,
		0.5f, 1.0f, 0.0f,		0.993150651, 0.5f, -0.116841137f, 	0.25,1.0,
		.49f, 1.0f, -0.085f,	0.993150651, 0.5f, -0.116841137f, 	0.2635,1.0,
		.98f, 0.0f, -0.17f,		0.993150651, 0.5f, -0.116841137f,	0.0277,0.0,
		.94f, 0.0f, -0.34f,		0.993417103f, 0.5f, -0.229039446f,	0.0554,0.0,
		.49f, 1.0f, -0.085f,	0.993417103f, 0.5f, -0.229039446f,	0.2635,1.0,
		.47f, 1.0f, -0.17f,		0.993417103f, 0.5f, -0.229039446f,	0.277,1.0,
		.94f, 0.0f, -0.34f,		0.993417103f, 0.5f, -0.229039446f,	0.0554,0.0,
		.87f, 0.0f, -0.5f,		0.993417103f, 0.5f, -0.229039446f,	0.0831,0.0,
		.47f, 1.0f, -0.17f,		0.993417103f, 0.5f, -0.229039446f,	0.277,1.0,
		.435f, 1.0f, -0.25f,	0.813733339f, 0.5f, -0.581238329f,	0.2905,1.0,
		.87f, 0.0f, -0.5f,		0.813733339f, 0.5f, -0.581238329f,	0.0831,0.0,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.5f, -0.581238329f,	0.1108,0.0,
		.435f, 1.0f, -0.25f,	0.813733339f, 0.5f, -0.581238329f,	0.2905,1.0,
		.385f, 1.0f, -0.32f,	0.813733339f, 0.5f, -0.581238329f,	0.304,1.0,
		.77f, 0.0f, -0.64f,		0.813733339f, 0.5f, -0.581238329f,	0.1108,0.0,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.5f, -0.707106769f,	0.1385,0.0,
		.385f, 1.0f, -0.32f,	0.707106769f, 0.5f, -0.707106769f,	0.304,1.0,
		.32f, 1.0f, -0.385f,	0.707106769f, 0.5f, -0.707106769f,	0.3175,1.0,
		.64f, 0.0f, -0.77f,		0.707106769f, 0.5f, -0.707106769f,	0.1385,0.0,
		.5f, 0.0f, -0.87f,		0.707106769f, 0.5f, -0.707106769f,	0.1662,0.0,
		.32f, 1.0f, -0.385f,	0.707106769f, 0.5f, -0.707106769f,	0.3175, 1.0,
		.25f, 1.0f, -0.435f,	0.400818795f, 0.5f, -0.916157305f,	0.331, 1.0,
		.5f, 0.0f, -0.87f,		0.400818795f, 0.5f, -0.916157305f,	0.1662, 0.0,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.5f, -0.916157305f,	0.1939, 0.0,
		.25f, 1.0f, -0.435f,	0.400818795f, 0.5f, -0.916157305f,	0.331, 1.0,
		.17f, 1.0f, -0.47f,		0.400818795f, 0.5f, -0.916157305f,	0.3445, 1.0,
		.34f, 0.0f, -0.94f,		0.400818795f, 0.5f, -0.916157305f,	0.1939, 0.0,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.5f, -0.973417103f,	0.2216, 0.0,
		.17f, 1.0f, -0.47f,		0.229039446f, 0.5f, -0.973417103f,	0.3445, 1.0,
		.085f, 1.0f, -0.49f,	0.229039446f, 0.5f, -0.973417103f,	0.358, 1.0,
		.17f, 0.0f, -0.98f,		0.229039446f, 0.5f, -0.973417103f,	0.2216, 0.0,
		0.0f, 0.0f, -1.0f,		0.229039446f, 0.5f, -0.973417103f,	0.2493, 0.0,
		.085f, 1.0f, -0.49f,	0.229039446f, 0.5f, -0.973417103f,	0.358, 1.0,
		0.0f, 1.0f, -0.5f,		-0.116841137f, 0.5f, -0.993150651f,	0.3715, 1.0,
		0.0f, 0.0f, -1.0f,		-0.116841137f, 0.5f, -0.993150651f,	0.2493, 0.0,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.5f, -0.993150651f,	0.277, 0.0,
		0.0f, 1.0f, -0.5f,		-0.116841137f, 0.5f, -0.993150651f,	0.3715, 1.0,
		-.085f, 1.0f, -0.49f,	-0.116841137f, 0.5f, -0.993150651f,	0.385, 1.0,
		-.17f, 0.0f, -0.98f,	-0.116841137f, 0.5f, -0.993150651f,	0.277, 0.0,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.5f, -0.973417103f,	0.3047, 0.0,
		-.085f, 1.0f, -0.49f,	-0.229039446f, 0.5f, -0.973417103f,	0.385, 1.0,
		-.17f, 1.0f, -0.47f,	-0.229039446f, 0.5f, -0.973417103f,	0.3985, 1.0,
		-.34f, 0.0f, -0.94f,	-0.229039446f, 0.5f, -0.973417103f,	0.3047, 0.0,
		-.5f, 0.0f, -0.87f,		-0.229039446f, 0.5f, -0.973417103f,	0.3324, 0.0,
		-.17f, 1.0f, -0.47f,	-0.229039446f, 0.5f, -0.973417103f,	0.3985, 1.0,
		-.25f, 1.0f, -0.435f,	-0.581238329f, 0.5f, -0.581238329f,	0.412, 1.0,
		-.5f, 0.0f, -0.87f,		-0.581238329f, 0.5f, -0.581238329f,	0.3324, 0.0,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.5f, -0.581238329f,	0.3601, 0.0,
		-.25f, 1.0f, -0.435f,	-0.581238329f, 0.5f, -0.581238329f,	0.412, 1.0,
		-.32f, 1.0f, -0.385f,	-0.581238329f, 0.5f, -0.581238329f,	0.4255, 1.0,
		-.64f, 0.0f, -0.77f,	-0.581238329f, 0.5f, -0.581238329f,	0.3601, 0.0,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.5f, -0.707106769f,	0.3878, 0.0,
		-.32f, 1.0f, -0.385f,	-0.707106769f, 0.5f, -0.707106769f,	0.4255, 1.0,
		-.385f, 1.0f, -0.32f,	-0.707106769f, 0.5f, -0.707106769f,	0.439, 1.0,
		-.77f, 0.0f, -0.64f,	-0.707106769f, 0.5f, -0.707106769f,	0.3878, 0.0,
		-.87f, 0.0f, -0.5f,		-0.707106769f, 0.5f, -0.707106769f,	0.4155, 0.0,
		-.385f, 1.0f, -0.32f,	-0.707106769f, 0.5f, -0.707106769f,	0.439, 1.0,
		-.435f, 1.0f, -0.25f,	-0.916157305f, 0.5f, -0.400818795f,	0.4525, 1.0,
		-.87f, 0.0f, -0.5f,		-0.916157305f, 0.5f, -0.400818795f,	0.4155, 0.0,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.5f, -0.400818795f,	0.4432, 0.0,
		-.435f, 1.0f, -0.25f,	-0.916157305f, 0.5f, -0.400818795f,	0.4525, 1.0,
		-.47f, 1.0f, -0.17f,	-0.916157305f, 0.5f, -0.400818795f,	0.466, 1.0,
		-.94f, 0.0f, -0.34f,	-0.916157305f, 0.5f, -0.400818795f,	0.4432, 0.0,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.4709, 0.0,
		-.47f, 1.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.466, 1.0,
		-.49f, 1.0f, -0.085f,	-0.973417103f, 0.5f, -0.229039446f,	0.4795, 1.0,
		-.98f, 0.0f, -0.17f,	-0.973417103f, 0.5f, -0.229039446f,	0.4709, 0.0,
		-1.0f, 0.0f, 0.0f,		-0.973417103f, 0.5f, -0.229039446f,	0.4986, 0.0,
		-.49f, 1.0f, -0.085f,	-0.973417103f, 0.5f, -0.229039446f,	0.4795, 1.0,
		-0.5f, 1.0f, 0.0f,		-0.993150651f, 0.5f, -0.116841137f,	0.493, 1.0,
		-1.0f, 0.0f, 0.0f,		-0.993150651f, 0.5f, -0.116841137f,	0.4986, 0.0,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.5f, 0.116841137f,	0.5263, 0.0,
		-0.5f, 1.0f, 0.0f,		-0.993150651f, 0.5f, 0.116841137f,	0.493, 1.0,
		-.49f, 1.0f, 0.085f,	-0.993150651f, 0.5f, 0.116841137f,	0.5065, 1.0,
		-.98f, 0.0f, 0.17f,		-0.993150651f, 0.5f, 0.116841137f,	0.5263, 0.0,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.5f, 0.229039446f,	0.554, 0.0,
		-.49f, 1.0f, 0.085f,	-0.973417103f, 0.5f, 0.229039446f,	0.5065, 1.0,
		-.47f, 1.0f, 0.17f,		-0.973417103f, 0.5f, 0.229039446f,	0.52, 1.0,
		-.94f, 0.0f, 0.34f,		-0.973417103f, 0.5f, 0.229039446f,	0.554, 0.0,
		-.87f, 0.0f, 0.5f,		-0.973417103f, 0.5f, 0.229039446f,	0.5817, 0.0,
		-.47f, 1.0f, 0.17f,		-0.973417103f, 0.5f, 0.229039446f,	0.52, 1.0,
		-.435f, 1.0f, 0.25f,	-0.813733339f, 0.5f, 0.581238329f,	0.5335, 1.0,
		-.87f, 0.0f, 0.5f,		-0.813733339f, 0.5f, 0.581238329f,	0.5817, 0.0,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.5f, 0.581238329f,	0.6094, 0.0,
		-.435f, 1.0f, 0.25f,	-0.813733339f, 0.5f, 0.581238329f,	0.5335, 1.0,
		-.385f, 1.0f, 0.32f,	-0.813733339f, 0.5f, 0.581238329f,	0.547, 1.0,
		-.77f, 0.0f, 0.64f,		-0.813733339f, 0.5f, 0.581238329f,	0.6094, 0.0,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.5f, 0.707106769f,	0.6371, 0.0,
		-.385f, 1.0f, 0.32f,	-0.707106769f, 0.5f, 0.707106769f,	0.547, 1.0,
		-.32f, 1.0f, 0.385f,	-0.707106769f, 0.5f, 0.707106769f,	0.5605, 1.0,
		-.64f, 0.0f, 0.77f,		-0.707106769f, 0.5f, 0.707106769f,	0.6371, 0.0,
		-.5f, 0.0f, 0.87f,		-0.707106769f, 0.5f, 0.707106769f,	0.6648, 0.0,
		-.32f, 1.0f, 0.385f,	-0.707106769f, 0.5f, 0.707106769f,	0.5605, 1.0,
		-.25f, 1.0f, 0.435f,	-0.400818795f, 0.5f, 0.916157305f,	0.574, 1.0,
		-.5f, 0.0f, 0.87f,		-0.400818795f, 0.5f, 0.916157305f,	0.6648, 0.0,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.5f, 0.916157305f,	0.6925, 0.0,
		-.25f, 1.0f, 0.435f,	-0.400818795f, 0.5f, 0.916157305f,	0.574, 1.0,
		-.17f, 1.0f, 0.47f,		-0.400818795f, 0.5f, 0.916157305f,	0.5875, 1.0,
		-.34f, 0.0f, 0.94f,		-0.400818795f, 0.5f, 0.916157305f,	0.6925, 0.0,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.5f, 0.973417103f,	0.7202, 0.0,
		-.17f, 1.0f, 0.47f,		-0.229039446f, 0.5f, 0.973417103f,	0.5875, 1.0,
		-.085f, 1.0f, 0.49f,	-0.229039446f, 0.5f, 0.973417103f,	0.601, 1.0,
		-.17f, 0.0f, 0.98f,		-0.229039446f, 0.5f, 0.973417103f,	0.7202, 0.0,
		0.0f, 0.0f, 1.0f,		-0.229039446f, 0.5f, 0.973417103f,	0.7479, 0.0,
		-.085f, 1.0f, 0.49f,	-0.229039446f, 0.5f, 0.973417103f,	0.601, 1.0,
		0.0f, 1.0f, 0.5f,		-0.116841137f, 0.5f, 0.993150651f,	0.6145, 1.0,
		0.0f, 0.0f, 1.0f,		-0.116841137f, 0.5f, 0.993150651f,	0.7479, 0.0,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.5f, 0.993150651f,	0.7756, 0.0,
		0.0f, 1.0f, 0.5f,		0.116841137f, 0.5f, 0.993150651f,	0.6145, 1.0,
		.085f, 1.0f, 0.49f,		0.116841137f, 0.5f, 0.993150651f,	0.628, 1.0,
		.17f, 0.0f, 0.98f,		0.116841137f, 0.5f, 0.993150651f,	0.7756, 0.0,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.5f, 0.973417103f,	0.8033, 0.0,
		.085f, 1.0f, 0.49f,		0.229039446f, 0.5f, 0.973417103f,	0.628, 1.0,
		.17f, 1.0f, 0.47f,		0.229039446f, 0.5f, 0.973417103f,	0.6415, 1.0,
		.34f, 0.0f, 0.94f,		0.229039446f, 0.5f, 0.973417103f,	0.8033, 0.0,
		.5f, 0.0f, 0.87f,		0.229039446f, 0.5f, 0.973417103f,	0.831, 0.0,
		.17f, 1.0f, 0.47f,		0.229039446f, 0.5f, 0.973417103f,	0.6415, 1.0,
		.25f, 1.0f, 0.435f,		0.581238329f, 0.5f, 0.813733339f,	0.655, 1.0,
		.5f, 0.0f, 0.87f,		0.581238329f, 0.5f, 0.813733339f,	0.831, 0.0,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.5f, 0.813733339f,	0.8587, 0.0,
		.25f, 1.0f, 0.435f,		0.581238329f, 0.5f, 0.813733339f,	0.655, 1.0,
		.32f, 1.0f, 0.385f,		0.581238329f, 0.5f, 0.813733339f,	0.6685, 1.0,
		.64f, 0.0f, 0.77f,		0.581238329f, 0.5f, 0.813733339f,	0.8587, 0.0,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.5f, 0.707106769f,	0.8864, 0.0,
		.32f, 1.0f, 0.385f,		0.707106769f, 0.5f, 0.707106769f,	0.6685, 1.0,
		.385f, 1.0f, 0.32f,		0.707106769f, 0.5f, 0.707106769f,	0.682, 1.0,
		.77f, 0.0f, 0.64f,		0.707106769f, 0.5f, 0.707106769f,	0.8864, 0.0,
		.87f, 0.0f, 0.5f,		0.707106769f, 0.5f, 0.707106769f,	0.9141, 0.0,
		.385f, 1.0f, 0.32f,		0.707106769f, 0.5f, 0.707106769f,	0.682, 1.0,
		.435f, 1.0f, 0.25f,		0.916157305f, 0.5f, 0.400818795f,	0.6955, 1.0,
		.87f, 0.0f, 0.5f,		0.916157305f, 0.5f, 0.400818795f,	0.9141, 0.0,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.5f, 0.400818795f,	0.9418, 0.0,
		.435f, 1.0f, 0.25f,		0.916157305f, 0.5f, 0.400818795f,	0.6955, 1.0,
		.47f, 1.0f, 0.17f,		0.916157305f, 0.5f, 0.400818795f,	0.709, 1.0,
		.94f, 0.0f, 0.34f,		0.916157305f, 0.5f, 0.400818795f,	0.9418, 1.0,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.9695, 0.0,
		.47f, 1.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.709, 0.0,
		.49f, 1.0f, 0.085f,		0.973417103f, 0.5f, 0.229039446f,	0.7225, 1.0,
		.98f, 0.0f, 0.17f,		0.973417103f, 0.5f, 0.229039446f,	0.9695, 0.0,
		1.0f, 0.0f, 0.0f,		0.973417103f, 0.5f, 0.229039446f,	1.0, 0.0,
		.49f, 1.0f, 0.085f,		0.973417103f, 0.5f, 0.229039446f,	0.7225, 1.0,
		0.5f, 1.0f, 0.0f,		0.993150651f, 0.5f, 0.116841137f,	0.75, 1.0,
		1.0f, 0.0f, 0.0f,		0.993150651f, 0.5f, 0.116841137f,	1.0, 0.0
	};

	// store vertex and index count
	m_TaperedCylinderMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (FloatsPerVertex + FloatsPerNormal + FloatsPerUV));
	m_TaperedCylinderMesh.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &m_TaperedCylinderMesh.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(m_TaperedCylinderMesh.vao);

	// Create VBO
	glGenBuffers(1, m_TaperedCylinderMesh.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_TaperedCylinderMesh.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	if (m_bMemoryLayoutDone == false)
	{
		SetShaderMemoryLayout();
	}
}

///////////////////////////////////////////////////
//	LoadTorusMesh()
//
//	Create a parameterized torus mesh by specifying the
//	main radius, tube radius, and segment counts. Store
//	the generated data in a VAO/VBO.
//
//	Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLES, 0, m_TorusMesh.nVertices);
///////////////////////////////////////////////////
void ShapeMeshes::LoadTorusMesh(float mainRadius, float tubeRadius, int mainSegments, int tubeSegments) {
	// Validate input parameters
	mainSegments = std::max(3, mainSegments);
	tubeSegments = std::max(3, tubeSegments);
	tubeRadius = std::max(0.01f, tubeRadius);

	float mainSegmentStep = 2.0f * Pi / mainSegments;
	float tubeSegmentStep = 2.0f * Pi / tubeSegments;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	// Generate vertices and normals
	for (int i = 0; i <= mainSegments; ++i) {
		float mainAngle = i * mainSegmentStep;
		float cosMain = cos(mainAngle);
		float sinMain = sin(mainAngle);

		for (int j = 0; j <= tubeSegments; ++j) {
			float tubeAngle = j * tubeSegmentStep;
			float cosTube = cos(tubeAngle);
			float sinTube = sin(tubeAngle);

			// Vertex position
			float x = (mainRadius + tubeRadius * cosTube) * cosMain;
			float y = (mainRadius + tubeRadius * cosTube) * sinMain;
			float z = tubeRadius * sinTube;

			// Normal vector
			glm::vec3 center(mainRadius * cosMain, mainRadius * sinMain, 0.0f);
			glm::vec3 vertex(x, y, z);
			glm::vec3 normal = glm::normalize(vertex - center);

			// Texture coordinates
			float u = (float)i / mainSegments;
			float v = (float)j / tubeSegments;

			// Store interleaved vertex data
			vertices.push_back(x);    // Position x
			vertices.push_back(y);    // Position y
			vertices.push_back(z);    // Position z
			vertices.push_back(normal.x); // Normal x
			vertices.push_back(normal.y); // Normal y
			vertices.push_back(normal.z); // Normal z
			vertices.push_back(u);    // Texture u
			vertices.push_back(v);    // Texture v
		}
	}

	// Generate indices for triangle strips
	for (int i = 0; i < mainSegments; ++i) {
		for (int j = 0; j < tubeSegments; ++j) {
			int current = i * (tubeSegments + 1) + j;
			int next = (i + 1) * (tubeSegments + 1) + j;

			// First triangle
			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1);

			// Second triangle
			indices.push_back(current + 1);
			indices.push_back(next);
			indices.push_back(next + 1);
		}
	}

	// Store vertex and index counts
	m_TorusMesh.nVertices = static_cast<GLuint>(vertices.size() / 8); // 8 floats per vertex
	m_TorusMesh.nIndices = static_cast<GLuint>(indices.size());

	// Create VAO
	glGenVertexArrays(1, &m_TorusMesh.vao);
	glBindVertexArray(m_TorusMesh.vao);

	// Create VBO for vertices
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	// Create EBO for indices
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Define vertex attributes
	GLint stride = sizeof(GLfloat) * 8;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                      // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat))); // Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(GLfloat))); // Texture coords
	glEnableVertexAttribArray(2);

	// Unbind VAO for safety
	glBindVertexArray(0);

	// Mark memory layout as complete
	if (!m_bMemoryLayoutDone) {
		SetShaderMemoryLayout();
	}
}


///////////////////////////////////////////////////
//	LoadExtraTorusMesh1()
//
//	Create a torus mesh by specifying the vertices and 
//  store it in a VAO/VBO.  The normals and texture
//  coordinates are also set.
//
//	Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLES, 0, meshes.gExtraTorusMesh1.nVertices);
///////////////////////////////////////////////////
void ShapeMeshes::LoadExtraTorusMesh1(float thickness)
{
	int _mainSegments = 30;
	int _tubeSegments = 30;
	float _mainRadius = 1.0f;
	float _tubeRadius = .1f;

	if (thickness <= 1.0)
	{
		_tubeRadius = thickness;
	}

	auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
	auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

	std::vector<glm::vec3> vertex_list;
	std::vector<std::vector<glm::vec3>> segments_list;
	std::vector<glm::vec2> texture_coords;
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 normal;
	glm::vec3 vertex;
	glm::vec2 text_coord;

	// generate the torus vertices
	auto currentMainSegmentAngle = 0.0f;
	for (auto i = 0; i < _mainSegments; i++)
	{
		// Calculate sine and cosine of main segment angle
		auto sinMainSegment = sin(currentMainSegmentAngle);
		auto cosMainSegment = cos(currentMainSegmentAngle);
		auto currentTubeSegmentAngle = 0.0f;
		std::vector<glm::vec3> segment_points;
		for (auto j = 0; j < _tubeSegments; j++)
		{
			// Calculate sine and cosine of tube segment angle
			auto sinTubeSegment = sin(currentTubeSegmentAngle);
			auto cosTubeSegment = cos(currentTubeSegmentAngle);

			// Calculate vertex position on the surface of torus
			auto surfacePosition = glm::vec3(
				(_mainRadius + _tubeRadius * cosTubeSegment) * cosMainSegment,
				(_mainRadius + _tubeRadius * cosTubeSegment) * sinMainSegment,
				_tubeRadius * sinTubeSegment);

			//vertex_list.push_back(surfacePosition);
			segment_points.push_back(surfacePosition);

			// Update current tube angle
			currentTubeSegmentAngle += tubeSegmentAngleStep;
		}
		segments_list.push_back(segment_points);
		segment_points.clear();

		// Update main segment angle
		currentMainSegmentAngle += mainSegmentAngleStep;
	}

	float horizontalStep = 1.0 / _mainSegments;
	float verticalStep = 1.0 / _tubeSegments;
	float u = 0.0;
	float v = 0.0;

	// connect the various segments together, forming triangles
	for (int i = 0; i < _mainSegments; i++)
	{
		for (int j = 0; j < _tubeSegments; j++)
		{
			if (((i + 1) < _mainSegments) && ((j + 1) < _tubeSegments))
			{
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i][j + 1]);
				texture_coords.push_back(glm::vec2(u, v + verticalStep));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v + verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i + 1][j]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v - verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
			}
			else
			{
				if (((i + 1) == _mainSegments) && ((j + 1) == _tubeSegments))
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((i + 1) == _mainSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][j + 1]);
					texture_coords.push_back(glm::vec2(u, v + verticalStep));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((j + 1) == _tubeSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i + 1][j]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, v));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}

			}
			v += verticalStep;
		}
		v = 0.0;
		u += horizontalStep;
	}

	std::vector<GLfloat> combined_values;

	// combine interleaved vertices, normals, and texture coords
	for (int i = 0; i < vertex_list.size(); i++)
	{
		vertex = vertex_list[i];
		normal = normalize(vertex);

		text_coord = texture_coords[i];
		combined_values.push_back(vertex.x);
		combined_values.push_back(vertex.y);
		combined_values.push_back(vertex.z);
		combined_values.push_back(normal.x);
		combined_values.push_back(normal.y);
		combined_values.push_back(normal.z);
		combined_values.push_back(text_coord.x);
		combined_values.push_back(text_coord.y);
	}

	// store vertex and index count
	m_ExtraTorusMesh1.nVertices = vertex_list.size();
	m_ExtraTorusMesh1.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &m_ExtraTorusMesh1.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(m_ExtraTorusMesh1.vao);

	// Create VBOs
	glGenBuffers(1, m_ExtraTorusMesh1.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ExtraTorusMesh1.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combined_values.size(), combined_values.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	if (m_bMemoryLayoutDone == false)
	{
		SetShaderMemoryLayout();
	}
}

///////////////////////////////////////////////////
//	LoadExtraTorusMesh2()
//
//	Create a torus mesh by specifying the vertices and 
//  store it in a VAO/VBO.  The normals and texture
//  coordinates are also set.
//
//	Correct triangle drawing command:
//
//	glDrawArrays(GL_TRIANGLES, 0, meshes.gExtraTorusMesh1.nVertices);
///////////////////////////////////////////////////
void ShapeMeshes::LoadExtraTorusMesh2(float thickness)
{
	int _mainSegments = 30;
	int _tubeSegments = 30;
	float _mainRadius = 1.0f;
	float _tubeRadius = .1f;

	if (thickness <= 1.0)
	{
		_tubeRadius = thickness;
	}

	auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
	auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

	std::vector<glm::vec3> vertex_list;
	std::vector<std::vector<glm::vec3>> segments_list;
	std::vector<glm::vec2> texture_coords;
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 normal;
	glm::vec3 vertex;
	glm::vec2 text_coord;

	// generate the torus vertices
	auto currentMainSegmentAngle = 0.0f;
	for (auto i = 0; i < _mainSegments; i++)
	{
		// Calculate sine and cosine of main segment angle
		auto sinMainSegment = sin(currentMainSegmentAngle);
		auto cosMainSegment = cos(currentMainSegmentAngle);
		auto currentTubeSegmentAngle = 0.0f;
		std::vector<glm::vec3> segment_points;
		for (auto j = 0; j < _tubeSegments; j++)
		{
			// Calculate sine and cosine of tube segment angle
			auto sinTubeSegment = sin(currentTubeSegmentAngle);
			auto cosTubeSegment = cos(currentTubeSegmentAngle);

			// Calculate vertex position on the surface of torus
			auto surfacePosition = glm::vec3(
				(_mainRadius + _tubeRadius * cosTubeSegment) * cosMainSegment,
				(_mainRadius + _tubeRadius * cosTubeSegment) * sinMainSegment,
				_tubeRadius * sinTubeSegment);

			//vertex_list.push_back(surfacePosition);
			segment_points.push_back(surfacePosition);

			// Update current tube angle
			currentTubeSegmentAngle += tubeSegmentAngleStep;
		}
		segments_list.push_back(segment_points);
		segment_points.clear();

		// Update main segment angle
		currentMainSegmentAngle += mainSegmentAngleStep;
	}

	float horizontalStep = 1.0 / _mainSegments;
	float verticalStep = 1.0 / _tubeSegments;
	float u = 0.0;
	float v = 0.0;

	// connect the various segments together, forming triangles
	for (int i = 0; i < _mainSegments; i++)
	{
		for (int j = 0; j < _tubeSegments; j++)
		{
			if (((i + 1) < _mainSegments) && ((j + 1) < _tubeSegments))
			{
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i][j + 1]);
				texture_coords.push_back(glm::vec2(u, v + verticalStep));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v + verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
				vertex_list.push_back(segments_list[i + 1][j]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v));
				vertex_list.push_back(segments_list[i + 1][j + 1]);
				texture_coords.push_back(glm::vec2(u + horizontalStep, v - verticalStep));
				vertex_list.push_back(segments_list[i][j]);
				texture_coords.push_back(glm::vec2(u, v));
			}
			else
			{
				if (((i + 1) == _mainSegments) && ((j + 1) == _tubeSegments))
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][0]);
					texture_coords.push_back(glm::vec2(0, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((i + 1) == _mainSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][j + 1]);
					texture_coords.push_back(glm::vec2(u, v + verticalStep));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[0][j]);
					texture_coords.push_back(glm::vec2(0, v));
					vertex_list.push_back(segments_list[0][j + 1]);
					texture_coords.push_back(glm::vec2(0, v + verticalStep));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}
				else if ((j + 1) == _tubeSegments)
				{
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i][0]);
					texture_coords.push_back(glm::vec2(u, 0));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
					vertex_list.push_back(segments_list[i + 1][j]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, v));
					vertex_list.push_back(segments_list[i + 1][0]);
					texture_coords.push_back(glm::vec2(u + horizontalStep, 0));
					vertex_list.push_back(segments_list[i][j]);
					texture_coords.push_back(glm::vec2(u, v));
				}

			}
			v += verticalStep;
		}
		v = 0.0;
		u += horizontalStep;
	}

	std::vector<GLfloat> combined_values;

	// combine interleaved vertices, normals, and texture coords
	for (int i = 0; i < vertex_list.size(); i++)
	{
		vertex = vertex_list[i];
		normal = normalize(vertex);

		text_coord = texture_coords[i];
		combined_values.push_back(vertex.x);
		combined_values.push_back(vertex.y);
		combined_values.push_back(vertex.z);
		combined_values.push_back(normal.x);
		combined_values.push_back(normal.y);
		combined_values.push_back(normal.z);
		combined_values.push_back(text_coord.x);
		combined_values.push_back(text_coord.y);
	}

	// store vertex and index count
	m_ExtraTorusMesh2.nVertices = vertex_list.size();
	m_ExtraTorusMesh2.nIndices = 0;

	// Create VAO
	glGenVertexArrays(1, &m_ExtraTorusMesh2.vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(m_ExtraTorusMesh2.vao);

	// Create VBOs
	glGenBuffers(1, m_ExtraTorusMesh2.vbos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ExtraTorusMesh2.vbos[0]); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * combined_values.size(), combined_values.data(), GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	if (m_bMemoryLayoutDone == false)
	{
		SetShaderMemoryLayout();
	}
}

//**************************************************************************
// The following set of methods are called to draw the various basic 3D
// shapes after they have been loaded in memory.
//**************************************************************************

///////////////////////////////////////////////////
// DrawBoxMesh()
//
// Binds the box mesh's VAO and draws the entire box.
///////////////////////////////////////////////////
void ShapeMeshes::DrawBoxMesh() const
{
	if (m_BoxMesh.vao == 0 || m_BoxMesh.nIndices == 0) {
		std::cerr << "Error: Box mesh not initialized properly." << std::endl;
		return;
	}

	glBindVertexArray(m_BoxMesh.vao);
	glDrawElements(GL_TRIANGLES, m_BoxMesh.nIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// DrawBoxMeshSide()
//
// Draws a specific side of the box mesh using 
// triangle fan mode. Each side can be textured 
// differently before drawing.
///////////////////////////////////////////////////
void ShapeMeshes::DrawBoxMeshSide(BoxSide side) const
{
	if (m_BoxMesh.vao == 0) {
		std::cerr << "Error: Box mesh not initialized properly." << std::endl;
		return;
	}

	glBindVertexArray(m_BoxMesh.vao);

	// Mapping side to starting vertex index
	constexpr GLint sideStartIndices[] = {
		0,   // back
		4,   // bottom
		8,   // left
		12,  // right
		16,  // top
		20   // front
	};

	if (side < back || side > front) {
		std::cerr << "Error: Invalid box side specified." << std::endl;
		glBindVertexArray(0);
		return;
	}

	glDrawArrays(GL_TRIANGLE_FAN, sideStartIndices[side], 4);
	glBindVertexArray(0);
}


///////////////////////////////////////////////////
// DrawBoxMeshLines()
//
// Binds the box mesh's VAO and draws the edges of 
// the box using line primitives.
///////////////////////////////////////////////////
void ShapeMeshes::DrawBoxMeshLines() const
{
	if (m_BoxMesh.vao == 0 || m_BoxMesh.nIndices == 0) {
		std::cerr << "Error: Box mesh not initialized properly." << std::endl;
		return;
	}

	glBindVertexArray(m_BoxMesh.vao);

	// Draw the box using line primitives for outlining edges
	glDrawElements(GL_LINE_STRIP, m_BoxMesh.nIndices, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindVertexArray(0);
}


///////////////////////////////////////////////////
//	DrawConeMesh()
//
//	Transform and draw the cone mesh to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawConeMesh(bool bDrawBottom) {
	glBindVertexArray(m_ConeMesh.vao);

	// Bottom circle vertex count: numSlices + 2 (center + all slices + closing slice)
	int bottomVertexCount = m_ConeMesh.numSlices + 2;
	// Side vertex count: 2 vertices per slice
	int sideVertexCount = m_ConeMesh.numSlices * 2;

	if (bDrawBottom) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, bottomVertexCount); // Bottom circle
	}
	glDrawArrays(GL_TRIANGLE_STRIP, bottomVertexCount, sideVertexCount); // Cone sides

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawConeMeshLines()
//
//	Transform and draw the cone mesh lines to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawConeMeshLines(bool bDrawBottom) {
	glBindVertexArray(m_ConeMesh.vao);

	// Bottom circle vertex count: numSlices + 2 (center + all slices + closing slice)
	int bottomVertexCount = m_ConeMesh.numSlices + 2;
	// Side vertex count: 2 vertices per slice
	int sideVertexCount = m_ConeMesh.numSlices * 2;

	if (bDrawBottom) {
		glDrawArrays(GL_LINES, 0, bottomVertexCount); // Bottom circle
	}
	glDrawArrays(GL_LINE_STRIP, bottomVertexCount, sideVertexCount); // Cone sides

	glBindVertexArray(0);
}


///////////////////////////////////////////////////
//	DrawCylinderMesh()
//
//	Transform and draw the cylinder mesh to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawCylinderMesh(
	bool bDrawTop,
	bool bDrawBottom,
	bool bDrawSides)
{
	glBindVertexArray(m_CylinderMesh.vao);

	// Calculate vertex counts
	int bottomVertexCount = m_CylinderMesh.numSlices + 2; // Center + all slices + closing slice
	int topVertexCount = m_CylinderMesh.numSlices + 2;    // Same as bottom
	int sideVertexCount = (m_CylinderMesh.numSlices + 1) * 2; // Two vertices per slice, +1 for closing strip

	// Draw the bottom circle
	if (bDrawBottom) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, bottomVertexCount);
	}

	// Draw the top circle
	if (bDrawTop) {
		glDrawArrays(GL_TRIANGLE_FAN, bottomVertexCount, topVertexCount);
	}

	// Draw the sides
	if (bDrawSides) {
		glDrawArrays(GL_TRIANGLE_STRIP, bottomVertexCount + topVertexCount, sideVertexCount);
	}

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawCylinderMeshLines()
//
//	Transform and draw the cylinder mesh lines to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawCylinderMeshLines(
	bool bDrawTop,
	bool bDrawBottom,
	bool bDrawSides
)
{
	glBindVertexArray(m_CylinderMesh.vao);

	// Calculate vertex counts
	int bottomVertexCount = m_CylinderMesh.numSlices + 2; // Center + all slices + closing slice
	int topVertexCount = m_CylinderMesh.numSlices + 2;    // Same as bottom
	int sideVertexCount = (m_CylinderMesh.numSlices + 1) * 2; // Two vertices per slice, +1 for closing strip

	// Draw the bottom circle lines
	if (bDrawBottom) {
		glDrawArrays(GL_LINE_LOOP, 1, m_CylinderMesh.numSlices); // Skip the center vertex for a proper loop
	}

	// Draw the top circle lines
	if (bDrawTop) {
		glDrawArrays(GL_LINE_LOOP, bottomVertexCount + 1, m_CylinderMesh.numSlices); // Skip the center vertex for a proper loop
	}

	// Draw the side lines
	if (bDrawSides) {
		glDrawArrays(GL_LINE_STRIP, bottomVertexCount + topVertexCount, sideVertexCount);
	}

	glBindVertexArray(0);
}


///////////////////////////////////////////////////
//	DrawPlaneMesh()
//
//	Transform and draw the plane mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawPlaneMesh()
{
	glBindVertexArray(m_PlaneMesh.vao);

	glDrawElements(GL_TRIANGLE_STRIP, m_PlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);
	
	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawPlaneMeshLines()
//
//	Transform and draw the plane mesh lines to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawPlaneMeshLines()
{
	glBindVertexArray(m_PlaneMesh.vao);

	glDrawElements(GL_LINE_STRIP, m_PlaneMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// DrawPrismMesh()
//
// Transform and draw the prism mesh to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawPrismMesh() {
	glBindVertexArray(m_PrismMesh.vao);

	// Draw the base and slanted faces
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_PrismMesh.nVertices);

	glBindVertexArray(0); // Unbind the VAO after drawing
}

///////////////////////////////////////////////////
// DrawPrismMeshLines()
//
// Transform and draw the prism mesh lines to the window.
//
///////////////////////////////////////////////////
void ShapeMeshes::DrawPrismMeshLines() {
	glBindVertexArray(m_PrismMesh.vao);

	// Use GL_LINE_LOOP or GL_LINE_STRIP for wireframe rendering
	glDrawArrays(GL_LINE_STRIP, 0, m_PrismMesh.nVertices);

	glBindVertexArray(0); // Unbind the VAO after drawing
}

///////////////////////////////////////////////////
// DrawPyramid3Mesh()
//
// Transform and draw the 3-sided pyramid mesh to the window.
///////////////////////////////////////////////////
void ShapeMeshes::DrawPyramid3Mesh()
{
	if (m_Pyramid3Mesh.nVertices == 0)
	{
		std::cerr << "Error: Pyramid mesh not loaded or empty!" << std::endl;
		return;
	}

	glBindVertexArray(m_Pyramid3Mesh.vao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Pyramid3Mesh.nVertices);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// DrawPyramid3MeshLines()
//
// Transform and draw the 3-sided pyramid mesh lines to the window.
///////////////////////////////////////////////////
void ShapeMeshes::DrawPyramid3MeshLines()
{
	if (m_Pyramid3Mesh.nVertices == 0)
	{
		std::cerr << "Error: Pyramid mesh not loaded or empty!" << std::endl;
		return;
	}

	glBindVertexArray(m_Pyramid3Mesh.vao);

	glDrawArrays(GL_LINE_STRIP, 0, m_Pyramid3Mesh.nVertices);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// DrawPyramid4Mesh()
//
// Transform and draw the 4-sided pyramid mesh to the window.
///////////////////////////////////////////////////
void ShapeMeshes::DrawPyramid4Mesh()
{
	if (m_Pyramid4Mesh.nVertices == 0)
	{
		std::cerr << "Error: Pyramid mesh not loaded or has no vertices!" << std::endl;
		return;
	}

	glBindVertexArray(m_Pyramid4Mesh.vao);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Pyramid4Mesh.nVertices);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
// DrawPyramid4MeshLines()
//
// Transform and draw the 4-sided pyramid mesh lines to the window.
///////////////////////////////////////////////////
void ShapeMeshes::DrawPyramid4MeshLines()
{
	if (m_Pyramid4Mesh.nVertices == 0)
	{
		std::cerr << "Error: Pyramid mesh not loaded or has no vertices!" << std::endl;
		return;
	}

	glBindVertexArray(m_Pyramid4Mesh.vao);

	glDrawArrays(GL_LINE_STRIP, 0, m_Pyramid4Mesh.nVertices);

	glBindVertexArray(0);
}


void ShapeMeshes::DrawSphereMesh()
{
	if (m_SphereMesh.vao == 0 || m_SphereMesh.nIndices == 0)
	{
		std::cerr << "Error: Sphere mesh VAO or indices not properly initialized." << std::endl;
		return;
	}

	glBindVertexArray(m_SphereMesh.vao);

	glDrawElements(GL_TRIANGLES, m_SphereMesh.nIndices, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}


void ShapeMeshes::DrawSphereMeshLines()
{
	glBindVertexArray(m_SphereMesh.vao);

	glDrawElements(GL_LINE_STRIP, m_SphereMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

void ShapeMeshes::DrawHalfSphereMesh()
{
	if (m_SphereMesh.vao == 0 || m_SphereMesh.nIndices == 0)
	{
		std::cerr << "Error: Sphere mesh VAO or indices not properly initialized." << std::endl;
		return;
	}

	glBindVertexArray(m_SphereMesh.vao);

	glDrawElements(GL_TRIANGLES, m_SphereMesh.nIndices / 2, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void ShapeMeshes::DrawHalfSphereMeshLines()
{
	if (m_SphereMesh.vao == 0 || m_SphereMesh.nIndices == 0)
	{
		std::cerr << "Error: Sphere mesh VAO or indices not properly initialized." << std::endl;
		return;
	}

	glBindVertexArray(m_SphereMesh.vao);

	glDrawElements(GL_LINES, m_SphereMesh.nIndices / 2, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawTaperedCylinderMesh()
//
//	Transform and draw the tapered cylinder mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawTaperedCylinderMesh(
	bool bDrawTop,
	bool bDrawBottom,
	bool bDrawSides)
{
	glBindVertexArray(m_TaperedCylinderMesh.vao);

	if (bDrawBottom == true)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 0, 36);	//bottom
	}
	if (bDrawTop == true)
	{
		glDrawArrays(GL_TRIANGLE_FAN, 36, 72);	//top
	}
	if (bDrawSides == true)
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 72, 146);	//sides
	}

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawTaperedCylinderMeshLines()
//
//	Transform and draw the tapered cylinder mesh lines to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawTaperedCylinderMeshLines(
	bool bDrawTop,
	bool bDrawBottom,
	bool bDrawSides)
{
	glBindVertexArray(m_TaperedCylinderMesh.vao);

	if (bDrawBottom == true)
	{
		glDrawArrays(GL_LINES, 0, 36);	//bottom
	}
	if (bDrawTop == true)
	{
		glDrawArrays(GL_LINES, 36, 72);	//top
	}
	if (bDrawSides == true)
	{
		glDrawArrays(GL_LINE_STRIP, 72, 146);	//sides
	}

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawTorusMesh()
//
//	Transform and draw the torus mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawTorusMesh()
{
	glBindVertexArray(m_TorusMesh.vao);

	// Use indexed drawing
	glDrawElements(GL_TRIANGLES, m_TorusMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawTorusMeshLines()
//
//	Transform and draw the torus mesh lines to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawTorusMeshLines()
{
	glBindVertexArray(m_TorusMesh.vao);

	// Use indexed drawing for lines
	glDrawElements(GL_LINES, m_TorusMesh.nIndices, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}


///////////////////////////////////////////////////
//	DrawExtraTorusMesh1()
//
//	Transform and draw the torus mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawExtraTorusMesh1()
{
	glBindVertexArray(m_ExtraTorusMesh1.vao);

	glDrawArrays(GL_TRIANGLES, 0, m_ExtraTorusMesh1.nVertices);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawExtraTorusMesh2()
//
//	Transform and draw the torus mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawExtraTorusMesh2()
{
	glBindVertexArray(m_ExtraTorusMesh2.vao);

	glDrawArrays(GL_TRIANGLES, 0, m_ExtraTorusMesh2.nVertices);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawHalfTorusMesh()
//
//	Transform and draw the half torus mesh to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawHalfTorusMesh()
{
	glBindVertexArray(m_TorusMesh.vao);

	// Use indexed drawing for half the indices
	glDrawElements(GL_TRIANGLES, m_TorusMesh.nIndices / 2, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

///////////////////////////////////////////////////
//	DrawHalfTorusMeshLines()
//
//	Transform and draw the half torus mesh lines to the window.
// 
///////////////////////////////////////////////////
void ShapeMeshes::DrawHalfTorusMeshLines()
{
	glBindVertexArray(m_TorusMesh.vao);

	// Use indexed drawing for half the indices in line mode
	glDrawElements(GL_LINES, m_TorusMesh.nIndices / 2, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}


glm::vec3 ShapeMeshes::QuadCrossProduct(
	glm::vec3 pnt0, glm::vec3 pnt1, glm::vec3 pnt2, glm::vec3 pnt3)
{
	glm::vec3 Normal(0, 0, 0);
	
	Normal = glm::cross((pnt3 - pnt0), (pnt2 - pnt1));

	return Normal;
}

glm::vec3 ShapeMeshes::CalculateTriangleNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 Normal(0, 0, 0);
	glm::vec3 v1;
	glm::vec3 v2;
	float magnitude = 0;

	v1 = (p2 - p1);
	v2 = (p3 - p1);

	Normal.x = (v1.y * v2.z) - (v1.z * v2.y);
	Normal.y = (v1.z * v2.x) - (v1.x * v2.z);
	Normal.z = (v1.x * v2.y) - (v1.y * v2.x);

	magnitude = (float)sqrt(Normal.x * Normal.x + Normal.y * Normal.y + Normal.z * Normal.z);
	if (magnitude == 0)
	{
		//throw Exception();
	}
	else
	{
		Normal.x /= magnitude;
		Normal.y /= magnitude;
		Normal.z /= magnitude;
	}

	return(Normal);
	
}
void ShapeMeshes::SetShaderMemoryLayout()
{
    // Attribute location definitions
    constexpr GLuint POSITION_ATTR_LOCATION = 0;
    constexpr GLuint NORMAL_ATTR_LOCATION = 1;
    constexpr GLuint UV_ATTR_LOCATION = 2;

    // Calculate stride as the size of all vertex attributes combined
    GLint stride = sizeof(float) * (FloatsPerVertex + FloatsPerNormal + FloatsPerUV);

    // Set up the position attribute
    glVertexAttribPointer(
        POSITION_ATTR_LOCATION,     // Attribute location in the shader
        FloatsPerVertex,            // Number of floats per vertex attribute
        GL_FLOAT,                   // Data type of each component
        GL_FALSE,                   // Normalize flag
        stride,                     // Stride (distance between consecutive attributes)
        reinterpret_cast<void*>(0)  // Offset from the beginning of the buffer
    );
    glEnableVertexAttribArray(POSITION_ATTR_LOCATION);

    // Set up the normal attribute
    glVertexAttribPointer(
        NORMAL_ATTR_LOCATION,       // Attribute location in the shader
        FloatsPerNormal,            // Number of floats per normal attribute
        GL_FLOAT,                   // Data type of each component
        GL_FALSE,                   // Normalize flag
        stride,                     // Stride
        reinterpret_cast<void*>(sizeof(float) * FloatsPerVertex)  // Offset
    );
    glEnableVertexAttribArray(NORMAL_ATTR_LOCATION);

    // Set up the UV attribute
    glVertexAttribPointer(
        UV_ATTR_LOCATION,           // Attribute location in the shader
        FloatsPerUV,                // Number of floats per UV attribute
        GL_FLOAT,                   // Data type of each component
        GL_FALSE,                   // Normalize flag
        stride,                     // Stride
        reinterpret_cast<void*>(sizeof(float) * (FloatsPerVertex + FloatsPerNormal))  // Offset
    );
    glEnableVertexAttribArray(UV_ATTR_LOCATION);
}
