#pragma once
#include <string>
#include <cstdint>
namespace ttyd {
// Original US selector1 template branch. Replace first AN, then first ITEM;
// preserve all other bytes/control tokens. Caller supplies original msgSearch.
std::string expandUsItemGetText(std::string sourceTemplate,const char* originalName);
// Original US coin branch replaces only the first NUM and S tokens.
std::string expandUsCoinGetText(std::string sourceTemplate,int32_t amount);
}
