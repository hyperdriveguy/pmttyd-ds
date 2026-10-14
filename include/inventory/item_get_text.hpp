#pragma once
#include <string>
namespace ttyd {
// Original US selector1 template branch. Replace first AN, then first ITEM;
// preserve all other bytes/control tokens. Caller supplies original msgSearch.
std::string expandUsItemGetText(std::string sourceTemplate,const char* originalName);
}
