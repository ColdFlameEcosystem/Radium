#include <r_signals_slots>

r_signal *r_create_signal()
{
    r_signal* signal = (r_signal*)malloc(sizeof(r_signal));
    if (signal == NULL) {
        return NULL;
    }

    signal->slots = NULL;
    signal->capacity = 0;
    signal->size = 0;
    pthread_mutex_init(&signal->mutex, NULL);

    return signal;
}


void r_destroy_signal(r_signal *signal)
{
    if (signal == NULL) {
        return;
    }

    pthread_mutex_lock(&signal->mutex);
    free(signal->slots);
    pthread_mutex_unlock(&signal->mutex);
    pthread_mutex_destroy(&signal->mutex);
    free(signal);
}

void r_connect_signal(r_signal *signal, void (*slot)(void *), void *data)
{
    pthread_mutex_lock(&signal->mutex);

    // Increases the capacity when needed of the signal
    size_t new_capacity = signal->capacity + 1;
    r_slot* new_slots = (r_slot*)realloc(signal->slots, sizeof(r_slot) * new_capacity);
    if (new_slots != NULL) {
        signal->slots = new_slots;
        signal->capacity = new_capacity;

        signal->slots[signal->size].slot = slot;
        signal->slots[signal->size].data = data;
        signal->size++;
    }

    pthread_mutex_unlock(&signal->mutex);
}


void r_disconnect_signal(r_signal *signal, void (*slot)(void *))
{
    pthread_mutex_lock(&signal->mutex);
    for(size_t i = 0; i < signal->size; i++) {
        if (signal->slots[i].slot == slot) {
            for (size_t j = i; j < signal->size - 1; j++) {
                signal->slots[j] = signal->slots[j+1];
            }
            signal->size--;
            break;
        }
    }
    pthread_mutex_unlock(&signal->mutex);
}


void r_emit_signal(r_signal *signal)
{
    pthread_mutex_lock(&signal->mutex);
    for (size_t i = 0; i < signal->size; i++) {
        signal->slots[i].slot(signal->slots[i].data);
    }
    pthread_mutex_unlock(&signal->mutex);
}
