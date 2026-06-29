#include <SGE/physics/constraints/ConstraintManager.h>

const uint32_t sge::ConstraintManager::INDEX_MASK = 0x000FFFFF;
const uint32_t sge::ConstraintManager::MIN_GENERATION = 1;

sge::Constraint sge::ConstraintManager::createConstraintID(uint32_t index, uint32_t generation) const
{
    return static_cast<sge::Constraint>((generation << 20) | (index & sge::ConstraintManager::INDEX_MASK));
}

sge::ConstraintManager::ConstraintManager()
{
    this->m_generations.push_back(0);
}

uint32_t sge::ConstraintManager::getIndex(sge::Constraint c) const
{
    return static_cast<uint32_t>(c) & sge::ConstraintManager::INDEX_MASK;
}

uint32_t sge::ConstraintManager::getGeneration(sge::Constraint c) const
{
    return static_cast<uint32_t>(c) >> 20;
}

sge::Constraint sge::ConstraintManager::createConstraint()
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
        this->m_generations.push_back(sge::ConstraintManager::MIN_GENERATION);
    }

    return this->createConstraintID(index, this->m_generations[index]);
}

void sge::ConstraintManager::destroyConstraint(sge::Constraint c)
{
    if (!this->isValid(c)) return;

    uint32_t index = this->getIndex(c);

    ++this->m_generations[index];

    if (this->m_generations[index] >= 4096 || this->m_generations[index] == 0)
    {
        this->m_generations[index] = sge::ConstraintManager::MIN_GENERATION;
    }

    this->m_freeIndices.push(index);
}

bool sge::ConstraintManager::isValid(sge::Constraint c) const
{
    if (sge::IsFakeConstraint(c)) return false;

    uint32_t index = this->getIndex(c);
    uint32_t generation = this->getGeneration(c);

    if (index <= 0 || index >= this->m_generations.size()) return false;

    return this->m_generations[index] == generation;
}