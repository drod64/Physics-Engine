#ifndef SGE_INPUT_SNAPSHOT_H
#define SGE_INPUT_SNAPSHOT_H
#include <assert.h>
#include <vector>
#include <sm/Vec2.h>
#include <SGE/util/Bitmask.h>

namespace sge {
class InputSnapshot {
private:
    std::vector<Bitmask> m_keyButtons;
    std::vector<Bitmask> m_mouseButtons;

public:
    static const int INPUT_SIZE;
    sm::Vec2 mouseDelta;
    sm::Vec2 mousePosition;

    InputSnapshot();
    InputSnapshot(const InputSnapshot &otherInput);

    InputSnapshot& operator = (const InputSnapshot &otherInput);

    void setKeyButton(int keyButton, bool value);
    void setMouseButton(int mouseButton, bool value);

    bool getKeyButtonState(int keyButton) const;
    bool getMouseButtonState(int mouseButton) const;

};
}

#endif // SGE_INPUT_SNAPSHOT_H