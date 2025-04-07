#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>

#include <source/source.h>
#include <source/PatientManagementSystem.h>
#include <screens/dialog/AppointmentDialog.h>
#include <screens/AppointmentScreen.h>

AppointmentScreen::AppointmentScreen(Source *source, QWidget *parent) : source(source), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout;

    QLabel *patientInfo = new QLabel("Appointment Details");
    patientInfo->setStyleSheet("font-weight: bold; font-size: 16px;");
    topBarLayout->addWidget(patientInfo);

    topBarLayout->addStretch();

    QPushButton *addBtn = new QPushButton;
    QIcon addIcon = style()->standardIcon(QStyle::SP_FileDialogNewFolder); // You can use a custom "+" icon
    addBtn->setIcon(addIcon);
    addBtn->setToolTip("Add Appointment");
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

    connect(exportBtn, &QPushButton::clicked, this, &AppointmentScreen::exportAppointment);
    connect(addBtn, &QPushButton::clicked, this, &AppointmentScreen::onAppointmentAdd);
}

void AppointmentScreen::exportAppointment()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Appointments"), "test.csv", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "ID,Patient Id,Patient Name\n";

    Appointment *appointment = nullptr;
    while ((appointment = source->appointmentSystem->listAllAppointments(appointment)) != nullptr)
    {
        Patient *patient = source->patientManagementSystem->getPatient(appointment->patientId);
        if (patient)
        {
            out << appointment->appointmentId << "," << appointment->patientId << "," << patient->name << "\n";
        }
    }

    file.close();
}

void AppointmentScreen::onAppointmentAdd()
{
    AppointmentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Appointment a = dialog.getFields();
        source->appointmentSystem->scheduleAppointment(a.patientId);
        refreshTable();
    }
}

void AppointmentScreen::onNextAppointment()
{
}

void AppointmentScreen::onAppointmentDelete()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt();
    source->appointmentSystem->cancelAppointment(id);
    refreshTable();
}

void AppointmentScreen::refreshTable()
{
    int index = 0;
    Appointment *appointment = nullptr;
    table->setRowCount(0);

    while ((appointment = source->appointmentSystem->listAllAppointments(appointment)) != nullptr)
    {
        table->insertRow(index);
        table->setItem(index, 0, new QTableWidgetItem(QString::number(appointment->appointmentId)));
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

        connect(deleteBtn, &QPushButton::clicked, this, &AppointmentScreen::onAppointmentDelete);
    }
}
