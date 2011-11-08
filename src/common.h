#ifndef COMMON_H
#define COMMON_H

    #include <cstring>
    #include <unistd.h>
    #include <errno.h>

    ssize_t writen( int fd, const void *vptr, size_t n );


#endif //COMMON_H
