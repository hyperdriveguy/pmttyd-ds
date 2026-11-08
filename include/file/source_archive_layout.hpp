#pragma once
#include <cstdint>
namespace ttyd::SourceArchiveLayout {
// arcOpen800782e4: lis r6,803e; addi r31,r6,-4010. The low half
// is signed. These are the shared process descriptors, not per-client copies.
constexpr uint32_t Descriptors=0x803dbff0;
constexpr uint32_t DescriptorCount=4,DescriptorBytes=40;
constexpr uint32_t TableBytes=DescriptorCount*DescriptorBytes;
}
