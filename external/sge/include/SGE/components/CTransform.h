#ifndef SGE_CTRANSFORM_H
#define SGE_CTRANSFORM_H
#include <SM/Vec2.h>
#include <SGE/components/Component.h>
#include <initializer_list>

namespace sge {

class CTransform : public Component {
public:
    sm::Vec2 position;
    sm::Vec2 prevPosition;
    sm::Vec2 velocity;
    sm::Vec2 scale;
    float angle;

    CTransform();
    CTransform(const CTransform &otherTransform);
    CTransform(const sm::Vec2 &position, const sm::Vec2 &velocity, float angle);


    ~CTransform();
    CTransform& operator = (const CTransform &otherTransform);
};

}

#endif // SGE_CTRANSFORM_H