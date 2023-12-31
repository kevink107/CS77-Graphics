//#####################################################################
// Main
// Dartmouth COSC 77/177 Computer Graphics, starter code
// Contact: Bo Zhu (bo.zhu@dartmouth.edu)
//#####################################################################
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "Common.h"
#include "Driver.h"
#include "OpenGLMesh.h"
#include "OpenGLCommon.h"
#include "OpenGLWindow.h"
#include "OpenGLViewer.h"
#include "TinyObjLoader.h"

#ifndef __Main_cpp__
#define __Main_cpp__

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class ShaderDriver : public Driver, public OpenGLViewer
{using Base=Driver;
	std::vector<OpenGLTriangleMesh*> mesh_object_array;						////mesh objects, every object you put in this array will be rendered.
	clock_t startTime;

public:
	virtual void Initialize()
	{
		////For those who are unhappy about the current background color: Here is a secret passage. 
		////Goto OpenGLShaderProgrammcpp Line 239 and 240, change the two colors, and you will get a different background.
		draw_bk=true;						////this flag specifies a customized way to draw the background. If you turn it off, there is no background.
		draw_axes=false;					////if you don't like the axes, turn them off!
		startTime = clock();
		OpenGLViewer::Initialize();
	}

	////This function adds a mesh object from an obj file
	int Add_Obj_Mesh_Object(std::string obj_file_name)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Array<std::shared_ptr<TriangleMesh<3> > > meshes;
		Obj::Read_From_Obj_File(obj_file_name,meshes);
		mesh_obj->mesh=*meshes[0];
		std::cout<<"load tri_mesh from obj file, #vtx: "<<mesh_obj->mesh.Vertices().size()<<", #ele: "<<mesh_obj->mesh.Elements().size()<<std::endl;		

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a sphere mesh onto the screen
	int Add_Sphere_Object(const double radius=1.)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();

		Initialize_Sphere_Mesh(radius,&mesh_obj->mesh,3);		////add a sphere with radius=1. if the obj file name is not specified

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function adds a triangle (the in-class demo)
	int Add_Triangle_Object(const std::vector<Vector3>& vertices)
	{
		auto mesh_obj=Add_Interactive_Object<OpenGLTriangleMesh>();
		auto& mesh=mesh_obj->mesh;

		////manually initialize the vertices and elements for a triangle mesh
		mesh.Vertices().resize(3);
		for(int i=0;i<vertices.size();i++)mesh.Vertices()[i]=vertices[i];
		mesh.Elements().resize(1);mesh.Elements()[0]=Vector3i(0,1,2);

		mesh_object_array.push_back(mesh_obj);
		return (int)mesh_object_array.size()-1;
	}

	////This function demonstrates how to manipulate the vertex array of a mesh on the CPU end.
	////The updated vertices will be sent to GPU for rendering automatically.
	void Translate_Vertex_Position_For_Mesh_Object(OpenGLTriangleMesh* obj,const Vector3& translate)
	{
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		
		for(auto& v:vertices){
			v+=translate;
		}
	}

	////This function demonstrates how to manipulate the color and normal arrays of a mesh on the CPU end.
	////The updated colors and normals will be sent to GPU for rendering automatically.
	void Update_Vertex_Color_And_Normal_For_Mesh_Object(OpenGLTriangleMesh* obj)
	{
		int vn=(int)obj->mesh.Vertices().size();					////number of vertices of a mesh
		std::vector<Vector3>& vertices=obj->mesh.Vertices();		////you might find this array useful
		std::vector<Vector3i>& elements=obj->mesh.Elements();		////you might find this array also useful

		std::vector<Vector4f>& vtx_color=obj->vtx_color;
		vtx_color.resize(vn);
		std::fill(vtx_color.begin(),vtx_color.end(),Vector4f::Zero());

		////TODO [Step 0]: update the color for each vertex.
		////NOTICE: This code updates the vertex color array on the CPU end. The array will then be sent to GPU and read it the vertex shader as v_color.
		////You don't need to implement the CPU-GPU data transfer code.

		// made the color white for the advanced lighting effect so it reflects the light from the light source
		for(int i=0;i<vn;i++){
			vtx_color[i]=Vector4f(1.,1.,1.,1.);	////specify color for each vertex (*changes color of initial sphere in starter code)
		}

		// [Step 0]: Uncomment the 3 for loops below for one of the implementations I made for Step 0 in helloworld vtx and frag shaders
		// Warning: Do not use these for loops for the other steps!
		// for(int i=0;i<vn/3;i++){
		// 	vtx_color[i]=Vector4f(0.,0.,1.,1.);	////specify color for each vertex (*changes color of initial sphere in starter code)
		// }
		// for(int i=vn/3;i<2*vn/3;i++){
		// 	vtx_color[i]=Vector4f(0.,1.,0.,1.);	////specify color for each vertex (*changes color of initial sphere in starter code)
		// }
		// for(int i=2*vn/3;i<vn;i++){
		// 	vtx_color[i]=Vector4f(1.,0.,0.,1.);	////specify color for each vertex (*changes color of initial sphere in starter code)
		// }

		std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		vtx_normal.resize(vn);
		std::fill(vtx_normal.begin(),vtx_normal.end(),Vector3::Zero());

		//TODO: update the normal for each vertex
		//NOTICE: This code updates the vertex normal array on the CPU end. The array will then be sent to GPU and read it the vertex shader as normal.
		//This is a default implementation of vertex normal that works for a sphere centered around the origin only.
		//comment out the below for-loop to check if step 1 code works - should do the same thing
		// for(int i=0;i<vn;i++){
		// 	vtx_normal[i]=Vector3(vertices[i][0],vertices[i][1],vertices[i][2]); // *specifically for the sphere? - set based on the location (Vector3)
		// 	// *need to implement a more general calculation - so not just specified for a sphere
		// }	

		////TODO [Step 1]: Comment the default implementation and uncomment the following function and implement it to calculate mesh normals.
		Update_Vertex_Normal(vertices,elements,vtx_normal);
	}

	////TODO [Step 1]: implement your function to update vertex normals
	void Update_Vertex_Normal(const std::vector<Vector3>& vertices,const std::vector<Vector3i>& elements,std::vector<Vector3>& normals)
	{
		////TODO [Step 1]: your implementation to calculate the normal vector for each mesh vertex
		// *NOTES:
		// 'elements' is the triangles array from last time, 'vertices' and 'elements' are input and 'normals' is output
		// for each vtx, find its incident triangles, get normals of each, update vertex normal as avg of incident triangle normals
		// phong: show normal as color on screen
		
		
		// iterate through each triangle
		for (const auto& tri: elements)
		{
			// get positions of its three vertices
			Vector3 p1 = vertices[tri[0]];
			Vector3 p2 = vertices[tri[1]];
			Vector3 p3 = vertices[tri[2]];

			// get edges 
			Vector3 e1 = p1-p2;
			Vector3 e2 = p1-p3;
			
			// calculate the normal of the triangle using the cross product
			Vector3 nml = Vector3(e1[1]*e2[2]-e1[2]*e2[1], e1[2]*e2[0]-e1[0]*e2[2], e1[0]*e2[1]-e1[1]*e2[0]);

			// add the normal vector to the incident vertices
			normals[tri[0]] += nml;
			normals[tri[1]] += nml;
			normals[tri[2]] += nml;
		}
		// iterate through each vertex normal
		// for (int i=0; i<normals.size(); i++)
		// {
		// 	// normalize the normal vertices
		// 	normals[i] = normals[i].normalized();
		// }
		for (auto& n : normals) {
			n = n.normalized();
		}
	}

	virtual void Initialize_Data()
	{
		////Add a sphere mesh - uncomment for sphere
		// {
		// 	int obj_idx=Add_Sphere_Object();
		// 	auto obj=mesh_object_array[obj_idx];
		// 	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		// }

		////Add an obj mesh
		////TODO [Step 4]: uncomment this part and use your own mesh for Step 4.

		// // bunny mesh
		// {
		// 	int obj_idx=Add_Obj_Mesh_Object("bunny.obj");
		// 	auto obj=mesh_object_array[obj_idx];
		// 	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		// }

		// dog mesh
		{
			int obj_idx=Add_Obj_Mesh_Object("dog3.obj");
			auto obj=mesh_object_array[obj_idx];
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		}

		// soccer ball mesh
		{
			int obj_idx=Add_Obj_Mesh_Object("futbol.obj");
			auto obj=mesh_object_array[obj_idx];
			Translate_Vertex_Position_For_Mesh_Object(obj,Vector3(30, 5, 0));
			Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);	
		}

		

		// ////If you want to put multiple objects in the scene, uncomment this block. It will add another sphere mesh in the scene.
		// {
		// 	int obj_idx=Add_Sphere_Object();	////add a sphere
		// 	auto obj=mesh_object_array[obj_idx];
		// 	//Translate_Vertex_Position_For_Mesh_Object(obj,Vector3::Unit(0)*3.);
		// 	Translate_Vertex_Position_For_Mesh_Object(obj,Vector3(-2, 1, 2));
		// 	Update_Vertex_Color_And_Normal_For_Mesh_Object(obj);		
		// }

		// ////Add a manually built triangle mesh (with a single triangle). This is the demo code I showed in class.
		// ////You don't need this part for your homework. Just put them here for your reference.
		// {
		// 	std::vector<Vector3> triangle_vertices={Vector3(0,0,0),Vector3(1,0,0),Vector3(1,1,0)};
		// 	int obj_idx=Add_Triangle_Object(triangle_vertices);	////add a sphere
		// 	auto obj=mesh_object_array[obj_idx];
			
		// 	////specify the vertex colors on the CPU end
		// 	std::vector<Vector4f>& vtx_color=obj->vtx_color;
		// 	vtx_color={Vector4f(1.f,0.f,0.f,1.f),Vector4f(0.f,1.f,0.f,1.f),Vector4f(0.f,0.f,1.f,1.f)};

		// 	std::vector<Vector3>& vtx_normal=obj->vtx_normal;
		// 	vtx_normal={Vector3(0.,0.,1.),Vector3(0.,0.,1.),Vector3(0.,0.,1.)};
		// }

		////initialize shader
		////TODO [Step 2,3,4]: switch the shaders by changing the file names here. We use the helloworld shader by default. 
		////You need to switch them to my_lambertian and my_phong for step 2,3, and 4. 
		// *can use time in your shaders to make it more advanced

		// std::string vertex_shader_file_name="helloworld.vert";
		// std::string fragment_shader_file_name="helloworld.frag";

		// std::string vertex_shader_file_name="my_lambertian.vert";
		// std::string fragment_shader_file_name="my_lambertian.frag";

		std::string vertex_shader_file_name="my_phong.vert";
		std::string fragment_shader_file_name="my_phong.frag";

		OpenGLShaderLibrary::Instance()->Add_Shader_From_File(vertex_shader_file_name,fragment_shader_file_name,"a2_shader");

		////bind the shader with each mesh object in the object array
		for(auto& mesh_obj: mesh_object_array){
			mesh_obj->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("a2_shader"));
			Set_Polygon_Mode(mesh_obj,PolygonMode::Fill);
			Set_Shading_Mode(mesh_obj,ShadingMode::A2);
			mesh_obj->Set_Data_Refreshed();
			mesh_obj->Initialize();	
		}
	}

	//// Go to next frame 
	virtual void Toggle_Next_Frame()
	{
		for (auto& mesh_obj : mesh_object_array) {
			mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
		}
		OpenGLViewer::Toggle_Next_Frame();
	}

	virtual void Run()
	{
		OpenGLViewer::Run();
	}
};

int main(int argc,char* argv[])
{
	ShaderDriver driver;
	driver.Initialize();
	driver.Run();	
}

#endif