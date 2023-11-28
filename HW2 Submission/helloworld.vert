/*Hello vertex shader!*/

#version 330 core

/*default camera matrices --- do not modify*/
// defined properties for the camera - matrix properties
// camera.position - position of "i"? 
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*input variables from CPU*/
// 3 arrays that we set/transfer from CPU to GPU
// don't need to change these
// 3 input channels
layout (location=0) in vec4 pos;			/*vertex position*/
layout (location=1) in vec4 v_color;		/*vertex color*/
layout (location=2) in vec4 normal;			/*vertex normal*/

/*output variables*/
// 1 output channel to fragment shader - only thing sent from vtx shader to frag shader
// vtx_color copied into v_color below
out vec4 vtx_color;

////TODO: define your own data channel(s) to send the normal attribute from the vertex shader to the fragment shader
// displaying color (from class)
out vec3 vtx_nml; // send array to frag shader
out vec3 vtx_pos;

void main()												
{
	/*camera-transformed position. do not modify.*/
	// use pvm matrix to multiply position and forward data to next stage
	// deals with model transformation 
	gl_Position=pvm*vec4(pos.xyz,1.f); 

	vtx_pos = vec3(pos.xyz);

	vtx_color=vec4(v_color.rgb,1.f);

	vtx_nml=vec3(normal.rgb);
}	