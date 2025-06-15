#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // Required for fcntl, F_SETFL, O_NONBLOCK
#include <string.h>
#include <errno.h> // Required for errno, EAGAIN, EWOULDBLOCK

int main() {
    int pipe_fds[2];
    #define large_buffer_size (1024 * 1024)
    char large_buffer[large_buffer_size];
    ssize_t bytes_written;

    memset(large_buffer, 'A', large_buffer_size);

    if (pipe(pipe_fds) == -1) {
        perror("pipe creation failed");
        return 1;
    }
    if (fcntl(pipe_fds[1], F_SETFL, O_NONBLOCK) == -1) {
        perror("setting non-blocking mode failed");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return 1;
    }
    printf("attempting to write %d bytes to a non-blocking pipe...\n", large_buffer_size);

    bytes_written = write(pipe_fds[1], large_buffer, large_buffer_size);

    if (bytes_written == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("error: pipe buffer full (EAGAIN/EWOULDBLOCK). this is expected for O_NONBLOCK when the buffer is full.\n");
        } else {
            perror("write failed");
        }
    } else {
        printf("successfully wrote %zd bytes. %d bytes were requested.\n", bytes_written, large_buffer_size);
        if (bytes_written < large_buffer_size) {
            printf("write() returned fewer bytes than requested.\n");
        } else {
            printf("all requested bytes were written\n");
        }
    }

    close(pipe_fds[0]);
    close(pipe_fds[1]);

    return 0;
}
