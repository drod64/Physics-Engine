#ifndef SGE_CLIFESPAN_H
#define SGE_CLIFESPAN_H
#include <SGE/components/Component.h>

namespace sge {

class CLifespan : public Component {
public:
    int remaining;
    int total;

    CLifespan();
    CLifespan(int total);
};

} // namespace sge

#endif // SGE_CLIFESPAN_H