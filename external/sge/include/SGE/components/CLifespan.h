#ifndef SGE_CLIFESPAN_H
#define SGE_CLIFESPAN_H
#include <SGE/components/Component.h>

namespace sge {
/**
 * Component that stores lifespan data such as total and remaining time.
 */
class CLifespan : public Component {
public:
    int remaining;
    int total;

    /**
     * Default Constructor. Sets both the total and remaining time to 0.
     */
    CLifespan();

    /**
     * Parameterized Constructor.
     * @param total the total lifespan time
     */
    CLifespan(int total);
};

} // namespace sge

#endif // SGE_CLIFESPAN_H