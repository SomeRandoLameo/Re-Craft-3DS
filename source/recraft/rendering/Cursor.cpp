#include "rendering/Cursor.h"
#include "rendering/VertexFmt.h"
#include "misc/NumberUtils.h"

Cursor::Cursor() {
	InitializeVertexBuffer();
}

Cursor::~Cursor() {
	if (m_cursorVBO) {
		linearFree(m_cursorVBO);
		m_cursorVBO = nullptr;
	}
}

void Cursor::InitializeVertexBuffer() {
	m_cursorVBO = (WorldVertex*)linearAlloc(sizeof(cube_sides_lut));
	memcpy(m_cursorVBO, cube_sides_lut, sizeof(cube_sides_lut));

	// Set all vertices to white
	for (int i = 0; i < 6 * 6; i++) {
		m_cursorVBO[i].rgb[0] = 255;
		m_cursorVBO[i].rgb[1] = 255;
		m_cursorVBO[i].rgb[2] = 255;
	}
}

void Cursor::Draw(int projUniform, C3D_Mtx* projectionview, World* world,
				  int x, int y, int z, Direction highlight) {
	// Create model matrix
	C3D_Mtx model;
	Mtx_Identity(&model);
	Mtx_Translate(&model, x, y, z, true);

	// Calculate MVP matrix
	C3D_Mtx mvp;
	Mtx_Multiply(&mvp, projectionview, &model);

	// Build vertex data for visible faces
	size_t vertices = 0;
	WorldVertex* vtx = m_cursorVBO;

	for (int i = 0; i < 6; i++) {
		const int* offset = DirectionToOffset[i];

		// Only draw faces that are exposed to air
		if (world->GetBlock(mc::Vector3i(x + offset[0], y + offset[1], z + offset[2])) == Block::Air) {
			memcpy(vtx, &cube_sides_lut[i * 6], sizeof(WorldVertex) * 6);

			// Set color - brighter for highlighted face
			uint8_t color = (i == highlight) ? 65 : 32;
			for (int j = 0; j < 6; j++) {
				vtx[j].rgb[0] = color;
				vtx[j].rgb[1] = color;
				vtx[j].rgb[2] = color;
			}

			vtx += 6;
			vertices += 6;
		}
	}

	// Set uniform
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &mvp);

	// Configure alpha blending
	C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_ONE, GPU_ONE,
				   GPU_ZERO, GPU_ONE_MINUS_SRC_ALPHA);

	// Configure depth mapping
	C3D_DepthMap(true, -1.f, 0.001f);

	// Configure texture environment
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	// Set up buffer info
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, m_cursorVBO, sizeof(WorldVertex), 4, 0x3210);

	// Draw
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertices);

	// Restore alpha blending
	C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
				   GPU_ONE_MINUS_SRC_ALPHA, GPU_SRC_ALPHA, GPU_ONE_MINUS_SRC_ALPHA);

	// Restore depth mapping
	C3D_DepthMap(true, -1.f, 0.0f);
}