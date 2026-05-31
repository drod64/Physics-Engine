#ifndef SGE_CSCORE_H
#define SGE_CSCORE_H
#include <SGE/components/Component.h>

namespace sge {

class CScore : public Component {
public:
    int score;

    CScore();
    CScore(int score);
};

} // namespace sge

#endif // SGE_CSCORE_H