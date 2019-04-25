#include <iostream>
#include <vector>
#include <stdint.h>
#include <arpa/inet.h>

class CircularQueue {

public:
    CircularQueue(size_t init_size);
    ~CircularQueue();

    bool enqueue(const uint8_t *buf, const size_t size);
    bool dequeue(uint8_t *buf, const size_t size);
    uint16_t current_packet_size();

    // Also requires a getter method for _num_free_bytes here.
    size_t get_num_free_bytes();
    size_t size();
    bool is_empty();
    bool is_full();

private:
    size_t _size;
    uint8_t *data;
    size_t _num_free_bytes;
    size_t front, rear;
};
