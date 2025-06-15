#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>

int main() {
    const char *filename = "test_file.bin";
    char data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    char buffer[4];
    int fd;
    ssize_t bytes_read;
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open for writing failed");
        return 1;
    }
    if (write(fd, data, sizeof(data)) == -1) {
        perror("write to file failed");
        close(fd);
        return 1;
    }
    close(fd);

    printf("file '%s' created and data written: ", filename);
    for (size_t i = 0; i < sizeof(data); ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open for reading failed");
        unlink(filename);
        return 1;
    }

    off_t seek_result = lseek(fd, 3, SEEK_SET);
    if (seek_result == (off_t)-1) {
        perror("lseek failed");
        close(fd);
        unlink(filename);
        return 1;
    }
    printf("file pointer moved to position: %lld (expected 3)\n", (long long)seek_result);

    bytes_read = read(fd, buffer, 4);
    if (bytes_read == -1) {
        perror("read failed");
        close(fd);
        unlink(filename);
        return 1;
    }
    if (bytes_read < 4) {
        printf("read fewer than 4 bytes. actual bytes read: %zd\n", bytes_read);
    }

    printf("buffer after read() contains: ");
    for (size_t i = 0; i < bytes_read; ++i) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    char expected_buffer[] = {2, 3, 3, 7};
    if (bytes_read == 4 && memcmp(buffer, expected_buffer, 4) == 0) {
        printf("result matches the expected: 2 3 3 7\n");
    } else {
        printf("result does not match the expected.\n");
    }

    close(fd);
    unlink(filename);

    return 0;
}
