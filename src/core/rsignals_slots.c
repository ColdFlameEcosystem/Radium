#include "rsignals_slots.h"

RSignal *RCreateSignal()
{
    RSignal* signal = (RSignal*)malloc(sizeof(RSignal));
    if (signal == NULL) {
        return NULL;
    }

    signal->Slots = NULL;
    signal->Capacity = 0;
    signal->Size = 0;
    pthread_mutex_init(&signal->Mutex, NULL);

    return signal;
}


void RNukeSignal(RSignal *signal)
{
    if (signal == NULL) {
        return;
    }

    pthread_mutex_lock(&signal->Mutex);
    free(signal->Slots);
    pthread_mutex_unlock(&signal->Mutex);
    pthread_mutex_destroy(&signal->Mutex);
    free(signal);
}

void RConnectSignal(RSignal *signal, void (*slot)(void *), void *data)
{
    pthread_mutex_lock(&signal->Mutex);

    // Increases the Capacity when needed of the signal
    size_t new_capacity = signal->Capacity + 1;
    RSlot* new_slots = (RSlot*)realloc(signal->Slots, sizeof(RSlot) * new_capacity);
    if (new_slots != NULL) {
        signal->Slots = new_slots;
        signal->Capacity = new_capacity;

        signal->Slots[signal->Size].Slot = slot;
        signal->Slots[signal->Size].Data = data;
        signal->Size++;
    }

    pthread_mutex_unlock(&signal->Mutex);
}


void RDisconnectSignal(RSignal *signal, void (*slot)(void *))
{
    pthread_mutex_lock(&signal->Mutex);
    for(size_t i = 0; i < signal->Size; i++) {
        if (signal->Slots[i].Slot == slot) {
            for (size_t j = i; j < signal->Size - 1; j++) {
                signal->Slots[j] = signal->Slots[j+1];
            }
            signal->Size--;
            break;
        }
    }
    pthread_mutex_unlock(&signal->Mutex);
}


void REmitSignal(RSignal *signal)
{
    pthread_mutex_lock(&signal->Mutex);
    for (size_t i = 0; i < signal->Size; i++) {
        signal->Slots[i].Slot(signal->Slots[i].Data);
    }
    pthread_mutex_unlock(&signal->Mutex);
}
