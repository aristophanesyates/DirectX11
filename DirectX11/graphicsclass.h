// The graphics class is the other object that is created by the system class. 
// All the graphics functionality in this application will be encapsulated in this class.
// I will also use the header in this file for all the graphics related global settings that we may want to change 
// such as full screen or windowed mode. 
// Currently this class will be empty but in future tutorials will contain all the graphics objects.
// GraphicsClass now has the three new classes added to it. 
// CameraClass, ModelClass, and ColorShaderClass have headers added here as well as private member variables. 
// Remember that GraphicsClass is the main class that is used to render the scene by invoking all the needed class objects for the project.
////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// Here is the first change. We have taken out the include for windows.h and instead included the new d3dclass.h.
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"
// The GraphicsClass now includes the new TextureShaderClass header.
#include "textureshaderclass.h"

// We'll need these four globals to start with.
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	// And the second change is the new private pointer to the D3DClass which we have called m_D3D. 
	// In case you were wondering I use the prefix m_ on all class variables. That way when I'm coding I can remember quickly 
	// what variables are members of the class and which are not.
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;

	//A new TextureShaderClass private object has been added.

	TextureShaderClass* m_TextureShader;
};

#endif