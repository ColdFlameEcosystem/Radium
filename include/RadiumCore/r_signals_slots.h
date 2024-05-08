#ifndef R_SIGNAL_SLOT_H
#define R_SIGNAL_SLOT_H

#include <stdlib.h>
#include <pthread.h>

/**
 * @brief Structure representing a slot in a signal.
 */
typedef struct {
    void (*slot)(void*); /**< Pointer to the slot function */
    void* data; /**< Data to be passed to the slot function */
} r_slot;

/**
 * @brief Structure representing a signal.
 */
typedef struct {
    r_slot* slots; /**< Array of slots */
    size_t capacity; /**< Capacity of the slots array */
    size_t size; /**< Number of slots in the array */
    pthread_mutex_t mutex; /**< Mutex for thread safety */
} r_signal;

/**
 * @brief Creates a new signal.
 * @return A pointer to the newly created signal.
 */
r_signal* r_create_signal();

/**
 * @brief Destroys a signal.
 * @param signal Pointer to the signal to destroy.
 */
void r_destroy_signal(r_signal* signal);

/**
 * @brief Connects a slot function to a signal.
 * @param signal Pointer to the signal.
 * @param function Pointer to the slot function.
 * @param data Data to be passed to the slot function.
 */
void r_connect_signal(r_signal* signal, void (*slot)(void*), void* data);

/**
 * @brief Disconnects a slot function from a signal.
 * @param signal Pointer to the signal.
 * @param function Pointer to the slot function.
 */
void r_disconnect_signal(r_signal* signal, void (*slot)(void*));

/**
 * @brief Emits a signal, calling all connected slot functions.
 * @param signal Pointer to the signal to emit.
 */
void r_emit_signal(r_signal* signal);

#endif /* R_SIGNAL_SLOT_H */
