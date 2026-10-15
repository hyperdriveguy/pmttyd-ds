#pragma once
#include <string>
#include <cstdint>
namespace ttyd {
// Original US selector1 template branch. Replace first AN, then first ITEM;
// preserve all other bytes/control tokens. Caller supplies original msgSearch.
// English AN decision uses first-byte ASCII vowel membership only, no phonetic
// exception/locale rewrite. First-token-only expansion retains other control bytes.
std::string expandUsItemGetText(std::string sourceTemplate,const char* originalName);
// Original US coin branch replaces only the first NUM and S tokens.
// Signed decimal NUM and singular only for amount==1 (zero/negative plural).
// Token lengths5/3 below are serialized <NUM>/<S> byte lengths.
std::string expandUsCoinGetText(std::string sourceTemplate,int32_t amount);
}
