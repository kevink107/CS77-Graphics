/*This is your fragment shader for texture and normal mapping*/

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

/*uniform variables*/
uniform float iTime;					////time
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

/*input variables*/
//// TODO: declare your input variables
in vec2 vtx_uv;
in vec3 vtx_normal;
in vec3 vtx_pos;
in vec3 vtx_tangent;
in vec4 vtx_color;

/*output variables*/
out vec4 frag_color;

/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
const vec3 LightPosition = vec3(3, 1, 3); // used this position for bunny and earth meshes
//const vec3 LightPosition = vec3(20, 18, 30); // used this position for jukebox mesh
const vec3 LightIntensity = vec3(50);
const vec3 ka = 0.1*vec3(1., 1., 1.);
const vec3 kd = 0.7*vec3(1., 1., 1.);
const vec3 Ia = vec3(0.4, 0.4, 0.4);
const vec3 Id = vec3(1.5, 1.5, 1.5);
const vec3 ks = vec3(2.);
const float p = 400.0;

//// UV spherical coordinates - uncomment these lines to get rid of the seam problem for the sphere
// const float PI = 3.1415926535;
// float radius = sqrt(vtx_pos.x*vtx_pos.x + vtx_pos.y*vtx_pos.y + vtx_pos.z*vtx_pos.z);
// float theta = asin(-vtx_pos.y/radius); 
// float phi = atan(-vtx_pos.z,vtx_pos.x);
// float u_coord = 0.5 + (phi / (2.0*PI));
// float v_coord = 0.5 + (theta / PI); 

void main()							
{		
	bool use_normal_mapping = true;	////TODO: set this flag to be true when you move from Step 2 to Step 3

	if(!use_normal_mapping){
		//// TODO (Step 1 and 2): texture with shading
		////Here are some useful hints:
		////Step 1.0: load the texture color from tex_albedo and then set it to frag_color
		////Step 2.0: use the loaded texture color as the local material color and multiply the local color with the Lambertian shading model you implemented previously to render a textured and shaded sphere.
		////The way to read a texture is to call texture(texture_name,uv). It will return a vec4.

		//// Using the commmented line below for the uv variable got rid of the high frequency line at the seam of the sphere
		//vec2 uv = vec2(u_coord,v_coord);
		vec2 uv = vtx_uv;

		// Step 1
		vec4 tex_color = texture(tex_albedo, uv); // loaded texture color
		//frag_color = vec4(tex_color.rgb, 1.f); // frag_color for step 1

		// Step 2 - Lambertian shading
		// Ambient
		vec3 ambient = ka * Ia;

		// Diffuse 
		vec3 n = normalize(vtx_normal); // assuming sphere normal?
		vec3 l = normalize(LightPosition - vtx_pos);
		vec3 diffuse = max(dot(n,l), 0.0) * kd * Id * tex_color.rgb;

		// Final output color
		frag_color = vec4((diffuse + ambient), 1.f);
	}
	else{
		//// TODO (Step 3): texture with normal mapping
		////Here are some useful hints:
		////Step 3.0: load the texture color from tex_albedo
		////Step 3.1: load the texture normal from tex_normal, and remap each component from [0, 1] to [-1, 1] (notice that currently the loaded normal is in the local tangent space)
		////Step 3.2: calculate the TBN matrix using the vertex normal and tangent
		////Step 3.3: transform the texture normal from the local tangent space to the global world space
		////Step 3.4 and following: use the transformed normal and the loaded texture color to conduct the further lighting calculation
		////The way to declare a 3x3 matrix is mat3 mat=mat3(v0,v1,v2);
		////The way to read a texture is to call texture(texture_name,uv). It will return a vec4.
		////The way to calculate cross product is to call cross(u1,u2);

		// Step 3.0: load the texture color from tex_albedo
		vec2 uv = vtx_uv;
		vec4 tex_color = texture(tex_albedo, uv);
		
		// Step 3.1: load the texture normal from tex_normal, and remap each component from [0, 1] to [-1, 1] (notice that currently the loaded normal is in the local tangent space)
		vec3 tex_nml = 2.0 * texture(tex_normal, uv).rgb - 1.0;
		
		// Step 3.2:  calculate the TBN matrix using the vertex normal and tangent
		vec3 T = normalize(vtx_tangent);
		vec3 N = normalize(vtx_normal);
		T = normalize(T - dot(T,N) * N); 
		vec3 B = normalize(cross(vtx_normal, vtx_tangent));
		mat3 TBN = mat3(T, B, N);

		// Step 3.3: transform the texture normal from the local tangent space to the global world space
		tex_nml = normalize(TBN * tex_nml);

		// Step 3.4 and following: use the transformed normal and the loaded texture color to conduct the further lighting calculation
		// Ambient
		vec3 ambient = ka * Ia;

		// Diffuse 
		vec3 n = normalize(tex_nml); // assuming sphere normal?
		vec3 l = normalize(LightPosition - vtx_pos);
		vec3 diffuse = max(dot(l,n), 0.0) * kd * Id * tex_color.rgb;
		
		// Final output color
		frag_color = vec4((ambient + diffuse),1);
	}
}	