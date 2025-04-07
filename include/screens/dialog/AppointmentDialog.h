#pragma once

#include <QDialog>
#include <QLineEdit>
#include <source/AppointmentSystem.h>

class AppointmentDialog : public QDialog
{
    Q_OBJECT

    QLineEdit *patientIdInput;

public:
    AppointmentDialog(QWidget *parent = nullptr);
    Appointment getFields() const;
};