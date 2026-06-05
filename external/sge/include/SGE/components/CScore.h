#ifndef SGE_CSCORE_H
#define SGE_CSCORE_H
#include <SGE/components/Component.h>

namespace sge {
/**
 * Component that stores score information.
 */
class CScore : public Component {
public:
    int score;

    /**
     * Default Constructor.
     */
    CScore();

    /**
     * Parameterized Constructor.
     * @param score the score of the component
     */
    CScore(int score);
};

} // namespace sge

#endif // SGE_CSCORE_H