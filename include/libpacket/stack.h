/* 
 This file is part of libpacket.

 libpacket is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libpacket is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libpacket.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef __LIBPACKET_STACK
#define __LIBPACKET_STACK

/**
 * @file stack.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implementing a stack and its elements.
 */

/*--------------------------------- StackItem_t -----------------------------*/

/**
 * @class StackItem "libpacket/stack.h"
 * @brief Class implementing the elements of a stack.
 *
 * This class is designed to be extended by other classes when they want to be
 * part of a stack.
 *
 * The specialization is done by adding a member of type StackItem_t to the
 * extending class (i.e. by adding a field of type StackItem_t to the struct).
 * You should not declare variables of type StackItem_t, instead you should
 * always declare pointers to them and call an initialization method prior
 * using them.
 */
typedef struct StackItem StackItem_t;

/* The Stack_t implements a simple double-linked list, hence the prev and next
 * pointers.
 *
 * The owner pointer should point to an instance of an extending class.
 * Protocol (implemented in packet.h) would be an example.
 */
struct StackItem {
    StackItem_t *prev;
    StackItem_t *next;
    void *owner;
};

/**
 * @memberof StackItem
 *
 * StackItem_t constructor with parameters.
 *
 * @param prev The previous positional element to this one.
 * @param next The next positional element to this one.
 * @param owner A pointer to the instance who owns this StackItem_t, namely
 * the class that extends StackItem_t.
 * @return A pointer to the new StackItem_t instance or NULL.
 */
StackItem_t * StackItem_createWithParams(
        StackItem_t *prev,
        StackItem_t *next,
        void *owner);

/**
 * @memberof StackItem
 * 
 * Constructor with default parameters.
 *
 * This constructor returns a standalone StackItem_t instance, with prev and
 * next pointers pointing to NULL and nobody owns it.
 *
 * @return A pointer to the new StackItem_t instance or NULL.
 */
StackItem_t * StackItem_create(void);

/**
 * @memberof StackItem
 *
 * Frees the memory allocated by the constructors.
 */
void StackItem_delete(StackItem_t *item);

/**
 * @memberof StackItem
 *
 * Sets the owner of the instance.
 *
 * @param item The StackItem_t to change its owner.
 * @param owner A pointer to the instance that will own this StackItem_t.
 * @return 0 on success, -1 otherwise.
 */
int StackItem_setOwner(StackItem_t *item, void *owner);

/**
 * @memberof StackItem
 * 
 * Gets the owner of the instance.
 *
 * @param item The StackItem_t to get its owner.
 * @return A pointer to an instance of a class that extends StackItem_t or
 * NULL. Note that it would be convenient to immediately cast the
 * returned pointer to whatever type it should be (as in malloc).
 */
void * StackItem_getOwner(const StackItem_t *item);

/*---------------------------------- Stack_t --------------------------------*/

/**
 * @class Stack "libpacket/stack.h"
 * @brief Class implementing a simple stack.
 * 
 * This class is designed to be extended by other classes. The specialization
 * is done by adding a member of type Stack_t to the extending class (i.e. by
 * adding a field of type Stack_t to the struct).
 *
 * You should not declare variables of type Stack_t, instead you should
 * always declare pointers to them and call a class constructor prior
 * using them.
 */
typedef struct Stack Stack_t;

/* A Stack_t is list of double-linked StackItem_t. The bottom pointer points
 * to the first element of the stack and top points to the last element of the
 * stack. Here we understand "first" as "first inserted".
 *
 * The member items is the current number of elements in the stack and
 * owner is a pointer to an intance of an extending class that owns this stack.
 */
typedef struct Stack {
    StackItem_t *bottom;
    StackItem_t *top;
    unsigned int items;
    void *owner;
} Stack_t;

/**
 * @memberof Stack
 *
 * Stack_t constructor with parameters.
 *
 * Please note that this method accepts the number of items in the stack. One
 * could create a stack in an unstable state and that's valid. Use it wisely.
 *
 * @param bottom A pointer to the first element of the stack.
 * @param top A pointer to the last element of the stack.
 * @param items The number of items in the stack.
 * @owner A pointer to an instance of an extending class that owns this stack.
 * @return A pointer to the newly allocated stack or NULL.
 */
Stack_t * Stack_createWithParams(
        StackItem_t *bottom,
        StackItem_t *top,
        unsigned int items,
        void *owner);

/**
 * @memberof Stack
 * 
 * The stack returned by this methos is empty and is owned by nobody.
 *
 * @return A pointer to the newly allocated Stack or NULL.
 */
Stack_t * Stack_create(void);

/**
 * @memberof Stack
 *
 * Frees the memory allocated by a previous call of a class constructor.
 *
 * @param stack The stack to be freed.
 */
void Stack_delete(Stack_t *stack);

/**
 * @memberof Stack
 *
 * Pushes an item onto the stack.
 *
 * @param stack The stack where to insert the item.
 * @param item The item to be pushed onto the stack.
 * @return 0 on success, -1 otherwise.
 */
int Stack_push(Stack_t *stack, StackItem_t *item);

/**
 * @memberof Stack
 *
 * Pops an item out from the stack.
 *
 * @param stack The stack from where to pop the item out.
 * @return A pointer to the popped item.
 */
StackItem_t * Stack_pop(Stack_t *stack);

/**
 * @memberof Stack
 *
 * Returns the number of items currently hold by the stack.
 *
 * @param stack The stack that we want to know the number of items.
 * @return The numbers of items holds by the stack. If stack is NULL, 0 is
 * returned. Note that 0 can be returned either because stack was NULL or
 * because the stack is currently empty.
 */
unsigned int Stack_numItems(const Stack_t *stack);

/**
 * @memberof Stack
 *
 * Checks if an item is in the stack.
 *
 * @param stack The stack where to check if the item exists.
 * @param item The item we want to check if it exists in the stack.
 * @return Boolean method. Returns 0 if the item doesn't exist in that stack
 * or non-zero if it exists.
 */
int Stack_itemExists(const Stack_t *stack, StackItem_t *item);

/**
 * @memberof Stack
 *
 * Sets the owner of a stack.
 *
 * @param stack The stack to set the owner.
 * @param owner A pointer to an instance of an extending class that owns this
 * stack.
 * @return 0 on success, -1 otherwise.
 */
int Stack_setOwner(Stack_t *stack, void *owner);

/**
 * @memberof Stack
 *
 * Gets the owner of a stack.
 *
 * @param stack The stack to get to owner from.
 * @return A pointer to an instance of an extending class that owns this
 * stack.
 */
void * Stack_getOwner(const Stack_t *stack);

#endif
