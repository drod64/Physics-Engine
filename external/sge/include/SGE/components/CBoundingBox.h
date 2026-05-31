#ifndef SGE_CBOUNDING_BOX
#define SGE_CBOUNDING_BOX
#include <sge/components/Component.h>
#include <SM/Vec2.h>

namespace sge {
class CBoundingBox : public Component {
private:
    sm::Vec2 m_size;
    sm::Vec2 m_halfSize;

public:
    CBoundingBox();
    CBoundingBox(const CBoundingBox &otherBox);
    CBoundingBox(const sm::Vec2 &size);

    sm::Vec2 size() const;
    sm::Vec2 halfSize() const;

    void setSize(const sm::Vec2 &size);

};
}

#endif // SGE_CBOUNDING_BOX