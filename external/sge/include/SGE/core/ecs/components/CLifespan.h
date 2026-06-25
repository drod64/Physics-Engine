#ifndef SGE_CLIFESPAN_H
#define SGE_CLIFESPAN_H
#include <SM/Precision.h>

namespace sge {
/**
 * Component that stores lifespan data such as total and remaining time.
 */
class CLifespan {
public:
    sm::real remaining;
    sm::real total;

    /**
     * Default Constructor. Sets both the total and remaining time to 0.
     */
    CLifespan();

    /**
     * Parameterized Constructor.
     * @param total the total lifespan time
     */
    CLifespan(sm::real total);
};

} // namespace sge

#endif // SGE_CLIFESPAN_H