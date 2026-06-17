#ifndef SGE_CTRANSFORM_H
#define SGE_CTRANSFORM_H
#include <SM/Vec2.h>
#include <initializer_list>

namespace sge {
/**
 * NOTE: OUTDATED
 * Component that stores transform data for a 2D environment.
 */
class CTransform {
public:
    sm::Vec2 position;
    sm::Vec2 prevPosition;
    sm::Vec2 velocity;
    sm::Vec2 scale;
    float angle;

    /**
     * Default Constructor.
     */
    CTransform();

    /**
     * Copy Constructor.
     * @param otherTransform the other CTransform to copy
     */
    CTransform(const CTransform &otherTransform);

    /**
     * Parameterized Constructor.
     * @param position the position of the transform
     * @param velocity the velocity of the transform
     * @param angle the angle of the transform
     */
    CTransform(const sm::Vec2 &position, const sm::Vec2 &velocity, float angle);

    /**
     * Deconstructor.
     */
    ~CTransform();

    /**
     * Overload of assignment operator.
     * @param otherTransform the other CTransform to copy.
     * @return a reference to *this
     */
    CTransform& operator = (const CTransform &otherTransform);
};

}

#endif // SGE_CTRANSFORM_H