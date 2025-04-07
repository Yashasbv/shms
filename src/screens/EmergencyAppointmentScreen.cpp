#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>

#include <source/source.h>
#include <source/PatientManagementSystem.h>
#include <screens/dialog/EmergencyAppointmentDialog.h>
#include <screens/EmergencyAppointmentScreen.h>

EmergencyAppointmentScreen::EmergencyAppointmentScreen(Source *source, QWidget *parent) : source(source), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout;

    QLabel *patientInfo = new QLabel("Emergency Appointment Details");
    patientInfo->setStyleSheet("font-weight: bold; font-size: 16px;");
    topBarLayout->addWidget(patientInfo);

    topBarLayout->addStretch();

    QPushButton *addBtn = new QPushButton;
    QIcon addIcon = style()->standardIcon(QStyle::SP_FileDialogNewFolder); // You can use a custom "+" icon
    addBtn->setIcon(addIcon);
    addBtn->setToolTip("Add Emergency Appointment");
    addBtn->setFlat(true);
    topBarLayout->addWidget(addBtn);

    // Export icon button
    QPushButton *exportBtn = new QPushButton;
    QIcon exportIcon = style()->standardIcon(QStyle::SP_DialogSaveButton); // Looks like a save/download icon
    exportBtn->setIcon(exportIcon);
    exportBtn->setToolTip("Export as CSV");
    exportBtn->setFlat(true);
    topBarLayout->addWidget(exportBtn);

    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({"ID", "Patient Id", "Patient Name", "Delete"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    layout->addLayout(topBarLayout);
    layout->addWidget(table);

    refreshTable();

    connect(exportBtn, &QPushButton::clicked, this, &EmergencyAppointmentScreen::exportEmergencyAppointment);
    connect(addBtn, &QPushButton::clicked, this, &EmergencyAppointmentScreen::onEmergencyAppointmentAdd);
}

void EmergencyAppointmentScreen::exportEmergencyAppointment()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Emergency Appointments"), "test.csv", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "ID,Patient Id,Patient Name\n";

    Emergency *emergency = nullptr;
    while ((emergency = source->emergencySystem->listAllEmergencies(emergency)) != nullptr)
    {
        Patient *patient = source->patientManagementSystem->getPatient(emergency->patientId);
        if (patient)
        {
            out << emergency->emergencyId << "," << emergency->patientId << "," << patient->name << "\n";
        }
    }

    file.close();
}

void EmergencyAppointmentScreen::onEmergencyAppointmentAdd()
{
    EmergencyAppointmentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Emergency a = dialog.getFields();
        source->emergencySystem->scheduleEmergency(a.patientId);
        refreshTable();
    }
}

void EmergencyAppointmentScreen::onNextEmergencyAppointment()
{
}

void EmergencyAppointmentScreen::onEmergencyAppointmentDelete()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt();
    source->emergencySystem->cancelEmergency(id);
    refreshTable();
}

void EmergencyAppointmentScreen::refreshTable()
{
    int index = 0;
    Emergency *appointment = nullptr;
    table->setRowCount(0);

    while ((appointment = source->emergencySystem->listAllEmergencies(appointment)) != nullptr)
    {
        table->insertRow(index);
        table->setItem(index, 0, new QTableWidgetItem(QString::number(appointment->emergencyId)));
        table->setItem(index, 1, new QTableWidgetItem(QString::number(appointment->patientId)));

        Patient *patient = source->patientManagementSystem->getPatient(appointment->patientId);
        if (patient)
        {
            table->setItem(index, 2, new QTableWidgetItem(patient->name));
        }

        // Delete icon button
        QPushButton *deleteBtn = new QPushButton;
        QIcon deleteIcon = style()->standardIcon(QStyle::SP_TrashIcon);
        deleteBtn->setIcon(deleteIcon);
        deleteBtn->setFlat(true);
        table->setCellWidget(index, 3, deleteBtn);

        index++;

        connect(deleteBtn, &QPushButton::clicked, this, &EmergencyAppointmentScreen::onEmergencyAppointmentDelete);
    }
}
