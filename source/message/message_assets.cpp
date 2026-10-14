#include "message_assets.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <utility>

namespace ttyd {
bool MessageResource::load(const char* path) {
    if(!path)return false;
    FILE* file=std::fopen(path,"rb");if(!file)return false;
    if(std::fseek(file,0,SEEK_END)!=0){std::fclose(file);return false;}
    const long length=std::ftell(file);
    // Native safety bound; not an original msgLoad allocation rule.
    if(length<=0 || length>4*1024*1024 || std::fseek(file,0,SEEK_SET)!=0){std::fclose(file);return false;}
    std::vector<uint8_t> bytes(static_cast<size_t>(length));
    const size_t read=std::fread(bytes.data(),1,bytes.size(),file);std::fclose(file);
    return read==bytes.size() && assign(std::move(bytes));
}
bool MessageResource::assign(std::vector<uint8_t> bytes) {
    std::vector<MessagePair> index;size_t cursor=0;
    while(cursor<bytes.size() && bytes[cursor]) {
        const size_t key=cursor;
        auto end=std::find(bytes.begin()+cursor,bytes.end(),0);
        if(end==bytes.end())return false;
        cursor=size_t(end-bytes.begin())+1;
        // An empty text is the first-empty-string boundary, not a full pair.
        if(cursor>=bytes.size() || !bytes[cursor])return false;
        const size_t text=cursor;
        end=std::find(bytes.begin()+cursor,bytes.end(),0);
        if(end==bytes.end())return false;
        cursor=size_t(end-bytes.begin())+1;
        index.push_back({key,text});
    }
    if(cursor>=bytes.size())return false; // Missing resource terminator.
    // US census establishes zero padding after the terminator.
    if(std::any_of(bytes.begin()+cursor,bytes.end(),[](uint8_t b){return b!=0;}))return false;
    std::stable_sort(index.begin(),index.end(),[&](const MessagePair& a,const MessagePair& b){
        return std::strcmp(reinterpret_cast<const char*>(bytes.data()+a.key),reinterpret_cast<const char*>(bytes.data()+b.key))<0;
    });
    bytes_=std::move(bytes);index_=std::move(index);return true;
}
bool MessageResource::ambiguous(const char* key) const {
    if(!key)return false;
    auto first=std::lower_bound(index_.begin(),index_.end(),key,[&](const MessagePair& p,const char* k){return std::strcmp(reinterpret_cast<const char*>(bytes_.data()+p.key),k)<0;});
    return first!=index_.end() && first+1!=index_.end() &&
        std::strcmp(reinterpret_cast<const char*>(bytes_.data()+first->key),key)==0 &&
        std::strcmp(reinterpret_cast<const char*>(bytes_.data()+(first+1)->key),key)==0;
}
const char* MessageResource::find(const char* key) const {
    if(!key || ambiguous(key))return nullptr;
    auto it=std::lower_bound(index_.begin(),index_.end(),key,[&](const MessagePair& p,const char* k){return std::strcmp(reinterpret_cast<const char*>(bytes_.data()+p.key),k)<0;});
    return it!=index_.end() && std::strcmp(reinterpret_cast<const char*>(bytes_.data()+it->key),key)==0?
        reinterpret_cast<const char*>(bytes_.data()+it->text):nullptr;
}
const char* MessageAssets::find(const char* key) const {
    // Do not silently substitute a global line for an ambiguous room key.
    if(room_.ambiguous(key))return nullptr;
    if(const char* text=room_.find(key))return text;
    return global_.find(key);
}
}
