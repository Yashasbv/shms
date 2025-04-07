#include <QPushButton>
#include <QVBoxLayout>
#include <screens/dialog/MedicalHistoryDialog.h>

MedicalHistoryDialog::MedicalHistoryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add Patient");
    setModal(true); // Prevent background access

    QVBoxLayout *layout = new QVBoxLayout(this);

    diagnosisInput = new QLineEdit(this);
    prescriptionInput = new QLineEdit(this);
    dateInput = new QLineEdit(this);
    
    diagnosisInput->setPlaceholderText("Diagnosis");
    prescriptionInput->setPlaceholderText("Prescription");
    dateInput->setPlaceholderText("Date");
    
    layout->addWidget(diagnosisInput);
    layout->addWidget(prescriptionInput);
    layout->addWidget(dateInput);
   
    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

MedicalHistoryNode MedicalHistoryDialog::getFields() const
{
    MedicalHistoryNode medicalHistoryNode;
    medicalHistoryNode.diagnosis = diagnosisInput->text();
    medicalHistoryNode.prescription = prescriptionInput->text();
    medicalHistoryNode.date = dateInput->text();
    return medicalHistoryNode;
}
