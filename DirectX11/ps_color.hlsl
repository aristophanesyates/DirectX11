// The pixel shader draws each pixel on the polygons that will be rendered to the screen. 
// In this pixel shader it uses PixelInputType as input and returns a float4 as output which represents the final pixel color. 
// This pixel shader program is very simple as we just tell it to color the pixel the same as the input value of the color. 
// Note that the pixel shader gets its input from the vertex shader output.

struct InputType
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{
	input.colour = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return input.colour;
}

