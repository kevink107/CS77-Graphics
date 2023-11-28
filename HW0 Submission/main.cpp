// #####################################################################
//  Main
//  Dartmouth COSC 77/177 Computer Graphics, starter code
//  Contact: Bo Zhu (bo.zhu@dartmouth.edu)
// #####################################################################
#include <iostream>

#include <random>
#include "Common.h"
#include "Driver.h"
#include "Particles.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLParticles.h"

/////////////////////////////////////////////////////////////////////
//// TODO: put your name in the string
/////////////////////////////////////////////////////////////////////

const std::string author = "Kevin King";

/////////////////////////////////////////////////////////////////////
//// These are helper functions we created to generate circles and triangles by testing whether a point is inside the shape or not.
//// They can be used in the paintGrid function as "if the pixel is inside, draw some color; else skip."
//// You may create your own functions to draw your own shapes

//// The paintGrid function is implemented as a GLSL fragment shader.
//// The GLSL grammar is C-style, and if you are curious about its full picture (which we will start to learn the details in Week 3),
//// you may find more information on https://www.khronos.org/files/opengl43-quick-reference-card.pdf (Page 6 - 7 would probably be helpful!)
//// You don't need advanced GLSL features for this assignment (reading the starter code should be enough).
//// You can also test it (copy the whole string) in Shadertoy: https://www.shadertoy.com/new
/////////////////////////////////////////////////////////////////////

const std::string draw_pixels = To_String(
	const float M_PI = 3.1415926535;

	// The side length of the minimum unit (or the new "pixels")
	const float PIXEL_SIZE = 5.;

	// To check if a point is inside a circle
	bool inCircle(vec2 p, vec2 center, float radius) {
		vec2 to_center = p - center;
		if (dot(to_center, to_center) < radius * radius)
		{
			return true;
		}
		return false;
	}

	// To check if a point is inside a triangle
	bool inTriangle(vec2 p, vec2 p1, vec2 p2, vec2 p3) {
		if (dot(cross(vec3(p2 - p1, 0), vec3(p - p1, 0)), cross(vec3(p2 - p1, 0), vec3(p3 - p1, 0))) >= 0. &&
			dot(cross(vec3(p3 - p2, 0), vec3(p - p2, 0)), cross(vec3(p3 - p2, 0), vec3(p1 - p2, 0))) >= 0. &&
			dot(cross(vec3(p1 - p3, 0), vec3(p - p3, 0)), cross(vec3(p1 - p3, 0), vec3(p2 - p3, 0))) >= 0.)
		{
			return true;
		}
		return false;
	}

	// To check if a point is inside a rectangle
	bool inRectangle(vec2 p, vec2 c1, vec2 c2, float angle) {
		vec2 center = (c1 + c2) / 2.;
		vec2 p_rot = vec2(p.x - center.x, p.y - center.y); // allows for rotation based on the given angle
		mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
		p_rot = p_rot * rot + center;

		if (p_rot.x <= max(c1.x, c2.x) && p_rot.x >= min(c1.x, c2.x) && 
		p_rot.y <= max(c1.y, c2.y) && p_rot.y >= min(c1.y, c2.y))
		{
			return true;
		}
		return false;
	}

	// To check if a point is inside an ellipse
	bool inEllipse(vec2 p, vec2 center, float rx, float ry) {
		float x = p.x - center.x;
		float y = p.y - center.y;
		if ((x*x)/(rx*rx) + (y*y)/(ry*ry) <= 1) // refers to the equation of an ellipse
		{
			return true;
		}
		return false;
	}

	// To check if a point is inside a sine wave
	bool inSineWave(vec2 p, float amplitude, float frequency, float phase, float shift) {
		float y = amplitude * sin(frequency * (p.x + shift) + phase);
		if (p.y >= y - amplitude && p.y <= y + amplitude)
		{
			return true;
		}
		return false;
	}

	// To convert from Polar Coordinates to Cartesian coordinates
	vec2 polar2cart(float angle, float length) {
		return vec2(cos(angle) * length, sin(angle) * length);
	}

	/////////////////////////////////////////////////////////////////////
	// TODO: replace the code below with your own code                 //
	// Useful variables:											   //
	// iTime: the passed seconds from the start of the program         //
	// iResolution: the size of the window (default: 1280*960)         //
	/////////////////////////////////////////////////////////////////////

	// Return the rgba color of the grid at position (x, y)
	// use this to customize stuff on the screen
	vec4 paintGrid(float x, float y) {
		// ***** SECTION: VARIABLE SETUP *****
		vec2 center = vec2(iResolution / PIXEL_SIZE / 2.); // window center
		vec2 p = vec2(x, y);

		// clouds - ellipses
		vec2 cloud = vec2(iTime * -150., center.y + 50.);  // cloud center
		float cloud_rx = PIXEL_SIZE * 2;
		float cloud_ry = PIXEL_SIZE * 1.5;

		// ocean - sine wave
		float amplitude = 5;
		float frequency = 0.1;
		float phase = 0;
		float shift = iTime * 70.;

		// radii of baseball and basketball (both circles)
		float radius = PIXEL_SIZE * 4;

		// baseball - circle + rectangles for stitching
		vec2 baseball = center - vec2(70., 0); // baseball center
		vec2 c1 = vec2(baseball.x - radius * 0.3, baseball.y - radius * 0.05);
		vec2 c2 = vec2(baseball.x + radius * 0.3, baseball.y + radius * 0.05);

		// basketball - circle + rectangles for stitching
		vec2 basketball = center + vec2(70., 0); // basketball center
		vec2 c3 = vec2(basketball.x - radius * 0.3, basketball.y - radius * 0.03);
		vec2 c4 = vec2(basketball.x + radius * 0.3, basketball.y + radius * 0.03);
		vec2 c5 = vec2(basketball.x - radius * 0.95, basketball.y);
		vec2 c6 = vec2(basketball.x + radius * 0.95, basketball.y - radius * 0.04);
		vec2 c7 = vec2(basketball.x, basketball.y - radius * 0.95);
		vec2 c8 = vec2(basketball.x - radius * 0.05, basketball.y + radius * 0.95);

		// football - ellipse 
		vec2 football = center; // football center
		float football_rx = PIXEL_SIZE * 5; // radius of long end
		float football_ry = PIXEL_SIZE * 3; // radius of short end
		vec2 stripe1 = football + vec2(football_rx / 1.5, 0.);
		vec2 stripe2 = football - vec2(football_rx / 1.5,0.);
		float fb_stripe_rx = football_ry / 10.;
		float fb_stripe_ry = football_rx / 2.2;

		// mountain - triangle
		vec2 mountain_peak = vec2(0, center.y + PIXEL_SIZE * 10.);
    	vec2 mountain_base1 = vec2(0, center.y-PIXEL_SIZE * 20.);
    	vec2 mountain_base2 = vec2(center.x, center.y-PIXEL_SIZE * 20.);
		vec2 snow_base1 = vec2(0, mountain_peak.y * 0.8);
		vec2 snow_base2 = vec2(mountain_base2.x * 0.195, mountain_peak.y*0.8);
		vec2 snow_wave = vec2(snow_base1.x, snow_base1.y);
		float snow_wave_rx = PIXEL_SIZE * 0.95;
		float snow_wave_ry = PIXEL_SIZE * 0.8; 


		// ***** SECTION: PAINTING DISPLAY *****
		// ***** CLOUDS *****
		float cloud_distance = 80.; // distance between each group of clouds
		float cloud_offset = cloud_rx * 10.; // initial offset of the first group of clouds
		// the for loop will create 4 clouds that travel from right to left on the display
		for (int i = 0; i < 10; i++)
		{
			vec2 cloud_pos = vec2(cloud.x + cloud_offset + i * cloud_distance, cloud.y); // position of the current group of clouds
			if (inEllipse(p - vec2(cloud_rx * 10., 0.), cloud_pos, cloud_rx, cloud_ry) ||
				inEllipse(p - vec2(cloud_rx * 11.5, cloud_ry * 0.5), cloud_pos, cloud_rx, cloud_ry) ||
				inEllipse(p - vec2(cloud_rx * 11.5, -cloud_ry * 0.5), cloud_pos, cloud_rx, cloud_ry) ||
				inEllipse(p - vec2(cloud_rx * 13.0, 0.), cloud_pos, cloud_rx, cloud_ry))
			{
				return vec4(0.95, 0.95, 0.95, 1.); // light gray cloud color
			}
		}

		// ***** OCEAN *****
		if (inSineWave(p-vec2(0., 10.), amplitude, frequency, phase, shift))
		{
			return vec4(0., 0., 1., 1.); // color of the sine wave
		}
		// fills in the pixels underneath the sine wave with blue water
		else if (y-10 < amplitude * sin(frequency * (x + shift) + phase))
		{
			return vec4(0., 0., 1., 1.);
		}
		else if (inSineWave(p-vec2(0., 18.), amplitude, frequency, phase, shift)) {
			return vec4(0., 0., 0.8, 1.);
		}
		else if (inSineWave(p-vec2(0., 26.), amplitude, frequency, phase, shift)) {
			return vec4(0., 0., 0.6, 1.);
		}

		// ***** BASEBALL *****
		// red stitching of baseball
		else if (inRectangle(p + vec2(radius / 2, -radius / 2), c1, c2, 0.25 * M_PI) ||
				 inRectangle(p + vec2(radius / 2, radius / 2), c1, c2, -0.25 * M_PI) ||
				 inRectangle(p + vec2(radius / 3.3, 0), c1, c2, 0.5 * M_PI) ||
				 inRectangle(p + vec2(-radius / 2., -radius / 2), c1, c2, -0.25 * M_PI) ||
				 inRectangle(p + vec2(-radius / 2, radius / 2), c1, c2, 0.25 * M_PI) ||
				 inRectangle(p + vec2(-radius / 3.3, 0), c1, c2, 0.5 * M_PI))
		{
			return vec4(1., 0., 0., 1.); // red stitching color
		}
		// white circle of baseball
		else if (inCircle(p, baseball, radius))
		{
			return vec4(1., 1., 1., 1.); // white baseball color
		}

		// ***** BASKETBALL *****
		// black stitching of basketball
		else if (inRectangle(p + vec2(radius / 2, -radius / 2), c3, c4, 0.25 * M_PI) ||
				 inRectangle(p + vec2(radius / 2, radius / 2), c3, c4, -0.25 * M_PI) ||
				 inRectangle(p + vec2(radius / 3.3, 0), c3, c4, 0.5 * M_PI) ||
				 inRectangle(p + vec2(-radius / 2., -radius / 2), c3, c4, -0.25 * M_PI) ||
				 inRectangle(p + vec2(-radius / 2, radius / 2), c3, c4, 0.25 * M_PI) ||
				 inRectangle(p + vec2(-radius / 3.3, 0), c3, c4, 0.5 * M_PI) ||
				 inRectangle(p, c5, c6, M_PI) ||
				 inRectangle(p + vec2(-radius * 0.01, 0), c7, c8, M_PI))
		{
			return vec4(0., 0., 0., 1.); // black stitching color
		}
		// orange circle of basketball
		else if (inCircle(p, basketball, radius))
		{
			return vec4(1., 0.65, 0., 1.); // orange basketball color
		}

		// ***** FOOTBALL *****
		// white center grip of football
		else if (inEllipse(p, football, football_rx / 3., football_ry / 11.)) {
			return vec4(1., 1., 1., 1.);
		}
		// white right stripe of football
		else if (inEllipse(p, stripe1, fb_stripe_rx, fb_stripe_ry)) {
			return vec4(1., 1., 1., 1.);
		}
		// white left stripe of football
		else if (inEllipse(p, stripe2, fb_stripe_rx, fb_stripe_ry)) {
			return vec4(1., 1., 1., 1.);
		}
		// brown ellipse of football
		else if (inEllipse(p, football, football_rx, football_ry))
		{
			return vec4(0.6, 0.4, 0.2, 1.);
		}

		// ***** MOUNTAIN ***** 
		// snow at peak
		else if (inTriangle(p, mountain_peak, snow_base1, snow_base2)) 
		{
			return vec4(1., 1., 1., 1.);
		}
		else if (inEllipse(p, snow_wave, snow_wave_rx, snow_wave_ry) ||
		inEllipse(p, snow_wave + vec2(2*snow_wave_rx, 0.), snow_wave_rx, snow_wave_ry) ||
		inEllipse(p, snow_wave + vec2(4*snow_wave_rx, 0.), snow_wave_rx, snow_wave_ry))
		{
			return vec4(1., 1., 1., 1.);
		}
		// brown mountain at edge of screen
		else if (inTriangle(p, mountain_peak, mountain_base1, mountain_base2))
		{
			return vec4(0.5, 0.3, 0.1, 1.);
		}

		// BACKGROUND COLOR 
		else
		{
			// gradient background
			vec4 bottomColor = vec4(vec3(184, 180, 255) / 255., 1.);
			vec4 topColor = vec4(vec3(184, 220, 235) / 255., 1.);
			float y = (gl_FragCoord.y / iResolution.y); // mix weight - divides fragment location by display height
			vec4 finalColor = mix(topColor, bottomColor, y);
			return finalColor;
		}
	}

	// The function called in the fragment shader
	void mainImage(out vec4 fragColor, in vec2 fragCoord) {
		// To divide the screen into the grids for painting!
		fragColor = paintGrid(floor(fragCoord.x / PIXEL_SIZE), floor(fragCoord.y / PIXEL_SIZE));
	}

);

class A0_Driver : public Driver, public OpenGLViewer
{
	OpenGLScreenCover *screen_cover = nullptr;
	clock_t startTime = clock();

public:
	virtual void Initialize()
	{
		OpenGLViewer::Initialize();
	}

	//// Initialize the screen covering mesh and shaders
	virtual void Initialize_Data()
	{
		OpenGLShaderLibrary::Instance()->Create_Screen_Shader(draw_pixels, "shaderToy");
		screen_cover = Add_Interactive_Object<OpenGLScreenCover>();
		Set_Polygon_Mode(screen_cover, PolygonMode::Fill);
		Uniform_Update();

		screen_cover->Set_Data_Refreshed();
		screen_cover->Initialize();
		screen_cover->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shaderToy"));
	}

	//// Update the uniformed variables used in shader
	void Uniform_Update()
	{
		screen_cover->setResolution((float)Win_Width(), (float)Win_Height());
		screen_cover->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
	}

	//// Go to next frame
	virtual void Toggle_Next_Frame()
	{
		Uniform_Update();
		OpenGLViewer::Toggle_Next_Frame();
	}

	////Keyboard interaction
	virtual void Initialize_Common_Callback_Keys()
	{
		OpenGLViewer::Initialize_Common_Callback_Keys();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc, char *argv[])
{
	if (author == "")
	{
		std::cerr << "***** The author name is not specified. Please put your name in the author string first. *****" << std::endl;
		return 0;
	}
	else
		std::cout << "Assignment 0 demo by " << author << " started" << std::endl;

	A0_Driver driver;
	driver.Initialize();
	driver.Run();
}
