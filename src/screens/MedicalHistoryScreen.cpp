#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>

#include <source/source.h>
#include <source/PatientManagementSystem.h>
#include <screens/MedicalHistoryScreen.h>
#include <screens/dialog/MedicalHistoryDialog.h>

MedicalHistoryScreen::MedicalHistoryScreen(Source *source, int id, QWidget *parent) : source(source), patient_id(id), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    Patient *patient = source->patientManagementSystem->getPatient(patient_id);
    QHBoxLayout *topBarLayout = new QHBoxLayout;

    QLabel *medicalHistoryInfo = new QLabel(QString("%1 Medical History Details").arg(patient->name));
    medicalHistoryInfo->setStyleSheet("font-weight: bold; font-size: 16px;");
    topBarLayout->addWidget(medicalHistoryInfo);

    topBarLayout->addStretch();

    QPushButton *addBtn = new QPushButton;
    QIcon addIcon = style()->standardIcon(QStyle::SP_FileDialogNewFolder); // You can use a custom "+" icon
    addBtn->setIcon(addIcon);
    addBtn->setToolTip("Add medical history");
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
    table->setHorizontalHeaderLabels({"ID", "Diagnosis", "Prescription", "Date"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    layout->addLayout(topBarLayout);
    layout->addWidget(table);

    refreshTable();

    connect(exportBtn, &QPushButton::clicked, this, &MedicalHistoryScreen::exportMedicalHistory);
    connect(addBtn, &QPushButton::clicked, this, &MedicalHistoryScreen::onMedicalHistoryAdd);
}

void MedicalHistoryScreen::exportMedicalHistory()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Patients"), "test.csv", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "ID,Diagnosis,Prescription,Date\n";

    int len = 0;
    Patient *patient = source->patientManagementSystem->getPatient(patient_id);
    MedicalHistoryNode *medicalHistory = source->patientManagementSystem->getMedicalHistory(patient, &len);

    for (int i = 0; i < len; i++)
    {
        out << medicalHistory[i].id << "," << medicalHistory[i].diagnosis << ","
            << medicalHistory[i].prescription << "," << medicalHistory[i].date << "\n";
    }
    delete[] medicalHistory;
    file.close();
}

void MedicalHistoryScreen::onMedicalHistoryAdd()
{
    Patient *patient = source->patientManagementSystem->getPatient(patient_id);
    MedicalHistoryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        MedicalHistoryNode p = dialog.getFields();
        source->patientManagementSystem->addPatientMedicalHistory(patient->id, p.diagnosis, p.prescription, p.date);
        refreshTable();
    }
}

void MedicalHistoryScreen::refreshTable()
{
    int index = 0;
    table->setRowCount(0);

    int len = 0;
    Patient *patient = source->patientManagementSystem->getPatient(patient_id);
    MedicalHistoryNode *medicalHistory = source->patientManagementSystem->getMedicalHistory(patient, &len);

    for (int i = 0; i < len; i++)
    {
        table->insertRow(index);
        table->setItem(index, 0, new QTableWidgetItem(QString::number(medicalHistory[i].id)));
        table->setItem(index, 1, new QTableWidgetItem(medicalHistory[i].diagnosis));
        table->setItem(index, 2, new QTableWidgetItem(medicalHistory[i].prescription));
        table->setItem(index, 3, new QTableWidgetItem(medicalHistory[i].date));

        index++;
    }

    delete[] medicalHistory;
}
