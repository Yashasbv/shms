#pragma once

#include <QDialog>
#include <QLineEdit>
#include <source/EmergencySystem.h>

class EmergencyAppointmentDialog : public QDialog
{
    Q_OBJECT

    QLineEdit *patientIdInput;

public:
    EmergencyAppointmentDialog(QWidget *parent = nullptr);
    Emergency getFields() const;
};
