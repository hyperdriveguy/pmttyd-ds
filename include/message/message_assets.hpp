#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace ttyd {
// Byte offsets into one retained resource buffer, not independently owned
// strings. Sorting the index never reorders or rewrites original message bytes.
struct MessagePair {size_t key, text;};
class MessageResource {
public:
    // Original alternating NUL key/text bytes, including all control tokens.
    // Rejected loads preserve the resident resource and its returned pointers.
    bool load(const char* path);
    bool assign(std::vector<uint8_t> bytes);
    // Borrowed original text; valid until successful reload/assignment or
    // destruction. Duplicate keys return null instead of choosing a story line.
    const char* find(const char* key) const;
    size_t size() const {return index_.size();}
    bool ambiguous(const char* key) const;
private:
    std::vector<uint8_t> bytes_;
    std::vector<MessagePair> index_;
};
class MessageAssets {
public:
    // Slot 0 (room) precedes slot 1 (global), as in US msgSearch.
    bool loadGlobal(const char* path) {return global_.load(path);}
    bool loadRoom(const char* path) {return room_.load(path);}
    const char* find(const char* key) const;
private:
    MessageResource room_,global_;
};
}
