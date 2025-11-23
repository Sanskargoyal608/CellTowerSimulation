#ifndef USER_DEVICE_H
#define USER_DEVICE_H

class UserDevice {
private:
    int id;

public:
    UserDevice(int uid) : id(uid) {}
    int getId() const { return id; }
    virtual ~UserDevice() {}
};

#endif