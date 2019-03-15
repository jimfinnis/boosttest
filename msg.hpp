/**
 * @file msg.h
 * @brief  Brief description of file.
 *
 */

#ifndef __MSG_H
#define __MSG_H

#define MSGSIZE (640*480*2)

struct request {
    int n;
};

struct response {
    // MASSIVE
    char b[MSGSIZE];
};

#endif /* __MSG_H */
