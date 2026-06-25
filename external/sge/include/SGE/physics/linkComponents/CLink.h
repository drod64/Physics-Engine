#ifndef SGE_CLINK_H
#define SGE_CLINK_H
#include <unordered_map>
#include <SGE/core/ecs/Entity.h>

namespace sge {
template <typename Derived>
struct CLink {
    void remapPlaceholder(const std::unordered_map<uint32_t, sge::Entity>& translationMap)
    {
        // Cast to derived component type.
        auto *derived = static_cast<Derived*>(this);

        // Check if the target entity is a placeholder (fake).
        if (sge::IsFakeEntity(derived->targetEntity))
        {
            auto it = translationMap.find(static_cast<uint32_t>(derived->targetEntity));

            // Update the target entity to be a registered entity.
            if (it != translationMap.end())
            {
                derived->targetEntity = it->second;
            }
        }
    }
}; // struct CLink
} // namespace sge

#endif // SGE_CLINK_H