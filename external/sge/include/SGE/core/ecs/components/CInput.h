#ifndef SGE_CINPUT_H
#define SGE_CINPUT_H
#include <SGE/util/Bitmask.h>
#include <utility>
#include <vector>

namespace sge {
/**
 * Component that stores and updates input data.
 */
class CInput {
private:
    std::vector<Bitmask> m_input_state;
    std::pair<int, int> getIndexes(int action);

public:
    /**
     * Default Constructor.
     */
    CInput();

    /**
     * Retrives the current state of input.
     * @param index  the index to be retrieve from the input state
     * @return the value stored at the requested index
     */
    bool getInputState(int index);

    /**
     * Sets the index of the input to a particular value.
     * @param index the index position to update
     * @param value true or false
     */
    void setInputState(int index, bool value);
};

} // namespace sge

#endif // SGE_CINPUT_H