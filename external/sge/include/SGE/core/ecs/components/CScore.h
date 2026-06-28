#ifndef SGE_CSCORE_H
#define SGE_CSCORE_H

namespace sge {
/**
 * Component that stores score information.
 */
class CScore {
public:
    int score = 0;

    /**
     * Parameterized Constructor.
     * @param score the score of the component
     */
    CScore(int score);
};

} // namespace sge

#endif // SGE_CSCORE_H