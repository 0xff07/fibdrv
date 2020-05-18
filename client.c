#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define FIB_DEV "/dev/fibdrv"

ssize_t read_wrapper(int fd, void *buf, size_t count)
{
    return read(fd, buf, count);
}

#undef read
#define read read_wrapper

int main()
{
    char buf[BIGUINT_WIDTH >> 3] = {0};
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

#ifdef CLOCK_TIMER
    for (int i = 0; i <= offset; i++) {
        struct timespec start, end;
        off_t err = 0;
        lseek(fd, i, SEEK_SET);
        clock_gettime(CLOCK_MONOTONIC, &start);
        read(fd, buf, BIGUINT_WIDTH >> 3);
        printf("%d, ", i);
        printf("%s", buf);
        clock_gettime(CLOCK_MONOTONIC, &end);
        unsigned long duration = 1000000000 * (end.tv_sec - start.tv_sec) +
                                 (end.tv_nsec - start.tv_nsec);
        printf("%d, %lu\n", i, duration);
    }
#else
    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        read(fd, buf, BIGUINT_WIDTH >> 3);
        printf("Reading from " FIB_DEV " at offset %d, returned the sequence ",
               i);
        printf("%s", buf);
        printf(".\n");
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        read(fd, buf, BIGUINT_WIDTH >> 3);
        printf("Reading from " FIB_DEV " at offset %d, returned the sequence ",
               i);
        printf("%s", buf);
        printf(".\n");
    }
#endif
    close(fd);
    return 0;
}
