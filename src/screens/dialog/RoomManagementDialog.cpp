#include <QPushButton>
#include <QVBoxLayout>
#include <screens/dialog/RoomManagementDialog.h>

RoomManagementDialog::RoomManagementDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Book Room");
    setModal(true); // Prevent background access

    QVBoxLayout *layout = new QVBoxLayout(this);

    patientIdInput = new QLineEdit(this);
    patientIdInput->setPlaceholderText("Patient ID");
    layout->addWidget(patientIdInput);

    QPushButton *saveButton = new QPushButton("Book", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

Room RoomManagementDialog::getFields() const
{
    Room room;
    room.patientId = patientIdInput->text().toInt();
    return room;
}
