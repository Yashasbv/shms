#pragma once
#include <QWidget>
#include <QTableWidget>

class Source;

class MedicalHistoryScreen : public QWidget
{
    Q_OBJECT
private:
    Source *source = nullptr;
    QTableWidget *table;
    int patient_id = -1;

    void refreshTable();

private slots:
    void exportMedicalHistory();
    void onMedicalHistoryAdd();

public:
    MedicalHistoryScreen(Source *source, int id, QWidget *parent = nullptr);
};
