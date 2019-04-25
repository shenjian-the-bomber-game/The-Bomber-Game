#include "test.hpp"
using namespace std;

int main() {
    uint8_t *v = new uint8_t[4];
    for (int i = 0; i < 4; i++) {
        v[i] = 1 + i;
    }
    CircularQueue q(10);
    cout << "queue size:\t" << q.size() << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // enqueue test
    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "current_pkg_size:\t" << q.current_packet_size() << endl;

    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // dequeue test
    uint8_t *v1 = new uint8_t[4];
    v1[3] = '\0';

    cout << "dequeue:\t" << q.dequeue(v1, 3) << endl;
    cout << "v1:\t" << v1 << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "dequeue:\t" << q.dequeue(v1, 3) << endl;
    cout << "v1:\t" << v1 << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "dequeue:\t" << q.dequeue(v1, 3) << endl;
    //cout << "v1:\t" << v1 << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    // enqueue again
    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    cout << "enqueue:\t" << q.enqueue(v, 4) << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;
    // dequeue again
    cout << "dequeue:\t" << q.dequeue(v1, 3) << endl;
    cout << "v1:\t" << v1 << endl;
    cout << "queue free:\t" << q.get_num_free_bytes() << endl;

    return 0;
}

/* TESTING PARTS */

CircularQueue::CircularQueue(size_t init_size) {
    _size = init_size;
    data = new uint8_t[_size];
    front = 0;
    rear = 0;   // point to the next of the last
    _num_free_bytes = _size;
}

CircularQueue::~CircularQueue() {
    delete[] data;
}

bool CircularQueue::enqueue(const uint8_t *buf, const size_t size) {
    if (_num_free_bytes < size) {
        //LOG(Debug) << "queue overflow" << endl;
        cerr << "DEBUG: queue overflow" << endl;
        return false;
    }
    for (size_t i = 0; i < size; i++) {
        data[rear] = buf[i];
        rear = (rear + 1) % _size;
        _num_free_bytes -= 1;
    }
    return true;
}

bool CircularQueue::dequeue(uint8_t *buf, const size_t size) {
    if (_num_free_bytes + size > _size) {
        //LOG(Debug) << "queue underflow" << endl;
        cerr << "DEBUG: queue underflow" << endl;
        return false;
    }
    for (size_t i = 0; i < size; i++) {
        buf[i] = data[front];
        front = (front + 1) % _size;
        _num_free_bytes += 1;
    }
    return true;
}

size_t CircularQueue::get_num_free_bytes() {
    return _num_free_bytes;
}

size_t CircularQueue::size() {
    return _size;
}

bool CircularQueue::is_empty() {
    return _num_free_bytes == _size;
}

bool CircularQueue::is_full() {
    return _num_free_bytes == 0;
}

uint16_t CircularQueue::current_packet_size() {
    uint8_t buf[2];
    buf[1] = data[(front+1) % _size];
    buf[0] = data[(front+2) % _size];
    return ntohs(*((uint16_t*)buf));
}
