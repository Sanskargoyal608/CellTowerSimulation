#ifndef CAPACITY_EXCEPTION_H
#define CAPACITY_EXCEPTION_H

class CapacityExceededException {
public:
    const char* message;
    // Simple constructor to store the error message
    CapacityExceededException(const char* msg) : message(msg) {}
};

#endif