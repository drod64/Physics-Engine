#include <SGE/systems/SystemManager.h>

sge::SystemManager::SystemManager()
{
    this->m_isCompiled = false;
}

void sge::SystemManager::registerSystem(SystemDescriptor desc)
{
    if (this->m_isCompiled)
    {
        throw std::runtime_error("[SystemManager]: Cannot register new systems after compile() has been called!");
    }

    this->m_rawSystems.push_back(std::move(desc));
}

void sge::SystemManager::registerSystem(SystemFn functionPtr, ComponentMask reads, ComponentMask writes, std::string name)
{
    if (this->m_isCompiled)
    {
        throw std::runtime_error("[SystemManager]: Cannot register new systems after compile() has been called!");
    }

    this->registerSystem(SystemDescriptor(functionPtr, reads, writes, name));
}

void sge::SystemManager::compile()
{   
    // Prevent double compilation.
    if (this->m_isCompiled) return;

    const size_t NUM_SYSTEMS = this->m_rawSystems.size();

    std::vector<std::vector<size_t>> dependents(NUM_SYSTEMS);
    std::vector<int> dependentDegrees(NUM_SYSTEMS, 0);

    // 1. Build graph and calculate inDegree.
    for (size_t i = 0; i < NUM_SYSTEMS; ++i)
    {
        for (size_t j = 0; j < NUM_SYSTEMS; ++j)
        {
            if (i == j) continue;

            const auto &sysA = this->m_rawSystems.at(i);
            const auto &sysB = this->m_rawSystems.at(j);

            // Check if System A writes to anything that System B reads or writes to.
            // If so, System A must execute first.
            bool hasDependency = (sysA.writes & sysB.reads).any() ||
                                (sysA.writes & sysB.writes).any();

            if (hasDependency)
            {
                dependents.at(i).push_back(j);
                ++dependentDegrees.at(j);
            }
        }
    }

    // 2. Push to queue systems with no dependencies.
    std::queue<size_t> noDependencies;
    for (size_t i = 0; i < NUM_SYSTEMS; ++i)
    {
        if (dependentDegrees.at(i) == 0)
        {
            noDependencies.push(i);
        }
    }
    
    // Prepare execution list to be filled.
    this->m_executionOrder.clear();
    this->m_executionOrder.reserve(NUM_SYSTEMS);
    
    // 3. Sort list.
    while (!noDependencies.empty())
    {
        // Pop independent system index from queue.
        size_t curSystem = noDependencies.front();
        noDependencies.pop();
    
        // Push independent system to sorted execution list
        this->m_executionOrder.push_back(this->m_rawSystems.at(curSystem));
        
        // Decrement dependent degree of systems that depended on the current system.
        for (size_t dependentSystem : dependents.at(curSystem))
        {
            --dependentDegrees.at(dependentSystem);
    
            // Add dependent system if it no longer has any dependencies.
            if (dependentDegrees.at(dependentSystem) == 0)
            {
                noDependencies.push(dependentSystem);
            }
        }
    }
    
    // 4. Check if no cycle was detected.
    if (this->m_executionOrder.size() != NUM_SYSTEMS)
    {
        throw std::runtime_error("[SystemManager]: Circular dependency detected. Ensure no two Systems depend on each other!");
    }

    this->m_isCompiled = true;
}

void sge::SystemManager::update(sge::Registry &registry, sge::CommandBuffer &cmdBuffer, sm::real dt)
{
    if (!this->m_isCompiled)
    {
        throw std::runtime_error("[SystemManager]: Cannot call update() before compiling the execution graph! Ensure to call compile() first.");
    }
    for (const auto &system : this->m_executionOrder)
    {
        system.functionPtr(registry, cmdBuffer, dt);
    }
}

void sge::SystemManager::clear()
{
    this->m_executionOrder.clear();
    this->m_rawSystems.clear();
    this->m_isCompiled = false;
}

bool sge::SystemManager::isCompiled() const noexcept
{
    return this->m_isCompiled;
}

size_t sge::SystemManager::getSystemCount() const noexcept
{
    return this->m_rawSystems.size();
}