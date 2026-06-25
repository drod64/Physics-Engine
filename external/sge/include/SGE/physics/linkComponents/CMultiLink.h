#ifndef SGE_CMULTI_LINK_H
#define SGE_CMULTI_LINK_H
#include <unordered_map>
#include <SGE/core/ecs/Entity.h>

namespace sge {
template <typename Derived>
struct CMultiLink {
    void remapPlaceholder(const std::unordered_map<uint32_t, sge::Entity>& translationMap)
    {
        // Cast to derived component type.
        auto *derived = static_cast<Derived*>(this);

        for (sge::Entity &entity : derived->targetEntities)
        {
            // Check if the target entity is a placeholder (fake).
            if (sge::IsFakeEntity(entity))
            {
                auto it = translationMap.find(static_cast<uint32_t>(entity));
    
                // Update the target entity to be a registered entity.
                if (it != translationMap.end())
                {
                    entity = it->second;
                }
            }
        }
    }
}; // struct CMultiLink
} // namespace sge

#endif // SGE_CMULTI_LINK_H