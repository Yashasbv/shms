#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>

#include <source/source.h>
#include <screens/dialog/PatientDialog.h>
#include <screens/MedicalHistoryScreen.h>
#include <screens/PatientScreen.h>
#include <Mainwindow.h>

PatientScreen::PatientScreen(Source *source, MainWindow *mainWindow, QWidget *parent) : source(source), mainWindow(mainWindow), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout;

    QLabel *patientInfo = new QLabel("Patient Details");
    patientInfo->setStyleSheet("font-weight: bold; font-size: 16px;");
    topBarLayout->addWidget(patientInfo);

    topBarLayout->addStretch();

    QPushButton *addBtn = new QPushButton;
    QIcon addIcon = style()->standardIcon(QStyle::SP_FileDialogNewFolder); // You can use a custom "+" icon
    addBtn->setIcon(addIcon);
    addBtn->setToolTip("Add Patient");
    addBtn->setFlat(true);
    topBarLayout->addWidget(addBtn);

    // Export icon button
    QPushButton *exportBtn = new QPushButton;
    QIcon exportIcon = style()->standardIcon(QStyle::SP_DialogSaveButton); // Looks like a save/download icon
    exportBtn->setIcon(exportIcon);
    exportBtn->setToolTip("Export as CSV");
    exportBtn->setFlat(true);
    topBarLayout->addWidget(exportBtn);

    table = new QTableWidget(0, 9);
    table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Condition", "Severity", "Doctor", "History", "Edit", "Delete"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    layout->addLayout(topBarLayout);
    layout->addWidget(table);

    refreshTable();

    connect(exportBtn, &QPushButton::clicked, this, &PatientScreen::exportPatient);
    connect(addBtn, &QPushButton::clicked, this, &PatientScreen::onPatientAdd);
}

void PatientScreen::exportPatient()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Patients"), "test.csv", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "ID,Name,Age,Condition,Severity,Doctor\n";

    Patient *patient = nullptr;
    while ((patient = source->patientManagementSystem->listAllPatients(patient)) != nullptr)
    {
        out << patient->id << "," << patient->name << "," << patient->age << ","
            << patient->condition << "," << patient->severity << "," << patient->doctor << "\n";
    }

    file.close();
}

void PatientScreen::onPatientAdd()
{
    PatientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Patient p = dialog.getFields();
        source->patientManagementSystem->addPatient(p.name, p.age, p.condition, p.severity, p.doctor);
        refreshTable();
    }
}

void PatientScreen::medicalHistoryScreen()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt();

    mainWindow->setContent(new MedicalHistoryScreen(source, id, mainWindow->contentArea));
}

void PatientScreen::onPatientEdit()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt();
    Patient *patient = source->patientManagementSystem->getPatient(id);
    if (!patient)
        return;

    PatientDialog dialog(this);
    dialog.setFields(patient);
    if (dialog.exec() == QDialog::Accepted)
    {
        Patient p = dialog.getFields();
        source->patientManagementSystem->updatePatient(id, p.name, p.age, p.condition, p.severity, p.doctor);
        refreshTable();
    }
}

void PatientScreen::onPatientDelete()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt();
    source->patientManagementSystem->removePatient(id);
    refreshTable();
}

void PatientScreen::refreshTable()
{
    int index = 0;
    Patient *patient = nullptr;
    table->setRowCount(0);

    while ((patient = source->patientManagementSystem->listAllPatients(patient)) != nullptr)
    {
        table->insertRow(index);
        table->setItem(index, 0, new QTableWidgetItem(QString::number(patient->id)));
        table->setItem(index, 1, new QTableWidgetItem(patient->name));
        table->setItem(index, 2, new QTableWidgetItem(QString::number(patient->age)));
        table->setItem(index, 3, new QTableWidgetItem(patient->condition));
        table->setItem(index, 4, new QTableWidgetItem(QString::number(patient->severity)));
        table->setItem(index, 5, new QTableWidgetItem(patient->doctor));

        // Medical history icon button
        QPushButton *medicalHistoryBtn = new QPushButton;
        QIcon medicalHistoryIcon = style()->standardIcon(QStyle::SP_FileDialogStart);
        medicalHistoryBtn->setIcon(medicalHistoryIcon);
        medicalHistoryBtn->setFlat(true);
        table->setCellWidget(index, 6, medicalHistoryBtn);

        // Edit icon button
        QPushButton *editBtn = new QPushButton;
        QIcon editIcon = style()->standardIcon(QStyle::SP_FileDialogContentsView);
        editBtn->setIcon(editIcon);
        editBtn->setFlat(true);
        table->setCellWidget(index, 7, editBtn);

        // Delete icon button
        QPushButton *deleteBtn = new QPushButton;
        QIcon deleteIcon = style()->standardIcon(QStyle::SP_TrashIcon);
        deleteBtn->setIcon(deleteIcon);
        deleteBtn->setFlat(true);
        table->setCellWidget(index, 8, deleteBtn);

        index++;

        connect(medicalHistoryBtn, &QPushButton::clicked, this, &PatientScreen::medicalHistoryScreen);
        connect(editBtn, &QPushButton::clicked, this, &PatientScreen::onPatientEdit);
        connect(deleteBtn, &QPushButton::clicked, this, &PatientScreen::onPatientDelete);
    }
}
