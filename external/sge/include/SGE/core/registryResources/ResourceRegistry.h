#ifndef SGE_RESOURCE_REGISTRY_H
#define SGE_RESOURCE_REGISTRY_H
#include <cstdint>
#include <bitset>

namespace sge {

using ResourceID = uint32_t;
// Creates a unique ID for every different class/struct type.
class ResourceIDCounter {
private:
    static inline ResourceID nextID()
    {
        static ResourceID counter = 0;
        return counter++;
    }

public:
    template <typename T>
    static ResourceID get()
    {
        static const ResourceID ID = nextID();
        return ID;
    }

}; // class ResourceIDCounter
constexpr size_t MAX_RESOURCES = 64;
using ResourceMask = std::bitset<MAX_RESOURCES>;



// Base class for resources
class IResourceStorage {
public:
    ~IResourceStorage() = default;
}; // class IResourceStorage





template <typename T>
class ConcreteResourceStorage : public IResourceStorage {
public:
    T value;
}; // class ConcreteResourceStorage

} // namespace sge

#endif // SGE_RESOURCE_REGISTRY_H