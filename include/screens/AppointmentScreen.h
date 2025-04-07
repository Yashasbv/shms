#pragma once
#include <QWidget>
#include <QTableWidget>

class Source;

class AppointmentScreen : public QWidget
{
    Q_OBJECT
private:
    Source *source = nullptr;
    QTableWidget *table;

    void refreshTable();

private slots:
    void exportAppointment();
    void onAppointmentAdd();
    void onNextAppointment();
    void onAppointmentDelete();

public:
    AppointmentScreen(Source *source, QWidget *parent = nullptr);
};
