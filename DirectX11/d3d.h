////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// First thing in the header is to specify the libraries to link when using this object module. 
// These libraries contain all the Direct3D functionality for setting up and drawing 3D graphics in DirectX as well as tools 
// to interface with the hardware on the computer to obtain information about the refresh rate of the monitor, 
// the video card being used, and so forth. 
// You will notice that some DirectX 10 libraries are still used, this is because those libraries were never upgraded for DirectX 11 
// as their functionality did not need to change.
/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// The next thing we do is include the headers for those libraries that we are linking to this object module as well as headers 
// for DirectX type definitions and such.
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// The class definition for the D3DClass is kept as simple as possible here. 
// It has the regular constructor, copy constructor, and destructor. 
// Then more importantly it has the Initialize and Shutdown function. 
// This will be what we are mainly focused on in this tutorial. 
// Other than that I have a couple helper functions which aren't important to this tutorial and a number of private member variables 
// that will be looked at when we examine the d3dclass.cpp file. 
// For now just realize the Initialize and Shutdown functions are what concerns us.
////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3D
{
public:
	D3D();
	D3D(const D3D&);
	~D3D();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(Matrix&);
	void GetWorldMatrix(Matrix&);
	void GetOrthoMatrix(Matrix&);

	void GetVideoCardInfo(char*, int&);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	Matrix m_projectionMatrix;
	Matrix m_worldMatrix;
	Matrix m_orthoMatrix;
};

#endif