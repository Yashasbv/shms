#pragma once

#include <SList.h>

class Source;

struct Emergency
{
    int emergencyId;
    int patientId;
    SLIST_ENTRY(Emergency) next;
};
typedef struct Emergency Emergency;

SLIST_HEAD(EmergencyList, Emergency);

class EmergencySystem
{
private:
    Source *source;
    struct EmergencyList head;

public:
    EmergencySystem(Source *source);
    ~EmergencySystem();
    void scheduleEmergency(int patientId);
    void cancelEmergency(int emergencyId);
    Emergency *viewEmergency(int emergencyId) const;
    Emergency *listAllEmergencies(Emergency *current) const;
    Emergency *getNextEmergency();
};
