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
const float p = 10;
/*input variables*/
in vec4 vtx_color;
////TODO [Step 2]: add your in variables from the vertex shader
in vec3 vtx_nml;
in vec3 vtx_pos;


/*output variables*/
out vec4 frag_color;

/*hard-coded lighting parameters*/
// vec3 LightPosition = vec3(3,1,3); // bunny.obj
// const vec3 LightPosition = vec3(60, 0, 30); // for dog3.obj
vec3 LightPosition = vec3(60*sin(iTime*2), 60*cos(iTime*2), 50); // advanced lighting effect - rotating light

////TODO [Step 3]: add your Phong lighting parameters here
const float ka = 0.2;
const float kd = 0.8;
const float ks = 1.5;

const vec3 ia = vec3(0.5, 0.5, 0.5);
//const vec3 id = vec3(1.0, 1.0, 1.0); // use this id for regular phong shading
const vec3 is = vec3(1.0, 1.0, 1.0);

// Advanced lighting effect - light source changing color 
const vec3 orange = vec3(1.0, 0.5, 0.0);
const vec3 green = vec3(0.0, 0.9, 0.0);
const vec3 brown = vec3(0.6, 0.4, 0.2);
const vec3 yellow = vec3(1.0, 1.0, 0.0);
const vec3 colors[4] = vec3[](orange, green, brown, yellow);
int color_idx = int(iTime / 3.0) % 4;
vec3 color1 = colors[int(color_idx)];
vec3 color2 = colors[int(color_idx + 1) % 4];
float t = mod(iTime, 3.0) / 3.0;
vec3 id = mix(color1, color2, t);


void main()							
{	
	////TODO [Step 3]: add your Phong lighting calculation

	// Ambient
    vec3 ambient = ka * ia;

	// Diffuse
    vec3 n = normalize(vtx_nml);
    vec3 l = normalize(LightPosition - vtx_pos);
	vec3 diffuse = max(dot(n,l), 0.0) * kd * id * vtx_color.rgb;

	// Specular
    vec3 r = normalize(reflect(-l, vtx_nml));
    vec3 v = normalize(position.xyz - vtx_pos);
    vec3 specular = pow(max(dot(v,r), 0.0), p) * ks * is;

	// Final output color
	frag_color = vec4((ambient + diffuse + specular), 1.f); // use this for Step 3 just for sphere
    
}	


