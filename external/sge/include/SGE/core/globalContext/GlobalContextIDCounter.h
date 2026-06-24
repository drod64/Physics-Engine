#ifndef SGE_GLOBAL_CONTEXT_ID_COUNTER_H
#define SGE_GLOBAL_CONTEXT_ID_COUNTER_H
#include <cstdint>
#include <bitset>

namespace sge {

using GlobalContextID = uint32_t;

class GlobalContextIDCounter {
private:
    static inline GlobalContextID nextID()
    {
        static GlobalContextID counter = 0;
        return counter++;
    } 

public:
    template <typename>
    static inline GlobalContextID get()
    {
        static const GlobalContextID ID = nextID();
        return ID;
    }
}; // class GlobalContextIDCounter

constexpr size_t MAX_GLOBAL_CONTEXTS = 64;
using GlobalContextMask = std::bitset<MAX_GLOBAL_CONTEXTS>;

} // namespace sge

#endif // SGE_GLOBAL_CONTEXT_ID_COUNTER_H