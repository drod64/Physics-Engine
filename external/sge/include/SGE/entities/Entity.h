#ifndef SGE_ENTITY_H
#define SGE_ENTITY_H
#include <memory>
#include <tuple>
#include <SGE/components/CBoundingBox.h>
#include <SGE/components/CCollision.h>
#include <SGE/components/CInput.h>
#include <SGE/components/CLifespan.h>
#include <SGE/components/CRigidBody3.h>
#include <SGE/components/CScore.h>
#include <SGE/components/CShape.h>
#include <SGE/components/CTransform.h>
#include <SGE/components/CTransform3.h>


namespace sge {

// Forward declaration of the EntityManager class.
class EntityManager;

typedef std::tuple<
    CBoundingBox,
    CCollision,
    CInput,
    CLifespan,
    CRigidBody3,
    CScore,
    CShape,
    CTransform,
    CTransform3
> ComponentTuple;

class Entity {
private:
    const size_t m_ID;
    const std::string m_TAG;
    bool m_isAlive;
    ComponentTuple m_components;

    Entity();
    Entity(const std::string &tag, size_t id);

public:

    void destroy();
    bool isAlive() const;
    size_t id() const;
    const std::string& tag() const;

    template<typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        return std::get<T>(this->m_components);
    }

    template<typename T>
    const T& getComponent() const
    {
        return std::get<T>(this->m_components);
    }

    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }

    friend class EntityManager;
};

} // namespace sge

#endif // SGE_ENTITY_H