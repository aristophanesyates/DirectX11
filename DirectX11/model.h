
// As stated previously the ModelClass is responsible for encapsulating the geometry for 3D models.
// In this tutorial we will manually setup the data for a single green triangle.
// We will also create a vertex and index buffer for the triangle so that it can be rendered.
// The ModelClass has had changes since the previous tutorial so that it can now accommodate texturing.
////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

// The TextureClass header is now included in the ModelClass header.
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class Model
{
private:
	// Here is the definition of our vertex type that will be used with the vertex buffer in this ModelClass. 
	// Also take note that this typedef must match the layout in the ColorShaderClass that will be looked at later in the tutorial.
	/*struct VertexType
	{
		Vector3 position;
	    Vector4 color;
	};*/

	// The VertexType has replaced the color component with a texture coordinate component. 
	// The texture coordinate is now replacing the green color that was used in the previous tutorial.
	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	// The functions here handle initializing and shutdown of the model's vertex and index buffers. 
	// The Render function puts the model geometry on the video card to prepare it for drawing by the color shader.
	//bool Initialize(ID3D11Device*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	// The ModelClass also has a GetTexture function so it can pass its own texture resource to shaders that will draw this model.
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	// ModelClass has both a private LoadTexture and ReleaseTexture for loading and releasing the texture that will be used to render this model.
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext, WCHAR*);
	void ReleaseTexture();

private:
	// The private variables in the ModelClass are the vertex and index buffer as well as two integers to keep track of the size of each buffer. 
	// Note that all DirectX 11 buffers generally use the generic ID3D11Buffer type and are more clearly identified by a buffer description 
	// when they are first created.
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	// The m_Texture variable is used for loading, releasing, and accessing the texture resource for this model.
	Texture* m_Texture;
};

#endif