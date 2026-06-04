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

/**
 * Stores entity data such as ID, tag, and it's list of components.
 */
class Entity {
private:
    const size_t m_ID;
    const std::string m_TAG;
    bool m_isAlive;
    ComponentTuple m_components;

    Entity();
    Entity(const std::string &tag, size_t id);

public:

    /**
     * Marks the entity as destoryed.
     * Ensure that the entity is stored in an object of type sge::EntityManager.
     */
    void destroy();

    /**
     * Checks if the entity is alive/still being used.
     * @return true if the entity is still alive, false if marked as destroyed
     */
    bool isAlive() const;

    /**
     * Retrieves the ID of the entity.
     * @return the ID of the entity
     */
    size_t id() const;

    /**
     * Retrieves the tag of the entity.
     * @return the tag of the entity
     */
    const std::string& tag() const;

    /**
     * Checks if the entity has a specific component.
     * @return true if the component is attached, false otherwise
     */
    template<typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    /**
     * Attaches a component to the entity.
     * @param mArgs the constructor arguments to create the component with
     * @return a reference to the newly attached component
     */
    template<typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    /**
     * Retrieves the desired component.
     * @return a reference to the component
     */
    template<typename T>
    T& getComponent()
    {
        return std::get<T>(this->m_components);
    }

    /**
     * Retrieves the desired component.
     * @return a const reference to the component
     */
    template<typename T>
    const T& getComponent() const
    {
        return std::get<T>(this->m_components);
    }

    /**
     * Removes an attached component from the entity.
     */
    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }

    friend class EntityManager;
};

} // namespace sge

#endif // SGE_ENTITY_H