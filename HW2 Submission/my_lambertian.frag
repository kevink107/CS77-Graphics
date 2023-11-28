/*This is your first fragment shader!*/

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
in vec4 vtx_color;
////TODO [Step 2]: add your in variables from the vertex shader
in vec3 nml;
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color; // L_0

/*hard-coded lighting parameters*/
const vec3 LightPosition = vec3(60, 0, 30); // for dog 
// const vec3 LightPosition = vec3(3,1,3); // for bunny
////TODO [Step 2]: add your Lambertian lighting parameters here
const float ka = 0.2;
const float kd = 1.0;
const vec3 ia = vec3(0.5, 0.5, 0.5);
const vec3 id = vec3(1.2, 1.2, 1.2);

void main()							
{		
	////TODO [Step 2]: add your Lambertian lighting calculation
	// ambient
	vec3 ambient = ka*ia;

	// diffuse
	vec3 n = normalize(nml);
	vec3 l = normalize(LightPosition - vtx_pos);
	vec3 diffuse = max(dot(n,l), 0.0) * kd * id * vtx_color.rgb;
	
	// final output color
	frag_color = vec4((diffuse + ambient), 1.f);
}	