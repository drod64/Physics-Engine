#ifndef SGE_RESOURCE_REGISTRY_H
#define SGE_RESOURCE_REGISTRY_H
#include <cstdint>

namespace sge {
    
// Creates a unique ID for every different class/struct type.
class ResourceIDCounter {
private:
    static inline size_t m_nextID = 0;

public:
    template <typename T>
    static size_t get()
    {
        static const size_t ID = m_nextID++;
        return ID;
    }

}; // class ResourceIDCounter





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