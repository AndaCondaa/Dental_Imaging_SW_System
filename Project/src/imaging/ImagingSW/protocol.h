#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_PKT 1024

typedef enum { dat, ack, nak } Type;

typedef struct {
    Type type;
    unsigned int sequence;
    char *data;
} Packet;

#endif // PROTOCOL_H
