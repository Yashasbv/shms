#pragma once

#include <QDialog>
#include <QLineEdit>
#include <source/PatientManagementSystem.h>

class PatientDialog : public QDialog
{
    Q_OBJECT

    QLineEdit *nameInput;
    QLineEdit *ageInput;
    QLineEdit *conditionInput;
    QLineEdit *severityInput;
    QLineEdit *doctorInput;

public:
    PatientDialog(QWidget *parent = nullptr);
    void setFields(Patient *patient);
    Patient getFields() const;
};