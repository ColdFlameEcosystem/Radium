#pragma once
#ifndef R_SIGNAL_SLOT_H
#define R_SIGNAL_SLOT_H

#include <stdlib.h>
#include <pthread.h>

/**
 * @brief Structure representing a slot in a signal.
 */
typedef struct {
    void (*Slot)(void*); /**< Pointer to the slot function */
    void* Data; /**< Data to be passed to the slot function */
} RSlot;

/**
 * @brief Structure representing a signal.
 */
typedef struct {
    RSlot* Slots; /**< Array of slots */
    size_t Capacity; /**< Capacity of the slots array */
    size_t Size; /**< Number of slots in the array */
    pthread_mutex_t Mutex; /**< Mutex for thread safety */
} RSignal;

/**
 * @brief Creates a new signal.
 * @return A pointer to the newly created signal.
 */
RSignal* RCreateSignal();

/**
 * @brief Destroys a Signal.
 * @param Signal Pointer to the Signal to destroy.
 */
void RNukeSignal(RSignal* Signal);

/**
 * @brief Connects a slot function to a Signal.
 * @param Signal Pointer to the Signal.
 * @param function Pointer to the slot function.
 * @param Data Data to be passed to the slot function.
 */
void RConnectSignal(RSignal* Signal, void (*Slot)(void*), void* Data);

/**
 * @brief Disconnects a slot function from a signal.
 * @param Signal Pointer to the signal.
 * @param Function Pointer to the slot function.
 */
void RDisconnectSignal(RSignal* Signal, void (*Slot)(void*));

/**
 * @brief Emits a signal, calling all connected slot functions.
 * @param Signal Pointer to the signal to emit.
 */
void REmitSignal(RSignal* Signal);

#endif /* R_SIGNAL_SLOT_H */
