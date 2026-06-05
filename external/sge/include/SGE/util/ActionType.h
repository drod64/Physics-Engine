#ifndef SGE_ACTION_TYPE_H
#define SGE_ACTION_TYPE_H

namespace sge {
/**
 * Enums which represent what type an action is (start, held, end).
 */    
enum class ActionType {START, HELD, END, NONE};
}

#endif // SGE_ACTION_TYPE_H