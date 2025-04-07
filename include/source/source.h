#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QVariant>

class PatientManagementSystem;
class AppointmentSystem;
class EmergencySystem;
class RoomManagementSystem;

class Source
{
private:
    QSqlDatabase db;
    QSqlQuery *query = nullptr;

public:
    // Systems
    PatientManagementSystem *patientManagementSystem = nullptr;
    AppointmentSystem *appointmentSystem = nullptr;
    EmergencySystem *emergencySystem = nullptr;
    RoomManagementSystem *roomManagementSystem = nullptr;

    Source();
    ~Source();
    QSqlQuery *getQuery() { return query; }
};
