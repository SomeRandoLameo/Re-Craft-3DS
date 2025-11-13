// This Backend is heavily based on mtheall's ftpd citro3d and ctr
// implementation Link: https://github.com/mtheall/ftpd/blob/master/source/3ds/
// Shader
#include "imgui_impl_citro3d.h"
#include <3ds.h>
#include <citro3d.h>

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "imgui_impl_c3d_shbin.h"

#ifdef IM_IMPL_C3D_NPI_ASSERT
#define NPI_ASSERT(expr)                                                  \
  if (!(expr)) {                                                          \
    std::ofstream logFile("sdmc:/assert_errors.txt", std::ios_base::app); \
    logFile << "Assertion failed in " << __FILE__ << " line " << __LINE__ \
            << ": " << #expr << std::endl;                                \
    logFile.close();                                                      \
    abort();                                                              \
  }
#else
#define NPI_ASSERT(expr) IM_ASSERT(expr)
#endif

struct ImGui_ImplCitro3D_Backend_Data {
  /* Data */
  // Shader
  DVLB_s *shader = nullptr;
  shaderProgram_s shader_program;
  C3D_AttrInfo attr_info;

  // Matrix
  int uLoc_projection;
  C3D_Mtx mtx_projection_top;
  C3D_Mtx mtx_projection_bot;

  // System Font and TextScale
  std::vector<C3D_Tex> FontTextures;
  float text_scale;
  // 3DS font glyph ranges
  std::vector<ImWchar> FontRanges;

  // Data stuff
  unsigned int boundScissor[4];
  ImDrawVert *boundVertexData;
  C3D_Tex *boundTexture;

  // Render Data
  ImDrawVert *VertexData = nullptr;
  std::size_t VertexSize = 0;
  ImDrawIdx *IndexData = nullptr;
  std::size_t IndexSize = 0;
};

static ImGui_ImplCitro3D_Backend_Data *ImGui_ImplCitro3D_GetBackendData() {
  return ImGui::GetCurrentContext()
             ? (ImGui_ImplCitro3D_Backend_Data *)ImGui::GetIO()
                   .BackendRendererUserData
             : nullptr;
}

// FastAccess
/**
 * bknd_data->
 * auto bknd_data = ImGui_ImplCitro3D_GetBackendData();
 */

// Helper Functions

///@brief Get code point from glyph index
///@param font Font to search
///@param glyphIndex Glyph index
unsigned int fontCodePointFromGlyphIndex(CFNT_s *const font,
                                         int const glyphIndex) {
  for (auto cmap = fontGetInfo(font)->cmap; cmap; cmap = cmap->next) {
    switch (cmap->mappingMethod) {
      case CMAP_TYPE_DIRECT:
        NPI_ASSERT(cmap->codeEnd >= cmap->codeBegin);
        if (glyphIndex >= cmap->indexOffset &&
            glyphIndex <= cmap->codeEnd - cmap->codeBegin + cmap->indexOffset)
          return glyphIndex - cmap->indexOffset + cmap->codeBegin;
        break;

      case CMAP_TYPE_TABLE:
        for (int i = 0; i <= cmap->codeEnd - cmap->codeBegin; ++i) {
          if (cmap->indexTable[i] == glyphIndex) return cmap->codeBegin + i;
        }
        break;

      case CMAP_TYPE_SCAN:
        for (unsigned i = 0; i < cmap->nScanEntries; ++i) {
          NPI_ASSERT(cmap->scanEntries[i].code >= cmap->codeBegin);
          NPI_ASSERT(cmap->scanEntries[i].code <= cmap->codeEnd);
          if (glyphIndex == cmap->scanEntries[i].glyphIndex)
            return cmap->scanEntries[i].code;
        }
        break;
    }
  }

  return 0;
}

void SetupRendererForScreen(const gfxScreen_t screen) {
  auto bknd_data = ImGui_ImplCitro3D_GetBackendData();
  for (int i = 0; i < 4; i++) {
    bknd_data->boundScissor[i] = 0xFFFFFFFF;
  }
  bknd_data->boundVertexData = nullptr;
  bknd_data->boundTexture = nullptr;

  // Bind Shader
  C3D_BindProgram(&bknd_data->shader_program);
  C3D_SetAttrInfo(&bknd_data->attr_info);

  C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_COLOR);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA);

  if (screen == GFX_TOP)
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, bknd_data->uLoc_projection,
                     &bknd_data->mtx_projection_top);
  else
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, bknd_data->uLoc_projection,
                     &bknd_data->mtx_projection_bot);

  C3D_CullFace(GPU_CULL_NONE);
}

IMGUI_IMPL_API bool ImGui_ImplCitro3D_Init(bool load_sysfont) {
  auto &io = ImGui::GetIO();

  NPI_ASSERT(io.BackendRendererUserData == nullptr &&
             "Already initialized a renderer backend!");

  // Setup backend capabilities flags
  ImGui_ImplCitro3D_Backend_Data *bd = IM_NEW(ImGui_ImplCitro3D_Backend_Data)();
  io.BackendRendererUserData = reinterpret_cast<void *>(bd);

  io.BackendRendererName = "imgui_impl_citro3d";
  io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

  auto bknd_data = ImGui_ImplCitro3D_GetBackendData();

  bknd_data->shader = DVLB_ParseFile(
      const_cast<u32 *>(reinterpret_cast<const u32 *>(imgui_impl_c3d_shbin)),
      imgui_impl_c3d_shbin_size);

  shaderProgramInit(&bknd_data->shader_program);
  shaderProgramSetVsh(&bknd_data->shader_program, &bknd_data->shader->DVLE[0]);

  bknd_data->uLoc_projection = shaderInstanceGetUniformLocation(
      bknd_data->shader_program.vertexShader, "projection");

  AttrInfo_Init(&bd->attr_info);
  AttrInfo_AddLoader(&bd->attr_info, 0, GPU_FLOAT, 2);          // inPosition aka v0
  AttrInfo_AddLoader(&bd->attr_info, 1, GPU_FLOAT, 2);          // inTexcoord aka v1
  AttrInfo_AddLoader(&bd->attr_info, 2, GPU_UNSIGNED_BYTE, 4);  // inColor aka v2

  // linear alloc Vertex Data
  bknd_data->VertexSize = 65536;
  bknd_data->VertexData = reinterpret_cast<ImDrawVert *>(
      linearAlloc(sizeof(ImDrawVert) * bknd_data->VertexSize));
  NPI_ASSERT(bknd_data->VertexData);

  // linear alloc Index Data
  bknd_data->IndexSize = 65536;
  bknd_data->IndexData = reinterpret_cast<ImDrawIdx *>(
      linearAlloc(sizeof(ImDrawIdx) * bknd_data->IndexSize));
  NPI_ASSERT(bknd_data->IndexData);

  // Load System Font
  if (R_FAILED(fontEnsureMapped()))
    NPI_ASSERT(false && "Shared System Font is not Mapped");

  const auto font = fontGetSystemFont();
  const auto fontInfo = fontGetInfo(font);
  const auto glyphInfo = fontGetGlyphInfo(font);
  NPI_ASSERT(bknd_data->FontTextures.empty());
  bknd_data->FontTextures.resize(glyphInfo->nSheets + 1);
  memset(bknd_data->FontTextures.data(), 0x00,
         bknd_data->FontTextures.size() * sizeof(bknd_data->FontTextures[0]));
  bknd_data->text_scale = 30.0f / glyphInfo->cellHeight;

  for (unsigned i = 0; i < glyphInfo->nSheets; i++) {
    auto &tex = bknd_data->FontTextures[i];
    tex.data = fontGetGlyphSheetTex(font, i);
    NPI_ASSERT(tex.data);

    tex.fmt = static_cast<GPU_TEXCOLOR>(glyphInfo->sheetFmt);
    tex.size = glyphInfo->sheetSize;
    tex.width = glyphInfo->sheetWidth;
    tex.height = glyphInfo->sheetHeight;
    tex.param = GPU_TEXTURE_MAG_FILTER(GPU_LINEAR) |
                GPU_TEXTURE_MIN_FILTER(GPU_LINEAR) |
                GPU_TEXTURE_WRAP_S(GPU_REPEAT) | GPU_TEXTURE_WRAP_T(GPU_REPEAT);
    tex.border = 0xFFFFFFFF;
    tex.lodParam = 0;
  }
  // Generate ImGui's white pixel texture
  auto &tex = bknd_data->FontTextures[glyphInfo->nSheets];
  C3D_TexInit(&tex, 8, 8, GPU_A4);

  uint32_t size;
  auto data = C3D_Tex2DGetImagePtr(&tex, 0, &size);
  NPI_ASSERT(data);
  NPI_ASSERT(size);
  memset(data, 0xFF, size);

  ImWchar alterChar =
      fontCodePointFromGlyphIndex(font, fontInfo->alterCharIndex);
  if (!alterChar) alterChar = '?';

  std::vector<ImWchar> charSet;
  for (auto cmap = fontInfo->cmap; cmap; cmap = cmap->next) {
    switch (cmap->mappingMethod) {
      case CMAP_TYPE_DIRECT:
        NPI_ASSERT(cmap->codeEnd >= cmap->codeBegin);
        charSet.reserve(charSet.size() + cmap->codeEnd - cmap->codeBegin + 1);
        for (auto i = cmap->codeBegin; i <= cmap->codeEnd; ++i) {
          if (cmap->indexOffset + (i - cmap->codeBegin) == 0xFFFF) break;

          charSet.emplace_back(i);
        }
        break;

      case CMAP_TYPE_TABLE:
        NPI_ASSERT(cmap->codeEnd >= cmap->codeBegin);
        charSet.reserve(charSet.size() + cmap->codeEnd - cmap->codeBegin + 1);
        for (auto i = cmap->codeBegin; i <= cmap->codeEnd; ++i) {
          if (cmap->indexTable[i - cmap->codeBegin] == 0xFFFF) continue;

          charSet.emplace_back(i);
        }
        break;

      case CMAP_TYPE_SCAN:
        charSet.reserve(charSet.size() + cmap->nScanEntries);
        for (unsigned i = 0; i < cmap->nScanEntries; ++i) {
          NPI_ASSERT(cmap->scanEntries[i].code >= cmap->codeBegin);
          NPI_ASSERT(cmap->scanEntries[i].code <= cmap->codeEnd);

          if (cmap->scanEntries[i].glyphIndex == 0xFFFF) continue;

          charSet.emplace_back(cmap->scanEntries[i].code);
        }
        break;
    }
  }

  NPI_ASSERT(!charSet.empty());

  // Duplicate Char Map
  std::sort(std::begin(charSet), std::end(charSet));
  charSet.erase(std::unique(std::begin(charSet), std::end(charSet)),
                std::end(charSet));

  auto it = std::begin(charSet);
  ImWchar start = *it++;
  ImWchar prev = start;
  while (it != std::end(charSet)) {
    if (*it != prev + 1) {
      bknd_data->FontRanges.emplace_back(start);
      bknd_data->FontRanges.emplace_back(prev);

      start = *it;
    }

    prev = *it++;
  }
  bknd_data->FontRanges.emplace_back(start);
  bknd_data->FontRanges.emplace_back(prev);

  // terminate glyph ranges
  bknd_data->FontRanges.emplace_back(0);

  // initialize font atlas
  auto const atlas = ImGui::GetIO().Fonts;
  atlas->Clear();
  atlas->TexWidth = glyphInfo->sheetWidth;
  atlas->TexHeight = glyphInfo->sheetHeight * glyphInfo->nSheets;
  atlas->TexUvScale = ImVec2(1.0f / atlas->TexWidth, 1.0f / atlas->TexHeight);
  atlas->TexUvWhitePixel =
      ImVec2(0.5f * 0.125f, glyphInfo->nSheets + 0.5f * 0.125f);
  atlas->TexPixelsAlpha8 =
      static_cast<unsigned char *>(IM_ALLOC(1));  // dummy allocation

  // Setup Font Config
  ImFontConfig config;
  config.FontData = nullptr;
  config.FontDataSize = 0;
  config.FontDataOwnedByAtlas = true;
  config.FontNo = 0;
  config.SizePixels = 14.0f;
  config.OversampleH = 3;
  config.OversampleV = 1;
  config.PixelSnapH = false;
  config.GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
  config.GlyphOffset = ImVec2(0.0f, fontInfo->ascent);
  config.GlyphRanges = bknd_data->FontRanges.data();
  config.GlyphMinAdvanceX = 0.0f;
  config.GlyphMaxAdvanceX = std::numeric_limits<float>::max();
  config.MergeMode = false;
  config.FontBuilderFlags = 0;
  config.RasterizerMultiply = 1.0f;
  config.EllipsisChar = 0x2026;
  memset(config.Name, 0, sizeof(config.Name));

  // create font
  auto const imFont = IM_NEW(ImFont);
  config.DstFont = imFont;

  // add config and font to atlas
  atlas->ConfigData.push_back(config);
  atlas->Fonts.push_back(imFont);
  atlas->SetTexID(bknd_data->FontTextures.data());

  // initialize font
  imFont->FallbackAdvanceX = fontInfo->defaultWidth.charWidth;
  imFont->FontSize = fontInfo->lineFeed;
  imFont->ContainerAtlas = atlas;
  imFont->ConfigData = &atlas->ConfigData[0];
  imFont->ConfigDataCount = 1;
  imFont->FallbackChar = alterChar;
  imFont->EllipsisChar = config.EllipsisChar;
  imFont->Scale = bknd_data->text_scale * 0.5f;
  imFont->Ascent = fontInfo->ascent;
  imFont->Descent = 0.0f;

  fontGlyphPos_s glyphPos;
  for (auto const &code : charSet) {
    auto const glyphIndex = fontGlyphIndexFromCodePoint(font, code);
    NPI_ASSERT(glyphIndex >= 0);
    NPI_ASSERT(glyphIndex < 0xFFFF);

    fontCalcGlyphPos(&glyphPos, font, glyphIndex,
                     GLYPH_POS_CALC_VTXCOORD | GLYPH_POS_AT_BASELINE, 1.0f,
                     1.0f);

    NPI_ASSERT(glyphPos.sheetIndex >= 0);
    NPI_ASSERT(static_cast<std::size_t>(glyphPos.sheetIndex) <
               bknd_data->FontRanges.size());

    // Add Glypth to Font
    imFont->AddGlyph(
        &config, code, glyphPos.vtxcoord.left,
        glyphPos.vtxcoord.top + fontInfo->ascent, glyphPos.vtxcoord.right,
        glyphPos.vtxcoord.bottom + fontInfo->ascent, glyphPos.texcoord.left,
        glyphPos.sheetIndex + glyphPos.texcoord.top, glyphPos.texcoord.right,
        glyphPos.sheetIndex + glyphPos.texcoord.bottom, glyphPos.xAdvance);
  }

  imFont->BuildLookupTable();
  atlas->TexReady = true;
  return true;
}

IMGUI_IMPL_API void ImGui_ImplCitro3D_Shutdown() {
  auto bknd_data = ImGui_ImplCitro3D_GetBackendData();
  linearFree(bknd_data->VertexData);
  linearFree(bknd_data->IndexData);

  shaderProgramFree(&bknd_data->shader_program);
  DVLB_Free(bknd_data->shader);
}

IMGUI_IMPL_API void ImGui_ImplCitro3D_NewFrame() {
  ImGui_ImplCitro3D_Backend_Data *bd = ImGui_ImplCitro3D_GetBackendData();
  NPI_ASSERT(bd != nullptr && "Did you call ImGui_ImplCitro3D_Init()?");
}

IMGUI_IMPL_API void ImGui_ImplCitro3D_RenderDrawData(ImDrawData *draw_data,
                                                     void *t_top, void *t_bot) {
  if (draw_data->CmdListsCount <= 0) return;

  auto bknd_data = ImGui_ImplCitro3D_GetBackendData();

  unsigned width = draw_data->DisplaySize.x * draw_data->FramebufferScale.x;
  unsigned height = draw_data->DisplaySize.y * draw_data->FramebufferScale.y;
  if (width <= 0 || height <= 0) return;

  Mtx_OrthoTilt(&bknd_data->mtx_projection_top, 0.0f, draw_data->DisplaySize.x,
                draw_data->DisplaySize.y * 0.5f, 0.0f, -1.0f, 1.0f, false);
  Mtx_OrthoTilt(&bknd_data->mtx_projection_bot, draw_data->DisplaySize.x * 0.1f,
                draw_data->DisplaySize.x * 0.9f, draw_data->DisplaySize.y,
                draw_data->DisplaySize.y * 0.5f, -1.0f, 1.0f, false);

  if (bknd_data->VertexSize < static_cast<size_t>(draw_data->TotalVtxCount)) {
    linearFree(bknd_data->VertexData);
    bknd_data->VertexSize = draw_data->TotalVtxCount * 1.1f;
    bknd_data->VertexData = reinterpret_cast<ImDrawVert *>(
        linearAlloc(sizeof(ImDrawVert) * bknd_data->VertexSize));
    NPI_ASSERT(bknd_data->VertexData);
  }
  if (bknd_data->IndexSize < static_cast<size_t>(draw_data->TotalIdxCount)) {
    linearFree(bknd_data->IndexData);
    bknd_data->IndexSize = draw_data->TotalIdxCount * 1.1f;
    bknd_data->IndexData = reinterpret_cast<ImDrawIdx *>(
        linearAlloc(sizeof(ImDrawIdx) * bknd_data->IndexSize));
    NPI_ASSERT(bknd_data->IndexData);
  }

  const auto clipOFF = draw_data->DisplayPos;
  const auto clipScale = draw_data->FramebufferScale;

  size_t VertexOffset = 0;
  size_t IndexOffset = 0;
  for (int i = 0; i < draw_data->CmdListsCount; i++) {
    const auto &cmdlist = *draw_data->CmdLists[i];
    NPI_ASSERT(bknd_data->VertexSize - VertexOffset >=
               static_cast<size_t>(cmdlist.VtxBuffer.Size));
    NPI_ASSERT(bknd_data->IndexSize - IndexOffset >=
               static_cast<size_t>(cmdlist.IdxBuffer.Size));

    memcpy(&bknd_data->VertexData[VertexOffset], cmdlist.VtxBuffer.Data,
           sizeof(ImDrawVert) * cmdlist.VtxBuffer.Size);
    memcpy(&bknd_data->IndexData[IndexOffset], cmdlist.IdxBuffer.Data,
           sizeof(ImDrawIdx) * cmdlist.IdxBuffer.Size);

    VertexOffset += cmdlist.VtxBuffer.Size;
    IndexOffset += cmdlist.IdxBuffer.Size;
  }

  for (const auto &it : {GFX_TOP, GFX_BOTTOM}) {
    if (it == GFX_TOP)
      C3D_FrameDrawOn(reinterpret_cast<C3D_RenderTarget *>(t_top));
    else
      C3D_FrameDrawOn(reinterpret_cast<C3D_RenderTarget *>(t_bot));

    SetupRendererForScreen(it);

    VertexOffset = 0;
    IndexOffset = 0;

    for (int i = 0; i < draw_data->CmdListsCount; i++) {
      const auto &cmdlist = *draw_data->CmdLists[i];
      for (const auto &cmd : cmdlist.CmdBuffer) {
        if (cmd.UserCallback) {
          // user callback, registered via ImDrawList::AddCallback()
          // (ImDrawCallback_ResetRenderState is a special callback value used
          // by the user to request the renderer to reset render state.)
          if (cmd.UserCallback == ImDrawCallback_ResetRenderState)
            SetupRendererForScreen(it);
          else
            cmd.UserCallback(&cmdlist, &cmd);
        } else {
          ImVec4 clip;
          clip.x = (cmd.ClipRect.x - clipOFF.x) * clipScale.x;
          clip.y = (cmd.ClipRect.y - clipOFF.y) * clipScale.y;
          clip.z = (cmd.ClipRect.z - clipOFF.x) * clipScale.x;
          clip.w = (cmd.ClipRect.w - clipOFF.y) * clipScale.y;

          if (clip.x >= width || clip.y >= height || clip.z < 0.0f ||
              clip.w < 0.0f)
            continue;
          if (clip.x < 0.0f) clip.x = 0.0f;
          if (clip.y < 0.0f) clip.y = 0.0f;
          if (clip.z > width) clip.z = width;
          if (clip.w > height) clip.z = height;
          if (it == GFX_TOP) {
            // check if clip starts on bottom screen
            if (clip.y > height * 0.5f) continue;

            // convert from framebuffer space to screen space (3DS screen
            // rotation)
            auto const x1 =
                std::clamp<unsigned>(height * 0.5f - clip.w, 0, height * 0.5f);
            auto const y1 = std::clamp<unsigned>(width - clip.z, 0, width);
            auto const x2 =
                std::clamp<unsigned>(height * 0.5f - clip.y, 0, height * 0.5f);
            auto const y2 = std::clamp<unsigned>(width - clip.x, 0, width);

            // check if scissor needs to be updated
            if (bknd_data->boundScissor[0] != x1 ||
                bknd_data->boundScissor[1] != y1 ||
                bknd_data->boundScissor[2] != x2 ||
                bknd_data->boundScissor[3] != y2) {
              bknd_data->boundScissor[0] = x1;
              bknd_data->boundScissor[1] = y1;
              bknd_data->boundScissor[2] = x2;
              bknd_data->boundScissor[3] = y2;
              C3D_SetScissor(GPU_SCISSOR_NORMAL, x1, y1, x2, y2);
            }
          } else {
            // check if clip ends on top screen
            if (clip.w < height * 0.5f) continue;

            // check if clip ends before left edge of bottom screen
            if (clip.z < width * 0.1f) continue;

            // check if clip starts after right edge of bottom screen
            if (clip.x > width * 0.9f) continue;

            // convert from framebuffer space to screen space
            // (3DS screen rotation + bottom screen offset)
            auto const x1 =
                std::clamp<unsigned>(height - clip.w, 0, height * 0.5f);
            auto const y1 =
                std::clamp<unsigned>(width * 0.9f - clip.z, 0, width * 0.8f);
            auto const x2 =
                std::clamp<unsigned>(height - clip.y, 0, height * 0.5f);
            auto const y2 =
                std::clamp<unsigned>(width * 0.9f - clip.x, 0, width * 0.8f);

            // check if scissor needs to be updated
            if (bknd_data->boundScissor[0] != x1 ||
                bknd_data->boundScissor[1] != y1 ||
                bknd_data->boundScissor[2] != x2 ||
                bknd_data->boundScissor[3] != y2) {
              bknd_data->boundScissor[0] = x1;
              bknd_data->boundScissor[1] = y1;
              bknd_data->boundScissor[2] = x2;
              bknd_data->boundScissor[3] = y2;
              C3D_SetScissor(GPU_SCISSOR_NORMAL, x1, y1, x2, y2);
            }
          }

          // Update Vertex Data if required
          auto const vtxData =
              &bknd_data->VertexData[cmd.VtxOffset + VertexOffset];
          if (vtxData != bknd_data->boundVertexData) {
            bknd_data->boundVertexData = vtxData;
            auto const bufInfo = C3D_GetBufInfo();
            BufInfo_Init(bufInfo);
            BufInfo_Add(bufInfo, vtxData, sizeof(ImDrawVert), 3, 0x210);
          }

          // Check if Bound Texture needs to be updated
          auto tex = static_cast<C3D_Tex *>(cmd.TextureId);
          if (tex == bknd_data->FontTextures.data()) {
            NPI_ASSERT(cmd.ElemCount % 3 == 0);

            // get sheet number from uv coords
            auto const getSheet = [](auto const vtx_, auto const idx_) {
              unsigned const sheet = std::min(
                  {vtx_[idx_[0]].uv.y, vtx_[idx_[1]].uv.y, vtx_[idx_[2]].uv.y});

              // NPI_ASSERT that these three vertices use the same sheet
              for (unsigned i = 0; i < 3; ++i)
                NPI_ASSERT(vtx_[idx_[i]].uv.y - sheet <= 1.0f);
              auto ldt = ImGui_ImplCitro3D_GetBackendData();
              NPI_ASSERT(sheet < ldt->FontTextures.size());
              return sheet;
            };

            for (unsigned i = 0; i < cmd.ElemCount; i += 3) {
              auto const idx = &cmdlist.IdxBuffer.Data[cmd.IdxOffset + i];
              auto const vtx = &cmdlist.VtxBuffer.Data[cmd.VtxOffset];
              auto drawVtx =
                  &bknd_data->VertexData[cmd.VtxOffset + VertexOffset];

              auto const sheet = getSheet(vtx, idx);
              if (sheet != 0) {
                float dummy;
                drawVtx[idx[0]].uv.y = std::modf(drawVtx[idx[0]].uv.y, &dummy);
                drawVtx[idx[1]].uv.y = std::modf(drawVtx[idx[1]].uv.y, &dummy);
                drawVtx[idx[2]].uv.y = std::modf(drawVtx[idx[2]].uv.y, &dummy);
              }
            }

            // initialize texture binding
            unsigned boundSheet =
                getSheet(&cmdlist.VtxBuffer.Data[cmd.VtxOffset],
                         &cmdlist.IdxBuffer.Data[cmd.IdxOffset]);

            NPI_ASSERT(boundSheet < bknd_data->FontTextures.size());
            C3D_TexBind(0, &bknd_data->FontTextures[boundSheet]);

            unsigned offset = 0;

            // update texture environment for non-image drawing
            auto const env = C3D_GetTexEnv(0);
            C3D_TexEnvInit(env);
            C3D_TexEnvSrc(env, C3D_RGB, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR,
                          GPU_PRIMARY_COLOR);
            C3D_TexEnvFunc(env, C3D_RGB, GPU_REPLACE);
            C3D_TexEnvSrc(env, C3D_Alpha, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                          GPU_PRIMARY_COLOR);
            C3D_TexEnvFunc(env, C3D_Alpha, GPU_MODULATE);

            // process one triangle at a time
            for (unsigned i = 3; i < cmd.ElemCount; i += 3) {
              // get sheet for this triangle
              unsigned const sheet =
                  getSheet(&cmdlist.VtxBuffer.Data[cmd.VtxOffset],
                           &cmdlist.IdxBuffer.Data[cmd.IdxOffset + i]);

              // check if we're changing textures
              if (boundSheet != sheet) {
                // draw everything up until now
                C3D_DrawElements(
                    GPU_TRIANGLES, i - offset, C3D_UNSIGNED_SHORT,
                    &bknd_data
                         ->IndexData[cmd.IdxOffset + IndexOffset + offset]);

                // bind texture for next draw call
                boundSheet = sheet;
                offset = i;
                C3D_TexBind(0, &bknd_data->FontTextures[boundSheet]);
              }
            }

            // draw the final set of triangles
            NPI_ASSERT((cmd.ElemCount - offset) % 3 == 0);
            C3D_DrawElements(
                GPU_TRIANGLES, cmd.ElemCount - offset, C3D_UNSIGNED_SHORT,
                &bknd_data->IndexData[cmd.IdxOffset + IndexOffset + offset]);
          } else {
            if (tex != bknd_data->boundTexture) {
              C3D_TexBind(0, tex);

              auto const env = C3D_GetTexEnv(0);
              C3D_TexEnvInit(env);
              C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR,
                            GPU_PRIMARY_COLOR);
              C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
            }

            // draw triangles
            C3D_DrawElements(
                GPU_TRIANGLES, cmd.ElemCount, C3D_UNSIGNED_SHORT,
                &bknd_data->IndexData[cmd.IdxOffset + IndexOffset]);
          }

          bknd_data->boundTexture = tex;
        }
      }
      VertexOffset += cmdlist.VtxBuffer.Size;
      IndexOffset += cmdlist.IdxBuffer.Size;
    }
  }
}
