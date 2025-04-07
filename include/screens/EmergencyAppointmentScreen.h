#pragma once
#include <QWidget>
#include <QTableWidget>

class Source;

class EmergencyAppointmentScreen : public QWidget
{
    Q_OBJECT
private:
    Source *source = nullptr;
    QTableWidget *table;

    void refreshTable();

private slots:
    void exportEmergencyAppointment();
    void onEmergencyAppointmentAdd();
    void onNextEmergencyAppointment();
    void onEmergencyAppointmentDelete();

public:
    EmergencyAppointmentScreen(Source *source, QWidget *parent = nullptr);
};
