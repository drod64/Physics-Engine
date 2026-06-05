#ifndef SGE_BITMASK_H
#define SGE_BITMASK_H
#include <cstdint>


namespace sge {
/**
 * This is a simple Bitmask class.
 * Useful to represent if a particular key or button is pressed.
 */
class Bitmask {
private:
    uint64_t _mask;

public:
    static const int BIT_SIZE;
    /**
     * Default constructor. Sets the _mask to 0.
     */
    Bitmask();

    /**
     * Copy constructor. Constructs a new Bitmask object whose bit states are copied from the otherBitmask.
     * @param otherBitmask the other bitmask to copy from
     */
    Bitmask(const Bitmask& otherBitmask);

    /**
     * Copies the value of another Bitmask object
     * @param otherBitmask the other bitmask to copy from
     */
    void setBitmask(const Bitmask& otherBitmask);

    /**
     * Sets a bit on at a desired index. The index should be from the range 0 to 31.
     * Indexes that fall out of this range may cause undefined behavior.
     * @param index the index of the bit to turn on
     */
    void setBitOn(int index);

    /**
     * Queries the _renderStates of a bit at a particular index. The index should be from the range 0 to 31.
     * Indexes that fall out of this range may cause undefined behavior.
     * @param index the index of the bit to query
     * @return true if the bit is on, false if the bit is off
     */
    bool getBit(int index) const;

    /**
     * Sets a bit off at a desired index. The index should be from the range 0 to 31.
     * Indexes that fall out of this range may cause undefined behavior.
     * @param index the index of the bit to turn on.
     */
    void setBitOff(int index);

    /**
     * Sets a bit at a particular on or off.
     * @param index the index of the bit to modify
     * @param on boolean representing if the bit is to be set on/off
     */
    void setBit(int index, bool on);

    /**
     * Overloaded assignment operator. Copies the bit states of the otherBitmask.
     * @param otherBitmask the other bitmask to copy
     * @return *this
     */
    Bitmask& operator = (const Bitmask& otherBitmask);

    /**
     * Overloaded AND operator. Performs a & operation between the invoking Bitmask object and the otherBitmask object.
     * @param otherBitmask the bitmask which will be on the right side of the &
     * @return the result of the & operation
     */
    Bitmask operator & (const Bitmask& otherBitmask) const;

    /**
     * Overloaded XOR operator. Performs a ^ operation between the invoking Bitmask object and the otherBitmask object.
     * @param otherBitmask the bitmask which will be on the right side of the ^
     * @return the result of the ^ operation
     */
    Bitmask operator ^ (const Bitmask& otherBitmask) const;

    /**
     * Overloaded OR operator. Performs a | operation between the invoking Bitmask object and the otherBitmask object.
     * @param otherBitmask the bitmask which will be on the right side of the |
     * @return the result of the | operation
     */
    Bitmask operator | (const Bitmask& otherBitmask) const;

    /**
     * Overloaded NOT operator. Performs a ~ operation on the invoking Bitmask object.
     * @return the result of the ~ operation
     */
    Bitmask operator ~ () const;
};
} // namespace sge

#endif //SGE_BITMASK_H