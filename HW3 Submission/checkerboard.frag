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

/*input variables*/
//// TODO: declare the input fragment attributes here
in vec2 vtx_uv;
in vec3 vtx_pos;

/*output variables*/
out vec4 frag_color;
const float PI = 3.1415926535;

void main()							
{						
	vec3 col = vec3(1.0);
	
	//// TODO: produce a checkerboard texture on the sphere with the input vertex uv

	// UV spherical coordinates for sphere
	float radius = sqrt(vtx_pos.x*vtx_pos.x + vtx_pos.y*vtx_pos.y + vtx_pos.z*vtx_pos.z);
	float theta = asin(-vtx_pos.y/radius); 
	float phi = atan(-vtx_pos.z, vtx_pos.x);
	float u = 0.5 + (phi / (2.0*PI));
	float v = 0.5 + (theta / PI); 
	
	// Checkerboard pattern
	float checker = mod(floor(u * 8.0) + floor(v * 8.0), 2.0);
	if (checker < 1.0) {
		col = vec3(0.0);
	}

	// Final output color
	frag_color = vec4(col,1.f);

}	