/*Hello fragment shader!*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/* Passed time from the begining of the program */ 
uniform float iTime;

/*input variables*/
// from the vertex shader
in vec4 vtx_color;
////TODO: define your own data channel(s) to receive the normal attribute from the vertex shader
in vec3 vtx_nml; // connects to vtx shader 
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;	/*or use gl_FragColor*/


void main()							
{	
	////This is the default implementation of the fragment color. Change it to the color depending on the input normal value.

	// Choose one of the below frag_color and comment out the rest for different spherical designs
	// frag_color = vec4(vtx_color.rgb,1.f);
	frag_color = vec4(vtx_nml.rgb, 1.f); // use this frag_color for Step 1
	// frag_color = vec4(sin(vtx_nml.r), cos(vtx_nml.g), sin(vtx_nml.b), 1.f);
	// frag_color = vec4(vtx_nml.g, vtx_nml.r, vtx_nml.r, 1.f);
	// frag_color = vec4(vtx_color.g, cos(vtx_nml.r), vtx_nml.g, 1.f);
	// frag_color = vec4(mix(vtx_color.rgb, vtx_nml.rgb, vtx_pos.y), 1.f);
	// frag_color = vec4(mix(vec3(0.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), vtx_pos.z), 1.f);
}	