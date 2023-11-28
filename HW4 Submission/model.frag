#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/*uniform variables*/
uniform float iTime;					////time

/*input variables*/
in vec3 vtx_pos;

/*input variables*/
out vec4 frag_color;

///////////// Part 1a /////////////////////
/* Create a function that takes in an xy coordinate and returns a 'random' 2d vector. (There is no right answer)
   Feel free to find a hash function online. Use the commented function to check your result */
vec2 hash2(vec2 v)
{
	// vec2 rand = vec2(0,0);

	// Sample hash function
	// rand  = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    // rand = -1.0 + 2 * 1.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);
	// return rand;

	// Your implementation starts here

	// Below is from the shadertoy example
	v = vec2(dot(v, vec2(127.1, 311.7)), dot(v, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(v) * 43758.5453123);

	// Your implementation ends here
}

///////////// Part 1b /////////////////////
/*  Using i, f, and m, compute the perlin noise at point v */
float perlin_noise(vec2 v) 
{
	float noise = 0;

	// Your implementation starts here
	vec2 i = floor(v); 
	vec2 f = fract(v);
	vec2 u = f*f*(3.0-2.0*f);

	noise = mix(mix(dot(hash2(i + vec2(0.0,0.0)), f - vec2(0.0,0.0)), 
					dot(hash2(i + vec2(1.0,0.0)), f - vec2(1.0,0.0)), u.x),
				mix(dot(hash2(i + vec2(0.0,1.0)), f - vec2(0.0,1.0)), 
					dot(hash2(i + vec2(1.0,1.0)), f - vec2(1.0,1.0)), u.x), u.y);
	// Your implementation ends here

	return noise;
}

///////////// Part 1c /////////////////////
/*  Given a point v and an int num, compute the perlin noise octave for point v with octave num
	num will be greater than 0 */
float noiseOctave(vec2 v, int num)
{
	float sum = 0.0;
	
	// Your implementation starts here
	float w = 1.0; // amplitude - gets decreased
	float s = 1.0; // frequency - gets increased

	for(int i=0; i<num; i++){
		sum += w * perlin_noise(s*v);
		w /= 2.0;
		s *= 2.0;
	}
	// Your implementation ends here

	return sum;
}

///////////// Part 2a /////////////////////
/* create a function that takes in a 2D point and returns a height using perlin noise 
    There is no right answer. Think about what functions will create what shapes.
    If you want steep mountains with flat tops, use a function like sqrt(noiseOctave(v,num)). 
    If you want jagged mountains, use a function like e^(noiseOctave(v,num))
    You can also add functions on top of each other and change the frequency of the noise
    by multiplying v by some value other than 1*/
float height(vec2 v)
{
    float h = 0;
	// Your implementation starts here

	// h = exp(noiseOctave(v,8));
	// h = 0.75 * noiseOctave(v,10);
	h = sin(noiseOctave(v,6)); 
	if (h < 0) {
		h *= 0.2; // for advanced feature
		//h *= 0.5; 
	}
	
	// Your implementation ends here
	return h;
}

///////////// Part 2b /////////////////////
/* compute the normal vector at v by find the points d to the left/right and d forward/backward 
    and using a cross product. Be sure to normalize the result */
vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	// Your implementation starts here

	vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
	vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
	vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
	vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));


	normal_vector = normalize(cross(v2-v1, v4-v3)); // ?????????

	// Your implementation ends here
	return normal_vector;
}

///////////// Part 2c /////////////////////
/* complete the get_color function by setting emissiveColor using some function of height/normal vector/noise */
/* put your Phong/Lambertian lighting model here to synthesize the lighting effect onto the terrain*/
vec3 get_color(vec2 v)
{
    // Your implementation starts here

    // Height-based color map
	vec3 vtx_normal = compute_normal(v, 0.01);
	vec3 emissiveColor = vec3(0,0,0);
	vec3 lightingColor= vec3(1.,1.,1.);

	// Your implementation starts here
	// vec3 topColor = vec3(80, 75, 0) / 255.;
	// vec3 bottomColor = vec3(80, 90, 40) / 255.;
	// emissiveColor = mix(bottomColor, topColor, height(v)*5);

	// Advanced feature below
	vec3 topColor = vec3(1, 1, 1);
	vec3 bottomColor = vec3(0.3, 0.4, 0.2);

	if (height(v) < 0) {
		emissiveColor = vec3(0.5, 0.7, 0.9); // water color
	}
	else {
		emissiveColor = mix(bottomColor, topColor, height(v)*3); // snow at the top of mountains
	}


	/*This part is the same as your previous assignment. Here we provide a default parameter set for the hard-coded lighting environment. Feel free to change them.*/
	const vec3 LightPosition = vec3(3, 2, 3);
	const vec3 LightIntensity = vec3(15);
	const vec3 ka = 0.1*vec3(1., 1., 1.);
	const vec3 kd = 0.7*vec3(1., 1., 1.);
	const vec3 ks = vec3(2.);
	const float n = 400.0;

	vec3 normal = normalize(vtx_normal.xyz);
	vec3 lightDir = LightPosition - vtx_pos;
	float _lightDist = length(lightDir);
	vec3 _lightDir = normalize(lightDir);
	vec3 _localLight = LightIntensity / (_lightDist * _lightDist);
	vec3 ambColor = ka;
	lightingColor = kd * _localLight * max(0., dot(_lightDir, normal));

	// Your implementation ends here

    return emissiveColor*lightingColor;


}

void main()
{
	frag_color = vec4(get_color(vtx_pos.xy),1.f);
}