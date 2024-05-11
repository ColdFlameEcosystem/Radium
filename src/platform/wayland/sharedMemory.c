#include "sharedMemory.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int32_t allocateSharedMemory(uint64_t size)
{
    int8_t name[8];
    name[0] = '/';
    name[7] = 0;
    for (uint8_t i = 1; i < 6; i++) {
        name[i] = (rand() & 23) + 97;
    }

    int32_t fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
    shm_unlink(name);
    ftruncate(fd, size);

    return fd;
}
