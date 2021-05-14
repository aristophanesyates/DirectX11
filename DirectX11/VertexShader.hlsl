// The texture vertex shader is similar to the previous color shader except that there have been some changes to accommodate texturing.
////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// We are no longer using color in our vertex type and have instead moved to using texture coordinates. 
// Since texture coordinates take a U and V float coordinate we use float2 as its type. 
// The semantic for texture coordinates is TEXCOORD0 for vertex shaders and pixel shaders. 
// You can change the zero to any number to indicate which set of coordinates you are working with as multiple texture coordinates are allowed.
//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);


    output.color = input.color;

    return output;
}