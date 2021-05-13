////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"

// The class constructor initializes the vertex and index buffer pointers to null.
// The class constructor now initializes the new texture object to null.
ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

// The Initialize function will call the initialization functions for the vertex and index buffers.
// Initialize now takes as input the file name of the .dds texture that the model will be using.
//bool ModelClass::Initialize(ID3D11Device* device)
bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename)
{
	bool result;


	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// The Initialize function now calls a new private function that will load the texture.

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

// The Shutdown function will call the shutdown functions for the vertex and index buffers.
void ModelClass::Shutdown()
{
	// The Shutdown function now calls a new private function to release the texture object that was loaded during initialization.
	 
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}

// Render is called from the GraphicsClass::Render function. 
// This function calls RenderBuffers to put the vertex and index buffers on the graphics pipeline so the color shader will be able to render them.
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

// GetIndexCount returns the number of indices in the model. The color shader will need this information to draw this model.
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// GetTexture returns the model texture resource. The texture shader will need access to this texture to render the model.
ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

// The InitializeBuffers function is where we handle creating the vertex and index buffers. 
// Usually you would read in a model and create the buffers from that data file. 
// For this tutorial we will just set the points in the vertex and index buffer manually since it is only a single triangle.
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points. 
	// Please note that I create the points in the clockwise order of drawing them. 
	// If you do this counter clockwise it will think the triangle is facing the opposite direction and not draw it due to back face culling. 
	// Always remember that the order in which you send your vertices to the GPU is very important. 
	// The color is set here as well since it is part of the vertex description. I set the color to green.
	// The vertex array now has a texture component instead of a color component. 
	// The texture vector is always U first and V second. 
	// For example the first texture coordinate is bottom left of the triangle which corresponds to U 0.0, V 1.0. 
	// Use the diagram at the top of this page to figure out what your coordinates need to be. 
	// Note that you can change the coordinates to map any part of the texture to any part of the polygon face. 
	// In this tutorial I'm just doing a direct mapping for simplicity reasons.

	// Load the vertex array with data.
	//vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[1].position = Vector3(0.0f, 1.0f, 0.0f);  // Top middle.
	//vertices[1].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].position = Vector3(1.0f, -1.0f, 0.0f);  // Bottom right.
	//vertices[2].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the vertex array with data.
	vertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = Vector2(0.0f, 1.0f);

	vertices[1].position = Vector3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = Vector2(0.5f, 0.0f);

	vertices[2].position = Vector3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = Vector2(1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// With the vertex array and index array filled out we can now use those to create the vertex buffer and index buffer. 
	// Creating both buffers is done in the same fashion. 
	// First fill out a description of the buffer. 
	// In the description the ByteWidth (size of the buffer) and the BindFlags (type of buffer) are what you need to ensure are filled out correctly. 
	// After the description is filled out you need to also fill out a subresource pointer which will point to either your vertex or index array
	// you previously created. 
	// With the description and subresource pointer you can call CreateBuffer using the D3D device and it will return a pointer to your new buffer.

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// After the vertex buffer and index buffer have been created you can delete the vertex and index arrays as they are no longer needed 
	// since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

// The ShutdownBuffers function just releases the vertex buffer and index buffer that were created in the InitializeBuffers function.
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

// RenderBuffers is called from the Render function. 
// The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU. 
// Once the GPU has an active vertex buffer it can then use the shader to render that buffer. 
// This function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth. 
// In this tutorial we set the vertex buffer and index buffer as active on the input assembler and tell the GPU that the buffers should 
// be drawn as triangles using the IASetPrimitiveTopology DirectX function.
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

// LoadTexture is a new private function that will create the texture object and then initialize it with the input file name provided. 
// This function is called during initialization.
bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

// The ReleaseTexture function will release the texture object that was created and loaded during the LoadTexture function.
void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}