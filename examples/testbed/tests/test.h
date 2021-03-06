/*
* Copyright (c) 2016-2016 Irlan Robson http://www.irlan.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef TEST_H
#define TEST_H

#include <glfw/glfw3.h>
#include <imgui/imgui.h>

#include <bounce/bounce.h>

#include <testbed/framework/debug_draw.h>
#include <testbed/framework/profiler.h>

inline float32 RandomFloat(float32 a, float32 b)
{
	float32 x = float32(rand()) / float32(RAND_MAX);
	float32 diff = b - a;
	float32 r = x * diff;
	return a + r;
}

struct Settings
{
	Settings()
	{
		hertz = 60.0f;
		velocityIterations = 8;
		positionIterations = 2;
		sleep = false;
		warmStart = true;
		convexCache = true;
		drawCenterOfMasses = false;
		drawVerticesEdges = true;
		drawFaces = true;
		drawBounds = false;
		drawJoints = true;
		drawContactPoints = true;
		drawContactNormals = false;
		drawContactTangents = false;
		drawContactAreas = false;
		drawStats = false;
		drawProfile = false;
		drawGrid = true;
		pause = false;
		singleStep = false;
		lastTestID = -1;
		testID = 0;
	}

	int lastTestID;
	int testID;
	bool pause;
	bool singleStep;

	float32 hertz;
	int velocityIterations;
	int positionIterations;
	bool sleep;
	bool warmStart;
	bool convexCache;
	
	bool drawCenterOfMasses;
	bool drawBounds;
	bool drawVerticesEdges;
	bool drawFaces;
	bool drawSolidShapes;
	bool drawJoints;
	bool drawContactPoints;
	bool drawContactNormals;
	bool drawContactTangents;
	bool drawContactAreas;
	bool drawStats;
	bool drawProfile;
	bool drawGrid;
};

class Test;

struct TestEntry
{
	typedef Test* (*TestCreate)();
	const char* name;
	TestCreate create;
};

extern TestEntry g_tests[];

class RayCastListener : public b3RayCastListener
{
public:
	float32 ReportShape(b3Shape* shape, const b3Vec3& point, const b3Vec3& normal, float32 fraction)
	{
		hit.shape = shape;
		hit.point = point;
		hit.normal = normal;
		hit.fraction = fraction;
		return 1.0f;
	}

	b3RayCastSingleOutput hit;
};

class Test : public b3ContactListener
{
public:
	enum Meshes
	{
		e_gridMesh,
		e_terrainMesh,
		e_clothMesh,
		e_maxMeshes,
	};

	Test();
	virtual ~Test();

	virtual void BeginContact(b3Contact* contact);
	virtual void EndContact(b3Contact* contact);
	virtual void PreSolve(b3Contact* contact);

	virtual void Step();
	virtual void RayHit();

	virtual void MouseMove(const Ray3& pw);
	virtual void MouseLeftDown(const Ray3& pw);
	virtual void MouseLeftUp(const Ray3& pw);
	virtual void KeyDown(int button) { }
	virtual void KeyUp(int button) { }

	virtual void Dump() { }

	b3World m_world;
	b3RayCastSingleOutput m_rayHit; // ray hit local space
	b3MouseJoint* m_mouseJoint;

	b3BoxHull m_groundHull;
	b3BoxHull m_boxHull;	
	b3Mesh m_meshes[e_maxMeshes];
};

#endif
