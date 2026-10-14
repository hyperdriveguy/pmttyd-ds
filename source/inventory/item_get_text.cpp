#include "item_get_text.hpp"
#include <cstring>
namespace ttyd {
std::string expandUsItemGetText(std::string text,const char* name) {
 if(!name)return text;
 const auto replace=[&](const char* token,const char* value){const auto at=text.find(token);if(at!=std::string::npos)text.replace(at,std::strlen(token),value);};
 const bool vowel=*name && std::strchr("AEIOUaeiou",*name);
 replace("<AN>",vowel?"an ":"a ");replace("<ITEM>",name);return text;
}
std::string expandUsCoinGetText(std::string text,int32_t amount) {
 const auto number=text.find("<NUM>");if(number!=std::string::npos)text.replace(number,5,std::to_string(amount));
 const auto suffix=text.find("<S>");if(suffix!=std::string::npos)text.replace(suffix,3,amount==1?"":"s");
 return text;
}
}
