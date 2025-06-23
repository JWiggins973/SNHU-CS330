///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
// ================
// This file contains the implementation of the `SceneManager` class, which is 
// responsible for managing the preparation and rendering of 3D scenes. It 
// handles textures, materials, lighting configurations, and object rendering.
//
// AUTHOR: Brian Battersby
// INSTITUTION: Southern New Hampshire University (SNHU)
// COURSE: CS-330 Computational Graphics and Visualization
//
// INITIAL VERSION: November 1, 2023
// LAST REVISED: December 1, 2024
//
// RESPONSIBILITIES:
// - Load, bind, and manage textures in OpenGL.
// - Define materials and lighting properties for 3D objects.
// - Manage transformations and shader configurations.
// - Render complex 3D scenes using basic meshes.
//
// NOTE: This implementation leverages external libraries like `stb_image` for 
// texture loading and GLM for matrix and vector operations.
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}


/***********************************************************
 *  LoadSceneTextures()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::LoadSceneTextures()
{
	bool bReturn = false;

	// tag name corresponds to what item its being applied to
	CreateGLTexture("textures/BeigeWall.jpg", "beigeWall");

	CreateGLTexture("textures/carpet.jpg", "carpet");

	CreateGLTexture("textures/cushionFabric.jpg", "cushionFabric");

	CreateGLTexture("textures/WoodTable.png", "woodTable");

	CreateGLTexture("textures/WoodFloor.jpg", "woodFloor");

	CreateGLTexture("textures/BlackMetal.jpg", "blackMetal");

	CreateGLTexture("textures/lampShadeCanvas.png", "lampShadeCanvas");
	
	CreateGLTexture("textures/MetalBulb.jpg", "MetalBulb");

	CreateGLTexture("textures/WoodTableTop.jpg", "WoodTableTop");

	CreateGLTexture("textures/glassBulb.jpg", "glassBulb");

	CreateGLTexture("textures/Marble.jpg", "marble");

	CreateGLTexture("textures/pillowFront.jpg", "pillowFront");

	CreateGLTexture("textures/pillowBody.jpg", "pillowBody");

	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}


/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	/*** STUDENTS - add the code BELOW for defining object materials. ***/
	/*** There is no limit to the number of object materials that can ***/
	/*** be defined. Refer to the code in the OpenGL Sample for help  ***/

	// fabric material
	OBJECT_MATERIAL fabricMaterial;
	fabricMaterial.diffuseColor = glm::vec3(0.8f, 0.7f, 0.6f);      // light tanish color
	fabricMaterial.specularColor = glm::vec3(0.06f, 0.06f, 0.05f); // flatter finish
	fabricMaterial.shininess = 3.0f;                              // low shine
	fabricMaterial.tag = "fabric";
	m_objectMaterials.push_back(fabricMaterial);

	// laminate wood material
	OBJECT_MATERIAL woodMaterial;
	woodMaterial.diffuseColor = glm::vec3(0.5f, 0.3f, 0.1f);    // brownish color
	woodMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.2f);  // glossier look
	woodMaterial.shininess = 30.0f;                           // shiny glow
	woodMaterial.tag = "wood";
	m_objectMaterials.push_back(woodMaterial);

	// metal material low shine
	OBJECT_MATERIAL metalMaterial;
	metalMaterial.diffuseColor = glm::vec3(0.6f, 0.6f, 0.6f);    // steel like gray metal
	metalMaterial.specularColor = glm::vec3(0.3f, 0.3f, 0.3f);  // mattish metal finish
	metalMaterial.shininess = 8.0f;                            // low shine
	metalMaterial.tag = "metal";
	m_objectMaterials.push_back(metalMaterial);

	// wall material
	OBJECT_MATERIAL wallMaterial;
	wallMaterial.diffuseColor = glm::vec3(0.9f, 0.85f, 0.8f);     // Warm off-white beige
	wallMaterial.specularColor = glm::vec3(0.02f, 0.02f, 0.02f); // flat finish
	wallMaterial.shininess = 2.0f;                              // low shine
	wallMaterial.tag = "wall";
	m_objectMaterials.push_back(wallMaterial);

	// carpet rug material
	OBJECT_MATERIAL carpetMaterial;
	carpetMaterial.diffuseColor = glm::vec3(0.5f, 0.3f, 0.1f);      // similiar color to wood
	carpetMaterial.specularColor = glm::vec3(0.03f, 0.03f, 0.03f); // Very subtle sheen
	carpetMaterial.shininess = 2.0f;                              // low shine
	carpetMaterial.tag = "carpet";
	m_objectMaterials.push_back(carpetMaterial);

}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting, if no light sources have
	// been added then the display window will be black - to use the 
	// default OpenGL lighting then comment out the following line
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	// directional light 
	m_pShaderManager->setVec3Value("directionalLight.direction", -0.2f, 1.0f, -0.3f); // light direction above scene objects
	m_pShaderManager->setVec3Value("directionalLight.ambient", 0.6f, 0.5f, 0.4f); // soft warm color light
	m_pShaderManager->setVec3Value("directionalLight.diffuse", 0.5f, 0.4f, 0.35f); // soft warm duffuse lighting
	m_pShaderManager->setVec3Value("directionalLight.specular", 0.4f, 0.35f, 0.3f); // bright specular highligts
	m_pShaderManager->setBoolValue("directionalLight.bActive", true);

	// point light 
	m_pShaderManager->setVec3Value("pointLights[0].position", -7.0f, 7.0f, -4.0f); // position to the left of scene
	m_pShaderManager->setVec3Value("pointLights[0].ambient", 0.2f, 0.15f, 0.12f); // low warm ambient glow so scene isnt too bright
	m_pShaderManager->setVec3Value("pointLights[0].diffuse", 0.4f, 0.4f, 0.3f); // low diffuse light so scene isnt too bright
	m_pShaderManager->setVec3Value("pointLights[0].specular", 0.4f, 0.3f, 0.2f); // low specular light so scene isnt too bright
	m_pShaderManager->setBoolValue("pointLights[0].bActive", true);

	// point light lamp 
	/*** Based on reference photo light shouldnt hit the wall,
	but it really enhances the scene in my opinion so I will keep it ***/
	m_pShaderManager->setVec3Value("pointLights[1].position", 13.0f, 5.5f, -6.0f); // position above light bulb
	m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.1f, 0.08f, 0.06f); // low warm brownish ambient glow
	m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.25f, 0.2f, 0.15f); // low warm diffuse light
	m_pShaderManager->setVec3Value("pointLights[1].specular", 0.2f, 0.15f, 0.1f); // low warm white specular light
	m_pShaderManager->setBoolValue("pointLights[1].bActive", true);
}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// load the texture image files for the textures applied
	// to objects in the 3D scene
	LoadSceneTextures();
	// define the materials for objects in the scene
	DefineObjectMaterials();
	// add and define the light sources for the scene
	SetupSceneLights();

	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene
	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadConeMesh();
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene() {

	RenderFloor();
	RenderWall();
	RenderRug();
	RenderTable();
	RenderLamp();
	RenderCouch();
	RenderPillow();
}

/***********************************************************
 *  RenderFloor()
 *
 *  This method is called to render the shapes for the Floor
 *  object.
 ***********************************************************/
void SceneManager::RenderFloor() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//set shader texture and uv scale
	SetShaderTexture("woodFloor");
	SetTextureUVScale(2.0, 2.0); // improve floor detail

	// set shader material
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();
}

/***********************************************************
 *  RenderWall()
 *
 *  This method is called to render the shapes for the Wall
 *  object.
 ***********************************************************/
void SceneManager::RenderWall() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/


	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(20.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 90.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 9.0f, -10.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//set shader texture and uv scale
	SetShaderTexture("beigeWall");
	SetTextureUVScale(2.0, 2.0);

	// set shader material
	SetShaderMaterial("wall");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();

}

/***********************************************************
 *  RenderRug()
 *
 *  This method is called to render the shapes for the Rug
 *  object.
 ***********************************************************/
void SceneManager::RenderRug() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(9.0f, 1.0f, 6.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, 0.1f, -4.0f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//set shader texture and uv scale
	SetShaderTexture("carpet");
	SetTextureUVScale(5.0, 5.0); // improve rug detail

	// set shader material
	SetShaderMaterial("wall");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();
}

/***********************************************************
 *  RenderTable()
 *
 *  This method is called to render the shapes for the Table
 *  object.
 ***********************************************************/
void SceneManager::RenderTable() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	//End Table left leg back
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.25f, 4.0f, 0.25f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(12.0f, 2.0f, -7.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table left leg front
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.25f, 4.0f, 0.25f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(12.0f, 2.0f, -5.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table right leg back
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.25f, 4.0f, 0.25f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(14.0f, 2.0f, -7.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table right leg front
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.25f, 4.0f, 0.25f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(14.0f, 2.0f, -5.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table bottom shelf
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(1.13f, 20.0f, 1.13f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 1.0f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawPlaneMesh();
	}

	//End Table drawer
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(2.0f, 1.0f, 2.0f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 3.5f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table top
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(1.3f, 20.0f, 1.3f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 4.03f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("woodTable");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("wood");

		// draw the mesh with transformation values
		m_basicMeshes->DrawPlaneMesh();
	}

	//End Table drawer handle left
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.1f, 0.1f, 0.3f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.5f, 3.5f, -4.7f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("blackMetal");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("metal");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table drawer handle right
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.1f, 0.1f, 0.3f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(12.5f, 3.5f, -4.7f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("blackMetal");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("metal");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}

	//End Table drawer handle middle
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(1.1f, 0.1f, 0.1f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = .0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 3.5f, -4.5f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("blackMetal");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("metal");

		// draw the mesh with transformation values
		m_basicMeshes->DrawBoxMesh();
	}
}

/***********************************************************
 *  RenderLamp()
 *
 *  This method is called to render the shapes for the Lamp
 *  object.
 ***********************************************************/
void SceneManager::RenderLamp() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	
	//Lamp base
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.5f, 0.09f, 0.5f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 4.02f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);


		// draw top lamp base
		{
			//set shader texture and uv scale
			SetShaderTexture("marble");
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("metal");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh(true, false, false); // draw only top with marble texture
		}

		//draw rest of lamp base
		{

			//set shader texture and uv scale
			SetShaderTexture("blackMetal");
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("metal");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh(false, true, true); // draw remaining with black metal
		}
	}

	//lamp base 2
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.1f, 1.3f, 0.1f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = .0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 4.07f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("blackMetal");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("metal");

		// draw the mesh with transformation values
		m_basicMeshes->DrawCylinderMesh();
	}

	//light bulb base
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.13f, 0.18f, 0.13f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = .0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 5.2f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("MetalBulb");
		SetTextureUVScale(1.0, 9.0); // resemble the appearance of bulb base

		// set shader material
		SetShaderMaterial("metal");

		// draw the mesh with transformation values
		m_basicMeshes->DrawCylinderMesh();
	}

	//light bulb 
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.2f, 0.2f, 0.2f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = 0.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 5.5f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale
		SetShaderTexture("glassBulb");
		SetTextureUVScale(1.0, 1.0);


		// draw the mesh with transformation values
		m_basicMeshes->DrawSphereMesh();
	}

	//lamp shade 
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(0.9f, 1.0f, 0.9f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 0.0f;
		YrotationDegrees = .0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(13.0f, 5.35f, -6.0f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);


		//set shader texture and uv scale 
		SetShaderTexture("lampShadeCanvas"); // created new texture to replicate a glowing shade
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("fabric");

		// draw the mesh with transformation values
		m_basicMeshes->DrawTaperedCylinderMesh(false, false, true);
	}
}

/***********************************************************
 *  RenderCouch()
 *
 *  This method is called to render the shapes for the Couch
 *  object.
 *	I left slight gaps in between to resemble the gaps bewteen the cushions and the frame pieces
 *	Increased uv scale so the couch frame and cushions will be slighlty different
 ***********************************************************/
void SceneManager::RenderCouch() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	//couch front left leg
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.35f, 0.9f, 0.35f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 180.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-7.0f, 0.9f, -2.0f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);


			//set shader texture and uv scale 
			SetShaderTexture("woodTable");
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("wood");

			// draw the mesh with transformation values
			m_basicMeshes->DrawTaperedCylinderMesh();
		}

		//couch back left leg
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.35f, 0.9f, 0.35f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 180.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-7.0f, 0.9f, -9.0f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);


			//set shader texture and uv scale 
			SetShaderTexture("woodTable");
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("wood");

			// draw the mesh with transformation values
			m_basicMeshes->DrawTaperedCylinderMesh();
		}

		//couch front right leg
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.35f, 0.9f, 0.35f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 180.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(7.0f, 0.9f, -2.0f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("woodTable");
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("wood");

			// draw the mesh with transformation values
			m_basicMeshes->DrawTaperedCylinderMesh();
		}

		//couch back right leg
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.35f, 0.9f, 0.35f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 180.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(7.0f, 0.9f, -9.0f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);


			//set shader texture and uv scale 
			SetShaderTexture("woodTable"); // created new texture to replicate a glowing shade
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("wood");

			// draw the mesh with transformation values
			m_basicMeshes->DrawTaperedCylinderMesh();
		}

		//couch arm rest left
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.8f, 4.0f, 7.75f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-7.0f, 2.9f, -5.4f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric"); 
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawBoxMesh();
		}

		//couch arm rest right
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(0.8f, 4.0f, 7.75f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(7.0f, 2.9f, -5.4f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);


			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric"); 
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawBoxMesh();
		}

		//couch base left
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(6.63f, 0.8f, 7.6f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-3.33f, 1.31f, -5.33f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);


			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric"); 
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawBoxMesh();
		}

		//couch base right
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(6.63f, 0.8f, 7.6f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(3.33f, 1.31f, -5.33f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric"); 
			SetTextureUVScale(1.0, 1.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawBoxMesh();
		}

		//couch back rest
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(13.2f, 4.0f, 0.8f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = -8.0f;// slight angle to reslemble a slight recline
			YrotationDegrees = 0.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(0.0f, 3.0f, -8.7f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric"); 
			SetTextureUVScale(1.0, 1.0);


			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawBoxMesh();
		}

		//couch base left cushion roundness
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(3.3f, 6.45f, 0.4f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 90.0f;
			YrotationDegrees = 90.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-6.5f, 2.0f, -4.8f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric");
			SetTextureUVScale(2.0, 2.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh();
		}

		//couch base right cushion roundness
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(3.3f, 6.45f, 0.4f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 90.0f;
			YrotationDegrees = 90.0f;
			ZrotationDegrees = 0.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(0.0f, 2.0f, -4.8f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric");
			SetTextureUVScale(2.0, 2.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh();
		}

		//couch back left cushion roundness
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(3.0f, 6.45f, 0.4f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = -20.0f;
			ZrotationDegrees = -90.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(-6.5f, 4.0f, -7.8f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric");
			SetTextureUVScale(2.0, 2.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh();
		}

		//couch back right cushion roundness
		{
			// set the XYZ scale for the mesh
			scaleXYZ = glm::vec3(3.0f, 6.45f, 0.4f);

			// set the XYZ rotation for the mesh
			XrotationDegrees = 0.0f;
			YrotationDegrees = -20.0f;
			ZrotationDegrees = -90.0f;

			// set the XYZ position for the mesh
			positionXYZ = glm::vec3(0.0f, 4.0f, -7.8f);

			// set the transformations into memory to be used on the drawn meshes
			SetTransformations(
				scaleXYZ,
				XrotationDegrees,
				YrotationDegrees,
				ZrotationDegrees,
				positionXYZ);

			//set shader texture and uv scale 
			SetShaderTexture("cushionFabric");
			SetTextureUVScale(2.0, 2.0);

			// set shader material
			SetShaderMaterial("fabric");

			// draw the mesh with transformation values
			m_basicMeshes->DrawCylinderMesh();
		}


}

/***********************************************************
 *  RenderPillow()
 *
 *  This method is called to render the shapes for the Pillow
 *  object.
 *	Made pillow round square shape looked more out of place
 ***********************************************************/
void SceneManager::RenderPillow() {

	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	// pillow base
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(1.4f, 0.7f, 1.4f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 40.0f;
		YrotationDegrees = -45.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(5.3f, 3.2f, -5.9f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale 
		SetShaderTexture("cushionFabric");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("fabric");

		// draw the mesh with transformation values
		m_basicMeshes->DrawCylinderMesh();
	}

	// pillow top
	{
		// set the XYZ scale for the mesh
		scaleXYZ = glm::vec3(1.4f, 0.4f, 1.4f);

		// set the XYZ rotation for the mesh
		XrotationDegrees = 40.0f;
		YrotationDegrees = -45.0f;
		ZrotationDegrees = 0.0f;

		// set the XYZ position for the mesh
		positionXYZ = glm::vec3(5.0f, 3.7f, -5.56f);

		// set the transformations into memory to be used on the drawn meshes
		SetTransformations(
			scaleXYZ,
			XrotationDegrees,
			YrotationDegrees,
			ZrotationDegrees,
			positionXYZ);

		//set shader texture and uv scale 
		SetShaderTexture("pillowFront");
		SetTextureUVScale(1.0, 1.0);

		// set shader material
		SetShaderMaterial("fabric");

		// draw the mesh with transformation values
		m_basicMeshes->DrawHalfSphereMesh();
	}
}

