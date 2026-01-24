#pragma once

#include "../world/CT_World.hpp"
#include "../world/Direction.hpp"
#include "VertexFmt.hpp"

#include <citro3d.h>

class Cursor {
public:
	Cursor();
	~Cursor();

	Cursor(const Cursor&) = delete;
	Cursor& operator=(const Cursor&) = delete;

	void Draw(int projUniform, C3D_Mtx* projectionview, World* world,
              mc::Vector3i pos, Direction highlight);

private:
	WorldVertex* m_cursorVBO = nullptr;

	void InitializeVertexBuffer();
};