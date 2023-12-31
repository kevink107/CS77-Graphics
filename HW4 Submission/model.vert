#version 330 core

// Add parts 1a-c first, then implement part 2a

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

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

	// Below is from shadertoy example
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

	// CHANGE THIS UP *********
	// Multiple noise octaves with different parameters
    // h += noiseOctave(v * 0.5, 4) * 0.5;
    // h += noiseOctave(v * 1.0, 6) * 0.25;
    // h += noiseOctave(v * 2.0, 8) * 0.125;
    // h += noiseOctave(v * 4.0, 10) * 0.0625;

    // // Non-linear functions to shape the terrain
    // h = pow(h, 1.5);
    // h = mix(h, 1.0, smoothstep(0.4, 0.6, h));
    // h = pow(h, 1.2);

	// Your implementation ends here
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

/*uniform variables*/
uniform float iTime;					////time

/*input variables*/
layout (location=0) in vec4 pos;
layout (location=2) in vec4 normal;
layout (location=3) in vec4 uv;
layout (location=4) in vec4 tangent;

/*output variables*/
out vec3 vtx_pos;		////vertex position in the world space


void main()												
{
	vtx_pos = (vec4(pos.xy, height(pos.xy), 1)).xyz;
	gl_Position = pvm * vec4(vtx_pos,1);
}