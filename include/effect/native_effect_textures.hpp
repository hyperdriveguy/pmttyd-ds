#pragma once
#include "native_source_texture_startup.hpp"
namespace ttyd {
// Preserve the established effect facade name; create() selects original
// primary/N64 effect drivers in the shared startup texture backend.
using NativeEffectTextures = NativeSourceTextureStartup;
} // namespace ttyd
