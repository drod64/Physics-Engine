#ifndef CDRAG3_H
#define CDRAG3_H
#include <SM/Precision.h>

namespace sge {
class CDrag3 {
public:
    sm::real linearK1;
    sm::real linearK2;
    sm::real angularK1;
    sm::real angularK2;

    /**
     * Parameterized constructor.
     * @param linearK1 the linear constant (dominates at low speeds)
     * @param linearK2 the quadratic constant (dominates at high speeds)
     */
    CDrag3(sm::real linearK1, sm::real linearK2, sm::real angularK1, sm::real angularK2)
    {
        this->linearK1 = linearK2;
        this->linearK2 = linearK2;
        this->angularK1 = angularK1;
        this->angularK2 = angularK2;
    }
}; // class CDrag3
} // namespace sge

#endif // CDRAG3_H