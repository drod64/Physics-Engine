#ifndef SGE_COMPONENT_ID_COUNTER_H
#define SGE_COMPONENT_ID_COUNTER_H
#include <bitset>

namespace sge {
    constexpr size_t MAX_COMPONENTS = 100;
    using ComponentMask = std::bitset<MAX_COMPONENTS>;
    
    // 1. Unique IDs for every Component type.
    using ComponentID = uint32_t;

    struct ComponentIDCounter {
    private:
        /**
         * Helper function to keep IDs unique.
         * For every different Component type, this function will be called.
         */
        static inline ComponentID nextID()
        {
            static ComponentID counter = 0;
            return counter++;
        }

    public:
        /**
         * Fetches a unique ID based on T.
         * @tparam the Component data type
         * @return a unique ID based on the Component data type. Will return the same unique ID for Components of the same type
         */
        template <typename T>
        static ComponentID get()
        {
            static const ComponentID ID = nextID();
            return ID;
        }
    }; // class ComponentIDCounter
}

#endif // SGE_COMPONENT_ID_COUNTER_H