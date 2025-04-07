#include <QPushButton>
#include <QVBoxLayout>
#include <screens/dialog/EmergencyAppointmentDialog.h>

EmergencyAppointmentDialog::EmergencyAppointmentDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Add Emergency Appointment");
    setModal(true); // Prevent background access

    QVBoxLayout *layout = new QVBoxLayout(this);

    patientIdInput = new QLineEdit(this);
    patientIdInput->setPlaceholderText("Patient ID");
    layout->addWidget(patientIdInput);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

Emergency EmergencyAppointmentDialog::getFields() const
{
    Emergency appointment;
    appointment.patientId = patientIdInput->text().toInt();
    return appointment;
}
