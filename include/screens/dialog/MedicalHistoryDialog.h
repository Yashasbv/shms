#pragma once

#include <QDialog>
#include <QLineEdit>
#include <source/PatientManagementSystem.h>

class MedicalHistoryDialog : public QDialog
{
    Q_OBJECT

    QLineEdit *diagnosisInput;
    QLineEdit *prescriptionInput;
    QLineEdit *dateInput;

public:
    MedicalHistoryDialog(QWidget *parent = nullptr);
    MedicalHistoryNode getFields() const;
};