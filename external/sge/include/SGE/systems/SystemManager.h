#ifndef SGE_SYSTEM_MANAGER_H
#define SGE_SYSTEM_MANAGER_H
#include <vector>
#include <queue>
#include <stdexcept>
#include <SGE/systems/SystemDescriptor.h>

namespace sge {
class SystemManager {
private:
    std::vector<SystemDescriptor> m_rawSystems;
    std::vector<SystemDescriptor> m_executionOrder;
    bool m_isCompiled;

public:
    SystemManager();
    ~SystemManager() = default;

    SystemManager(const SystemManager &) = delete;
    SystemManager& operator=(const SystemManager &) = delete;

    SystemManager(SystemManager &&) noexcept = default;
    SystemManager& operator=(SystemManager &&) noexcept = default;
    
    void registerSystem(SystemDescriptor desc);

    void registerSystem(SystemFn functionPtr, ComponentMask componentReads, ComponentMask componentWrites,
                        ResourceMask resourceReads, ResourceMask resourceWrites, std::string name);

    void compile();

    void update(Registry &registry, CommandBuffer &cmdBuffer, sm::real dt);

    void clear();

    bool isCompiled() const noexcept;

    size_t getSystemCount() const noexcept;
};
}

#endif // SGE_SYSTEM_MANAGER_H