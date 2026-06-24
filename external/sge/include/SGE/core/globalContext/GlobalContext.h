#ifndef SGE_GLOBAL_CONTEXT_H
#define SGE_GLOBAL_CONTEXT_H
#include <any>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <exception>

namespace sge {
class GlobalContext {
private:
    std::unordered_map<std::type_index, std::any> m_registry;
    bool m_isLocked = false;

public:
    GlobalContext() = default;

    template <typename T, typename... Args>
    T& registerContext(Args&&... args);

    void lockInitialization();
    
    template <typename T>
    [[nodiscard]] T& get();

    template <typename T>
    [[nodiscard]] const T& get() const;

    template <typename T>
    bool has() const;
    
}; // class GlobalContext
} // namespace sge

// Implementation

template <typename T, typename... Args>
inline T& sge::GlobalContext::registerContext(Args&&... args)
{
    if (this->m_isLocked)
    {
        throw std::runtime_error("Cannot register new contexts after the GlobalContext has been locked!");
    }

    std::type_index typeID = typeid(T);
    assert(this->m_registry.find(typeID) == this->m_registry.end() && "Context type has already been registered");

    auto [it, inserted] = this->m_registry.emplace(typeID, std::make_any<T>(std::forward<Args>(args)...));
    return std::any_cast<T&>(it->second);
}

inline void sge::GlobalContext::lockInitialization()
{
    this->m_isLocked = true;
}

template <typename T>
inline T& sge::GlobalContext::get()
{
    std::type_index typeID = typeid(T);
    auto it = this->m_registry.find(typeID);

    assert(it != this->m_registry.end() && "Requested context was never registered during boot!");

    return std::any_cast<T&>(it->second);
}

template <typename T>
inline const T& sge::GlobalContext::get() const
{
    std::type_index typeID = typeid(T);
    auto it = this->m_registry.find(typeID);

    assert(it != this->m_registry.end() && "Requested context was never registered during boot!");

    return std::any_cast<const T&>(it->second);
}

template <typename T>
inline bool sge::GlobalContext::has() const
{
    std::type_index typeID = typeid(T);
    auto it = this->m_registry.find(typeID);

    return (it != this->m_registry.end());
}

#endif // SGE_GLOBAL_CONTEXT_H