#ifndef SGE_CBOUNDING_BOX
#define SGE_CBOUNDING_BOX
#include <SM/Vec2.h>

namespace sge {
/**
 * Component that stores bounding box information such as its size (width and length).
 */
class CBoundingBox {
private:
    sm::Vec2 m_size;
    sm::Vec2 m_halfSize;

public:
    /**
     * Default constructor. Creates the component with size = {0, 0}.
     */
    CBoundingBox();

    /**
     * Copy Constructor. Copies the size of another CboundingBox.
     * @param otherBox the other CBoundingBox to copy from
     */
    CBoundingBox(const CBoundingBox &otherBox);

    /**
     * Parameterized Constructor. Constructs a bounding box with the passed in size.
     * @param size the size to construct the CBoundingBox with
     */
    CBoundingBox(const sm::Vec2 &size);

    /**
     * Retrieves the size of the CBoundingBox.
     * @return the size of the bounding box
     */
    sm::Vec2 size() const;

    /**
     * Retrieves the half-size of the CBoundingBox.
     * @return the half-size of the bounding box
     */
    sm::Vec2 halfSize() const;

    /**
     * Sets the size of the CBoundingBox.
     * @param size the new size of the bounding box
     */
    void setSize(const sm::Vec2 &size);

};
}

#endif // SGE_CBOUNDING_BOX