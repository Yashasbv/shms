#pragma once

#include <QString>

class Source;

#define MAX_ROOMS 100

struct Room
{
    bool occupied;
    int patientId;
};
typedef struct Room Room;

class RoomManagementSystem
{
private:
    Source *source;
    Room rooms[MAX_ROOMS];

public:
    RoomManagementSystem(Source *source);
    ~RoomManagementSystem();
    bool assignRoom(int patientId);
    void evacuateRoom(int roomNumber);
    int getRoomNumber(int patientId) const;
    Room *getRoomDetails();
};
