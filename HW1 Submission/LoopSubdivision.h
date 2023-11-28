#ifndef __LoopSubdivision_h__
#define __LoopSubdivision_h__
#include <unordered_map>
#include <vector>
#include "Mesh.h"

// Function used in step 2 to find opposite-side vertices
inline int find_opp_vtx(Vector3i t0, Vector2i curr_edge) {
	for (int i = 0; i < 3; i++) {
		if (t0[i] != curr_edge[0] && t0[i] != curr_edge[1]) {
			return t0[i];
		}
	}
	return -1;
}

inline void LoopSubdivision(TriangleMesh<3>& mesh)
{
	std::vector<Vector3>& old_vtx=mesh.Vertices();
	std::vector<Vector3i>& old_tri=mesh.Elements();
	std::vector<Vector3> new_vtx;		////vertex array for the new mesh
	std::vector<Vector3i> new_tri;	////element array for the new mesh
	std::vector<Vector3> new_even_vtx; 

	// auxiliary data structures
	std::unordered_map<Vector2i,int> edge_odd_vtx_map; // map from an edge to a midpoint vertex (for odd vertices)
	std::unordered_map<Vector2i,std::vector<int> > edge_tri_map; // map from an edge to incident triangles
	std::unordered_map<int,std::vector<int> > vtx_vtx_map; // map from vertex to incident vertices (for even vertices)
	
	new_vtx=old_vtx;	////copy all the old vertices to the new_vtx array
	
	
	// ***************************************** STEP 1 ***************************************** //
	////step 1: add mid-point vertices and triangles
	////for each old triangle, 
	////add three new vertices (in the middle of each edge) to new_vtx 
	////add four new triangles to new_tri

	/*your implementation here*/

	// *** For each old triangle ***
	for (auto t : old_tri) { 
		Vector3i vtx = t;

		// for each edge
		for (int e=0; e < 3; e++) { 
			Vector2i es = Sorted(Vector2i(vtx[e], vtx[(e+1)%3]));

			if (edge_odd_vtx_map.find(es) != edge_odd_vtx_map.end()) {
				continue; // if current edge already stored
			}

			// *** Add three new vertices (in the middle of each edge) to new_vtx ***
			Vector3 pos = 0.5 * (new_vtx[es[0]] + new_vtx[es[1]]); 
			new_vtx.push_back(pos);
			edge_odd_vtx_map[es] = new_vtx.size()-1;
		}

		// *** Add four new triangles to new_tri ***
		// get the three new vertices (mid-points of edges) in the triangle 
		int p0 = edge_odd_vtx_map[Sorted(Vector2i(vtx[0], vtx[1]))];
		int p1 = edge_odd_vtx_map[Sorted(Vector2i(vtx[1], vtx[2]))];
		int p2 = edge_odd_vtx_map[Sorted(Vector2i(vtx[2], vtx[0]))];

		// add to new_tri
		new_tri.push_back(Vector3i(p0, p1, p2));
		new_tri.push_back(Vector3i(vtx[0], p0, p2)); 
		new_tri.push_back(Vector3i(p0, vtx[1], p1));
		new_tri.push_back(Vector3i(p2, p1, vtx[2]));
	}
	
	// Build/update edge_tri_map
	for (int t=0; t<old_tri.size(); t++) {
		Vector3i vtx = old_tri[t]; 

		// for each edge
		for (int e=0; e < 3; e++) {
			Vector2i es = Sorted(Vector2i(vtx[e], vtx[(e+1)%3]));

			if (edge_tri_map.find(es) == edge_tri_map.end()) {
				edge_tri_map[es] = std::vector<int>(); // setup array for unseen edges
			}
			edge_tri_map[es].push_back(t);
		}
	}


	// ***************************************** STEP 2 ***************************************** //
	////step 2: update the position for each new mid-point vertex: 
	////for each mid-point vertex, find its two end-point vertices A and B, 
	////and find the two opposite-side vertices on the two incident triangles C and D,
	////then update the new position as .375*(A+B)+.125*(C+D)

	/*your implementation here*/

	// *** For each mid-point vertex ***
	for (auto i : edge_odd_vtx_map) {
		Vector2i edge = Sorted(i.first);
		int odd_vtx = i.second;
		
		// *** Find its two end-point vertices A and B ***
		Vector3 A = old_vtx[edge[0]];
		Vector3 B = old_vtx[edge[1]];

		Vector3i t0 = old_tri[edge_tri_map[edge][0]];
		Vector3i t1 = old_tri[edge_tri_map[edge][1]];

		// *** Find the two opposite-side vertices on the two incident triangles C and D ***
		Vector3 C = old_vtx[find_opp_vtx(t0, edge)];
		Vector3 D = old_vtx[find_opp_vtx(t1, edge)];

		// *** Update the new position as .375*(A+B)+.125*(C+D) ***
		new_vtx[odd_vtx] = 0.375 * (A + B) + 0.125 * (C + D);
	}


	// ***************************************** STEP 3 ***************************************** //
	////step 3: update vertices of each old vertex
	////for each old vertex, find its incident old vertices, and update its position according its incident vertices

	/*your implementation here*/

	// Build vtx_vtx_map - copied from tutorial_mesh main.cpp code
	for (int i=0; i<old_tri.size(); i++){
		Vector3i& t = old_tri[i];
		for (int j=0; j<3; j++){
			if (vtx_vtx_map.find(t[j]) == vtx_vtx_map.end()) {
				vtx_vtx_map[t[j]]=std::vector<int>();
			}
			std::vector<int>& nbs=vtx_vtx_map[t[j]];
			for (int k=0; k<3; k++){
				if (t[k] == t[j]) continue;
				if (std::find(nbs.begin(), nbs.end(), t[k]) != nbs.end())continue;
				nbs.push_back(t[k]);
			}
		}
	}

	// copy all vertices into new_even_vtx array to store new positions
	new_even_vtx = new_vtx; 

	// *** For each old vertex ***
	for (auto i : vtx_vtx_map) {
		int vtx = i.first; 
		std::vector<int> nbs = i.second; // neighbor vertices
		float n = i.second.size() * 1.0; // number of neighbor vertices
		
		// determine the beta value based on the number of vertices
		float beta; 
		if (n < 3) {
			beta = 1.0 / 8.0; // for boundary vertex
		}
		else if (n == 3) {
			beta = 3.0 / 16.0;
		}
		else {
			beta = 3.0 / (n * 8.0);
		}

		// weighted average(pos of v and its nbs)
		Vector3 smoothed_pos = (1.0 - (n*beta)) * old_vtx[vtx];

		// *** Find its incident old vertices ***
		for (int v : nbs) {
			smoothed_pos += (beta * old_vtx[v]);
		}

		// *** Update its position according its incident vertices ***
		new_even_vtx[vtx] = smoothed_pos;
	}
	// copy back into new_vtx to synchronize the information
	new_vtx = new_even_vtx;
	
	////update subdivided vertices and triangles onto the input mesh
	old_vtx=new_vtx;
	old_tri=new_tri;
}

#endif