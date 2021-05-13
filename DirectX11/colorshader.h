// The ColorShaderClass is what we will use to invoke our HLSL shaders for drawing the 3D models that are on the GPU.
////////////////////////////////////////////////////////////////////////////////
// Filename: colorshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

#include <d3dcompiler.h>

#include <fstream>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


////////////////////////////////////////////////////////////////////////////////
// Class name: ColorShaderClass
////////////////////////////////////////////////////////////////////////////////
class ColorShader
{
private:

	// Here is the definition of the cBuffer type that will be used with the vertex shader. 
	// This typedef must be exactly the same as the one in the vertex shader as the model data needs to match the typedefs in the shader 
	// for proper rendering.
	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	// The functions here handle initializing and shutdown of the shader. 
	// The render function sets the shader parameters and then draws the prepared model vertices using the shader.
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, Matrix, Matrix, Matrix);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, Matrix, Matrix, Matrix);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif