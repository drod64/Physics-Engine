#ifndef CDRAG3_H
#define CDRAG3_H
#include <SM/Precision.h>

namespace sge {
class CDrag3 {
public:
    sm::real k1;
    sm::real k2;

    /**
     * Parameterized constructor.
     * @param k1 the linear constant (dominates at low speeds)
     * @param k2 the quadratic constant (dominates at high speeds)
     */
    CDrag3(sm::real k1, sm::real k2)
    {
        this->k1 = k1;
        this->k2 = k2;
    }
}; // class CDrag3
} // namespace sge

#endif // CDRAG3_H