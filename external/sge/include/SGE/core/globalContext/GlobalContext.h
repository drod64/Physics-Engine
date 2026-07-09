#ifndef SGE_GLOBAL_CONTEXT_H
#define SGE_GLOBAL_CONTEXT_H
#include <any>
#include <unordered_map>
#include <cassert>
#include <exception>
#include <SGE/core/globalContext/GlobalContextIDCounter.h>

namespace sge {
class GlobalContext {
private:
    std::unordered_map<GlobalContextID, std::any> m_registry;
    bool m_isLocked = false;

public:
    GlobalContext() = default;

    /**
     * Register a context.
     * @tparam T the context type
     * @tparam Args the constructor arguments
     * @param args the type T constructor arguments
     * @return a reference to the newly registered context
     */
    template <typename T, typename... Args>
    T& registerContext(Args&&... args);

    /**
     * Lock the GlobalContext from registering more contexts.
     * Call this before the game loop starts.
     */
    void lockInitialization();
    
    /**
     * Retrieves a context.
     * @tparam T the context type
     * @return a reference to the context
     */
    template <typename T>
    [[nodiscard]] T& get();

    /**
     * Retrieves a context.
     * @tparam T the context type
     * @return a const reference to the context
     */
    template <typename T>
    [[nodiscard]] const T& get() const;

    /**
     * Checks if the GlobalContext has a context.
     * @tparam T the context type
     * @return true/false whether the context was registered
     */
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

    const sge::GlobalContextID ID =  sge::GlobalContextIDCounter::get<T>();
    assert(this->m_registry.find(ID) == this->m_registry.end() && "Context type has already been registered");

    auto [it, inserted] = this->m_registry.emplace(ID, std::make_any<T>(std::forward<Args>(args)...));
    return std::any_cast<T&>(it->second);
}

inline void sge::GlobalContext::lockInitialization()
{
    this->m_isLocked = true;
}

template <typename T>
inline T& sge::GlobalContext::get()
{
    const sge::GlobalContextID ID = sge::GlobalContextIDCounter::get<T>();
    auto it = this->m_registry.find(ID);

    assert(it != this->m_registry.end() && "Requested context was never registered during boot!");

    return std::any_cast<T&>(it->second);
}

template <typename T>
inline const T& sge::GlobalContext::get() const
{
    const sge::GlobalContextID ID = sge::GlobalContextIDCounter::get<T>();
    auto it = this->m_registry.find(ID);

    assert(it != this->m_registry.end() && "Requested context was never registered during boot!");

    return std::any_cast<const T&>(it->second);
}

template <typename T>
inline bool sge::GlobalContext::has() const
{
    const sge::GlobalContextID ID = sge::GlobalContextIDCounter::get<T>();
    auto it = this->m_registry.find(ID);

    return (it != this->m_registry.end());
}

#endif // SGE_GLOBAL_CONTEXT_H