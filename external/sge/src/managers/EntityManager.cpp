#include <SGE/managers/EntityManager.h>

const uint32_t sge::EntityManager::INDEX_MASK = 0x000FFFFF;
const uint32_t sge::EntityManager::MIN_GENERATION = 1;

sge::EntityManager::EntityManager()
{
    // Push a dummy slot which represents an invalid Entity ID index and generation.
    this->m_generations.push_back(0);
}

sge::Entity sge::EntityManager::createEntityID(uint32_t index, uint32_t generation) const
{
    return static_cast<Entity>((generation << 20) | (index & sge::EntityManager::INDEX_MASK));
}

uint32_t sge::EntityManager::getIndex(sge::Entity e) const
{
    return static_cast<uint32_t>(e) & sge::EntityManager::INDEX_MASK;
}

uint32_t sge::EntityManager::getGeneration(sge::Entity e) const
{
    return static_cast<uint32_t>(e) >> 20;
}

sge::Entity sge::EntityManager::createEntity()
{
    uint32_t index;

    if (!this->m_freeIndices.empty())
    {
        index = this->m_freeIndices.front();
        this->m_freeIndices.pop();
    }
    else
    {
        index = static_cast<uint32_t>(this->m_generations.size());
        this->m_generations.push_back(sge::EntityManager::MIN_GENERATION);
    }

    return this->createEntityID(index, this->m_generations.at(index));
}

void sge::EntityManager::destroyEntity(Entity e)
{
    // Early exit if Entity ID does not exist anymore.
    if (!isAlive(e)) return;
    
    // Get Entity index
    uint32_t index = this->getIndex(e);

    // Increment generation count of this index.
    ++this->m_generations.at(index);

    // Wrap generation back to 1 if it is out of bounds.
    if (this->m_generations.at(index) >= 4096 || this->m_generations.at(index) == 0)
    {
        this->m_generations.at(index) = sge::EntityManager::MIN_GENERATION;
    }

    // Recycle index
    this->m_freeIndices.push(index);
}

bool sge::EntityManager::isAlive(Entity e) const
{
    // If Entity is fake, exit
    if (IsFakeEntity(e)) return false;

    uint32_t index = this->getIndex(e);
    uint32_t generation = this->getGeneration(e);

    // If generation is invalid, exit
    if (generation == 0) return false;

    // If index is out of bounds
    if (index <= 0 || index >= this->m_generations.size()) return false;

    // Check if calculated index holds the same generation value...
    //...as the passed in Entity ID's generation.
    return this->m_generations[index] == generation;
}