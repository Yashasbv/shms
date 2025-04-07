#pragma once

#include <SList.h>

class Source;

struct Appointment
{
    int appointmentId;
    int patientId;
    SLIST_ENTRY(Appointment) next;
};
typedef struct Appointment Appointment;

SLIST_HEAD(AppointmentList, Appointment);

class AppointmentSystem
{
private:
    Source *source;
    struct AppointmentList head;

public:
    AppointmentSystem(Source *source);
    ~AppointmentSystem();
    void scheduleAppointment(int patientId);
    void cancelAppointment(int appointmentId);
    Appointment *viewAppointment(int appointmentId) const;
    Appointment *listAllAppointments(Appointment *current) const;
    Appointment *getNextAppointment();
};
