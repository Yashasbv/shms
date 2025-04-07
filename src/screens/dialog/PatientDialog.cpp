#include <QPushButton>
#include <QVBoxLayout>
#include <screens/dialog/PatientDialog.h>

PatientDialog::PatientDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add Patient");
    setModal(true); // Prevent background access

    QVBoxLayout *layout = new QVBoxLayout(this);

    nameInput = new QLineEdit(this);
    ageInput = new QLineEdit(this);
    conditionInput = new QLineEdit(this);
    severityInput = new QLineEdit(this);
    doctorInput = new QLineEdit(this);

    nameInput->setPlaceholderText("Name");
    ageInput->setPlaceholderText("Age");
    conditionInput->setPlaceholderText("Condition");
    severityInput->setPlaceholderText("Severity (1-10)");
    doctorInput->setPlaceholderText("Doctor");

    layout->addWidget(nameInput);
    layout->addWidget(ageInput);
    layout->addWidget(conditionInput);
    layout->addWidget(severityInput);
    layout->addWidget(doctorInput);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void PatientDialog::setFields(Patient *patient)
{
    setWindowTitle("Edit Patient");
    nameInput->setText(patient->name);
    ageInput->setText(QString::number(patient->age));
    conditionInput->setText(patient->condition);
    severityInput->setText(QString::number(patient->severity));
    doctorInput->setText(patient->doctor);
}

Patient PatientDialog::getFields() const
{
    Patient patient;
    patient.name = nameInput->text();
    patient.age = ageInput->text().toInt();
    patient.condition = conditionInput->text();
    patient.severity = severityInput->text().toInt();
    patient.doctor = doctorInput->text();
    return patient;
}
