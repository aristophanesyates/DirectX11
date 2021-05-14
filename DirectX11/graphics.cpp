// If you'll remember from the previous tutorial this class was entirely empty with no code in it at all. 
// Now that we have a D3DClass member we will start to fill out some code inside the GraphicsClass to initialize 
// and shutdown the D3DClass object. We will also add calls to BeginScene and EndScene in the Render function so that 
// we are now drawing to the window using Direct3D.
////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"

// So the very first change is in the class constructor.
// Here we initialize the pointer to null for safety reasons as we do with all class pointers.
// The first change to GraphicsClass is initializing the camera, model, and color shader objects in the class constructor to null.
Graphics::Graphics()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Shader = 0;
}


Graphics::Graphics(const Graphics& other)
{
}

Graphics::~Graphics()
{
}

// The second change is in the Initialize function inside the GraphicsClass.
// Here we create the D3DClass object and then call the D3DClass Initialize function.
// We send this function the screen width, screen height, handle to the window, and the four global variables 
// from the Graphicsclass.h file. 
// The D3DClass will use all these variables to setup the Direct3D system.
// We'll go into more detail about that once we look at the d3dclass.cpp file.
// The Initialize function has also been updated to create and initialize the three new objects.

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new D3D;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new Model;
	if (!m_Model)
	{
		return false;
	}

	// The ModelClass::Initialize function now takes in the name of the texture that will be used for rendering the model.

	// Initialize the model object.
	//result = m_Model->Initialize(m_Direct3D->GetDevice());
	WCHAR* texture_filepath = (WCHAR*)malloc(sizeof(WCHAR) * 64);
	std::copy(std::begin(L"seafloor.dds"), std::end(L"seafloor.dds"), texture_filepath);
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), texture_filepath);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// The new TextureShaderClass object is createdand initialized.

	// Create the texture shader object.
	m_Shader = new Shader;
	if (!m_Shader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_Shader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

// The next change is in the Shutdown function in the GraphicsClass.
// Shut down of all graphics objects occur here so we have placed the D3DClass shutdown in this function.
// Note that I check to see if the pointer was initialized or not.
// If it wasn't we can assume it was never set up and not try to shut it down. 
// That is why it is important to set all the pointers to null in the class constructor. 
// If it does find the pointer has been initialized then it will attempt to shut down the D3DClass and then clean up the pointer afterwards.
// Shutdown is also updated to shutdown and release the three new objects.
void Graphics::Shutdown()
{
	// The TextureShaderClass object is also released in the Shutdown function.

	// Release the texture shader object.
	if (m_Shader)
	{
		m_Shader->Shutdown();
		delete m_Shader;
		m_Shader = 0;
	}

	// Release the D3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}

// The Frame function has been updated so that it now calls the Render function each frame.
bool Graphics::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

// The final change to this class is in the Render function. 
// We call the D3D object to clear the screen to a grey color. 
// After that we call EndScene so that the grey color is presented to the window.
// As you would expect the Render function had the most changes to it. 
// It still begins with clearing the scene except that it is cleared to black. 
// After that it calls the Render function for the camera object to create a view matrix based on the camera's location that was set in 
// the Initialize function. 
// Once the view matrix is created we get a copy of it from the camera class. 
// We also get copies of the world and projection matrix from the D3DClass object. 
// We then call the ModelClass::Render function to put the green triangle model geometry on the graphics pipeline. 
// With the vertices now prepared we call the color shader to draw the vertices using the model information and the three matrices 
// for positioning each vertex. 
// The green triangle is now drawn to the back buffer. 
// With that the scene is complete and we call EndScene to display it to the screen.
bool Graphics::Render()
{
	Matrix viewMatrix, projectionMatrix, worldMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// The texture shader is called now instead of the color shader to render the model.Notice it also takes the texture resource pointer
	// from the model so the texture shader has access to the texture from the model object.

	// Render the model using the texture shader.
	result = m_Shader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									 m_Model->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}