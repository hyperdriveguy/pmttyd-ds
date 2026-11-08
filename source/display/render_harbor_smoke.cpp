#include "hit_n64_shbin.h"
#include "native_effect_process.hpp"
#include "native_graphics_program.hpp"
#include "native_harbor_smoke.hpp"
#include "platform.hpp"
#include <cmath>
namespace ttyd {
// Native camera reflow; source Entry/Main operands and original pixels remain
// independent of original effect-pool and queued camera4 display completion.
struct Renderer::NativeHarborSmoke {
  struct Vertex {
    float x, y, z, u0, v0, u1, v1, r, g, b, a;
  };
  static constexpr unsigned Vertices = NativeHarborSmokeVisual::Capacity *
                                       NativeHarborSmokeVisual::Particles * 12;
  Renderer *renderer = nullptr;
  bool alive = true;
  std::weak_ptr<const void> runtime;
  std::shared_ptr<NativeEffectProcess> process;
  std::unique_ptr<NativeHarborSmokeVisual> visual;
  Vertex *vertices = nullptr;
  C3D_Tex texture{};
  Tex3DS_Texture imported = nullptr;
  DVLB_s *shader = nullptr;
  shaderProgram_s program{};
  bool initialized = false;
  int transform = -1;
  unsigned count = 0;
  bool current() const {
    return alive && renderer && renderer->c3dReady && runtime.lock() &&
           process && process->current() && process->texturesReady() &&
           vertices && imported && initialized && transform >= 0;
  }
  void release() {
    alive = false;
    if (visual)
      visual->retire();
    if (renderer && renderer->c3dReady) {
      C3D_FrameSync();
      bindNativeGraphicsParkingProgram();
      renderer->parkUnusedSamplers();
      C3D_TexBind(0, &renderer->fontTexture.texture);
    }
    if (vertices)
      linearFree(vertices);
    vertices = nullptr;
    if (imported) {
      Tex3DS_TextureFree(imported);
      C3D_TexDelete(&texture);
      imported = nullptr;
    }
    if (initialized) {
      shaderProgramFree(&program);
      initialized = false;
    }
    if (shader) {
      DVLB_Free(shader);
      shader = nullptr;
    }
    renderer = nullptr;
  }
};
bool Renderer::prepareNativeHarborSmoke(Game &g) {
  if (!tutorialReady || !c3dReady || !g.harborTutorial.running())
    return false;
  const auto runtime = g.harborTutorial.nativeGameplayLifetime().lock();
  const auto process = g.nativeEffects();
  if (!runtime || !process || !process->current() || !process->texturesReady())
    return false;
  if (nativeHarborSmoke && nativeHarborSmoke->runtime.lock() != runtime)
    retireNativeHarborSmoke();
  if (!nativeHarborSmoke) {
    auto c = std::make_shared<NativeHarborSmoke>();
    c->renderer = this;
    c->runtime = runtime;
    c->process = process;
    NativeHarborSmokeData data;
    if (!data.load("romfs:/harbor-smoke/memory.pmevt"))
      return false;
    FILE *f = std::fopen("romfs:/harbor-smoke40.t3x", "rb");
    if (!f)
      return false;
    c->imported = Tex3DS_TextureImportStdio(f, &c->texture, nullptr, false);
    std::fclose(f);
    const auto *image =
        c->imported && Tex3DS_GetNumSubTextures(c->imported) == 1
            ? Tex3DS_GetSubTexture(c->imported, 0)
            : nullptr;
    if (!c->imported || c->texture.width != 512 || c->texture.height != 32 ||
        c->texture.fmt != GPU_RGBA8 || !image || image->width != 512 ||
        image->height != 32 || image->left != 0 || image->right != 1 ||
        image->top != 1 || image->bottom != 0) {
      c->release();
      return false;
    }
    C3D_TexSetFilter(&c->texture, GPU_LINEAR, GPU_LINEAR);
    C3D_TexSetWrap(&c->texture, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
    c->shader =
        DVLB_ParseFile(const_cast<uint32_t *>(
                           reinterpret_cast<const uint32_t *>(hit_n64_shbin)),
                       hit_n64_shbin_size);
    if (!c->shader || c->shader->numDVLE != 1) {
      c->release();
      return false;
    }
    shaderProgramInit(&c->program);
    c->initialized = true;
    shaderProgramSetVsh(&c->program, &c->shader->DVLE[0]);
    c->transform =
        shaderInstanceGetUniformLocation(c->program.vertexShader, "transform");
    c->vertices = static_cast<NativeHarborSmoke::Vertex *>(linearAlloc(
        NativeHarborSmoke::Vertices * sizeof(NativeHarborSmoke::Vertex)));
    if (!c->current()) {
      c->release();
      return false;
    }
    const std::weak_ptr<NativeHarborSmoke> weak = c;
    c->visual = std::make_unique<NativeHarborSmokeVisual>(
        data, std::make_shared<const uint8_t>(0), [weak] {
          auto v = weak.lock();
          return v && v->current();
        });
    NativeHarborSmokeConsumer consumer;
    consumer.user = c.get();
    consumer.lifetime = c;
    consumer.current = [](void *p) {
      return static_cast<NativeHarborSmoke *>(p)->current();
    };
    consumer.spawn = [](void *p, const NativeHarborSmokeRequest &q,
                        const NativeHarborSmokeSource &s) {
      auto &v = *static_cast<NativeHarborSmoke *>(p);
      return v.current() ? v.visual->spawn(q, s) : std::optional<uint64_t>{};
    };
    consumer.advance = [](void *p, uint64_t visit) {
      auto &v = *static_cast<NativeHarborSmoke *>(p);
      return v.current() && v.visual->advance(visit);
    };
    if (!g.harborTutorial.bindNativeSmoke(consumer)) {
      c->release();
      return false;
    }
    nativeHarborSmoke = std::move(c);
  }
  return nativeHarborSmoke->current();
}
void Renderer::drawNativeHarborSmoke(const Game &g, bool lower) {
  const auto c = nativeHarborSmoke;
  if (!c || !c->current() ||
      c->runtime.lock() != g.harborTutorial.nativeGameplayLifetime().lock())
    return;
  if (!lower) {
    unsigned count = 0;
    const auto &instances = c->visual->instances();
    // Complete finite geometry preflight before any upload/GPU mutation.
    for (const auto &i : instances)
      if (i.id)
        for (const auto &p : i.particles)
          if (!p.delay && p.age && p.frame <= 15) {
            for (unsigned shadow = 0; shadow < 2; ++shadow) {
              const float dx = shadow ? 0 : 2, dy = shadow ? 0 : -2;
              const float x =
                  (i.request.position[0] + (p.x + dx) * i.request.scale) * .01f;
              const float y =
                  (i.request.position[1] + (p.y + dy) * i.request.scale) * .01f;
              const float z = i.request.position[2] * .01f,
                          half = 16 * i.request.scale * .01f;
              if (!std::isfinite(x - half) || !std::isfinite(x + half) ||
                  !std::isfinite(y - half) || !std::isfinite(y + half) ||
                  !std::isfinite(z) || count + 6 > NativeHarborSmoke::Vertices)
                return;
              const float u0 = float(p.frame) / 16,
                          u1 = float(p.nextFrame) / 16, width = 1.f / 16;
              const float shade = shadow ? 1 : 0,
                          alpha = shadow ? 1 : 96.f / 255;
              const NativeHarborSmoke::Vertex q[4] = {
                  {x - half, y + half, z, u0, 1, u1, 1, shade, shade, shade,
                   alpha},
                  {x + half, y + half, z, u0 + width, 1, u1 + width, 1, shade,
                   shade, shade, alpha},
                  {x + half, y - half, z, u0 + width, 0, u1 + width, 0, shade,
                   shade, shade, alpha},
                  {x - half, y - half, z, u0, 0, u1, 0, shade, shade, shade,
                   alpha}};
              for (unsigned n : {0u, 1u, 2u, 0u, 2u, 3u})
                c->vertices[count++] = q[n];
            }
          }
    c->count = count;
    if (count)
      GSPGPU_FlushDataCache(c->vertices,
                            count * sizeof(NativeHarborSmoke::Vertex));
  }
  if (!c->count)
    return;
  C3D_BindProgram(&c->program);
  C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, c->transform, &worldTransform);
  auto *a = C3D_GetAttrInfo();
  AttrInfo_Init(a);
  AttrInfo_AddLoader(a, 0, GPU_FLOAT, 3);
  AttrInfo_AddLoader(a, 1, GPU_FLOAT, 2);
  AttrInfo_AddLoader(a, 2, GPU_FLOAT, 2);
  AttrInfo_AddLoader(a, 3, GPU_FLOAT, 4);
  auto *b = C3D_GetBufInfo();
  BufInfo_Init(b);
  BufInfo_Add(b, c->vertices, sizeof(NativeHarborSmoke::Vertex), 4, 0x3210);
  parkUnusedSamplers();
  C3D_TexBind(0, &c->texture);
  C3D_TexBind(1, &c->texture);
  for (unsigned stage = 0; stage < 6; ++stage)
    C3D_TexEnvInit(C3D_GetTexEnv(stage));
  auto *e = C3D_GetTexEnv(0);
  C3D_TexEnvSrc(e, C3D_Both, GPU_TEXTURE0);
  C3D_TexEnvFunc(e, C3D_Both, GPU_REPLACE);
  e = C3D_GetTexEnv(1);
  C3D_TexEnvSrc(e, C3D_Both, GPU_TEXTURE1, GPU_PREVIOUS, GPU_CONSTANT);
  C3D_TexEnvFunc(e, C3D_Both, GPU_INTERPOLATE);
  C3D_TexEnvColor(e, C2D_Color32(128, 128, 128, 128));
  e = C3D_GetTexEnv(2);
  C3D_TexEnvSrc(e, C3D_Both, GPU_PREVIOUS, GPU_PRIMARY_COLOR);
  C3D_TexEnvFunc(e, C3D_Both, GPU_MODULATE);
  C3D_CullFace(GPU_CULL_NONE);
  C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_COLOR);
  C3D_AlphaTest(false, GPU_ALWAYS, 0);
  C3D_AlphaBlend(GPU_BLEND_ADD, GPU_BLEND_ADD, GPU_SRC_ALPHA,
                 GPU_ONE_MINUS_SRC_ALPHA, GPU_ONE, GPU_ONE_MINUS_SRC_ALPHA);
  C3D_DrawArrays(GPU_TRIANGLES, 0, c->count);
}
bool Renderer::nativeHarborSmokeMatches(const std::shared_ptr<const void>& runtime) const {
 if(!nativeHarborSmoke)return true;
 const auto owned=nativeHarborSmoke->runtime.lock();
 return owned&&runtime&&owned==runtime&&!owned.owner_before(runtime)&&!runtime.owner_before(owned);
}
void Renderer::retireNativeHarborSmoke() {
  auto c = std::move(nativeHarborSmoke);
  if (c)
    c->release();
}
} // namespace ttyd
