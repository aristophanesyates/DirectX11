////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texture.h"

// The class constructor will initialize the texture shader resource pointer to null.
Texture::Texture()
{
	m_texture = 0;
}


Texture::Texture(const Texture& other)
{
}


Texture::~Texture()
{
}

// Initialize takes in the Direct3D device and file name of the texture and then loads the texture file into the shader resource 
// variable called m_texture. The texture can now be used to render with.
bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)			// <--- pass in device context.
{
	HRESULT result;

	// Load the texture in.
	result = CreateDDSTextureFromFile(device, deviceContext, filename, NULL, &m_texture);
	//result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

// The Shutdown function releases the texture resource if it has been loaded and then sets the pointer to null.
void Texture::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}

// GetTexture is the function that is called by other objects that need access to the texture shader resource so that they can 
// use the texture for rendering.
ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}