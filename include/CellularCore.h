#ifndef CELLULAR_CORE_H
#define CELLULAR_CORE_H

class CellularCore {
private:
    long max_messages;
    long current_load;

public:
    CellularCore(long limit) : max_messages(limit), current_load(0) {}

    bool addMessageLoad(int count) {
        if (current_load + count > max_messages) {
            return false;
        }
        current_load += count;
        return true;
    }

    long getCurrentLoad() const { return current_load; }
    void reset() { current_load = 0; }
};

#endif