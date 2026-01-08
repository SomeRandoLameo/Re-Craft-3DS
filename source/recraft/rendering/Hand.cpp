#include "rendering/Hand.h"

int16_t toTexCoord(int x, int tw) {
    return (int16_t)(((float)(x) / (float)(tw)) * (float)(1 << 15));
}

Hand::Hand() {
	m_handVBO = (WorldVertex*)linearAlloc(sizeof(cube_sides_lut));
	
	Texture_Load(&m_SkinTexture, "romfs:/assets/textures/entity/player.png");
}
Hand::~Hand() {
	linearFree(m_handVBO);
	C3D_TexDelete(&m_SkinTexture);
}

void Hand::Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player) {
	C3D_Mtx pm;
	C3D_Mtx model;
	Mtx_Identity(&model);
	Mtx_Translate(&model, 0.5f + sinf(player->bobbing) * 0.03f + (stack.GetItemCount() == 0) * 0.2f,
		      -0.68f + ABS(sinf(player->bobbing)) * 0.01f, -1.2f - (stack.GetItemCount() == 0) * 0.4f, true);
	Mtx_RotateX(&model, M_PI / 18.f + (stack.GetItemCount() == 0) * M_PI / 5.f, true);
	Mtx_RotateY(&model, M_PI / 18.f, true);
	if (player->breakPlaceTimeout > -0.1f) {
		float dist = sinf((player->breakPlaceTimeout + 0.1f) / (PLAYER_PLACE_REPLACE_TIMEOUT + 0.1f) * M_PI);
		float h = cosf((player->breakPlaceTimeout + 0.1f) / (PLAYER_PLACE_REPLACE_TIMEOUT + 0.1f) * M_PI);
		Mtx_RotateX(&model, -dist, true);
		Mtx_Translate(&model, 0.f, -h * 0.3f, -dist * 0.25f, true);
	}
	if (stack.GetItemCount() == 0) Mtx_RotateZ(&model, DEG_TO_RAD * 40.f, true);
	Mtx_Scale(&model, 0.28f, 0.28f, stack.GetItemCount() == 0 ? 0.8f : 0.28f);

	Mtx_Multiply(&pm, projection, &model);

	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &pm);

	memcpy(m_handVBO, cube_sides_lut, sizeof(cube_sides_lut));
	for (int i = 0; i < 6; i++) {
		if (stack.GetItemCount() > 0) {
			int16_t iconUV[2];
			uint8_t color[3];
			Block_GetTexture(stack, (Direction)i, iconUV);
			Block_GetColor(static_cast<Block>(stack.GetItemId()), stack.GetItemDamage(), (Direction)i, color);

#define oneDivIconsPerRow (32768 / 8)
#define halfTexel (6)

			for (int j = 0; j < 6; j++) {
				int idx = i * 6 + j;

				m_handVBO[idx].uv[0] = (m_handVBO[idx].uv[0] == 1 ? (oneDivIconsPerRow - 1) : 1) + iconUV[0];
                m_handVBO[idx].uv[1] = (m_handVBO[idx].uv[1] == 1 ? (oneDivIconsPerRow - 1) : 1) + iconUV[1];

                m_handVBO[idx].rgb[0] = color[0];
				m_handVBO[idx].rgb[1] = color[1];
				m_handVBO[idx].rgb[2] = color[2];
			}
		} else {
			C3D_TexBind(0, &m_SkinTexture);

			if (i == Direction::East ||
			    i == Direction::West) {  // eines der dümmsten Dinge, die ich jemals in meinem Leben getan habe
				const int16_t uvRotationTable[2][2][2][2] = {
				    {{{0, 1}, {0, 0}}, {{1, 1}, {1, 0}}}, {{{1, 0}, {1, 1}}, {{0, 0}, {0, 1}}},
				};
				for (int j = 0; j < 6; j++) {
					int idx = i * 6 + j;
					int u = m_handVBO[idx].uv[0];
					int v = m_handVBO[idx].uv[1];
					m_handVBO[idx].uv[0] = uvRotationTable[(i == Direction::West) ? 1 : 0][v][u][0];
					m_handVBO[idx].uv[1] = uvRotationTable[(i == Direction::East) ? 1 : 0][v][u][1];
				}
			}
			for (int j = 0; j < 6; j++) {
				int idx = i * 6 + j;

				const int16_t uvLookUp[6][4] = {
				    {toTexCoord(48, 64), toTexCoord(52, 64), toTexCoord(20, 64), toTexCoord(32, 64)},  // west = inside
				    {toTexCoord(40, 64), toTexCoord(44, 64), toTexCoord(20, 64), toTexCoord(32, 64)},  // east = outside
				    {toTexCoord(52, 64), toTexCoord(56, 64), toTexCoord(20, 64), toTexCoord(32, 64)},  // bottom = back
				    {toTexCoord(44, 64), toTexCoord(48, 64), toTexCoord(20, 64), toTexCoord(32, 64)},  // top = front
				    {toTexCoord(48, 64), toTexCoord(52, 64), toTexCoord(16, 64), toTexCoord(20, 64)},  // south = bottom
				    {toTexCoord(44, 64), toTexCoord(48, 64), toTexCoord(16, 64), toTexCoord(20, 64)},  // north = top
				};

				m_handVBO[idx].uv[0] = uvLookUp[i][m_handVBO[idx].uv[0]];
				m_handVBO[idx].uv[1] = uvLookUp[i][m_handVBO[idx].uv[1] + 2];
			}
		}
	}

	C3D_AlphaTest(true, GPU_GEQUAL, 255);

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, m_handVBO, sizeof(WorldVertex), 4, 0x3210);

	C3D_DrawArrays(GPU_TRIANGLES, 0, 6 * 6);

	C3D_AlphaTest(false, GPU_GREATER, 0);
}