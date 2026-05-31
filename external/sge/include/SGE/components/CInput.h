#ifndef SGE_CINPUT_H
#define SGE_CINPUT_H
#include <SGE/components/Component.h>
#include <SGE/util/Bitmask.h>
#include <utility>
#include <vector>

namespace sge {

class CInput : public Component {
private:
    std::vector<Bitmask> m_input_state;

    std::pair<int, int> getIndexes(int action);

public:
    CInput();
    bool getInputState(int action);
    void setInputState(int action, bool value);
};

} // namespace sge

#endif // SGE_CINPUT_H