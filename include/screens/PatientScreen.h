#pragma once
#include <QWidget>
#include <QTableWidget>

class Source;
class MainWindow;

class PatientScreen : public QWidget
{
    Q_OBJECT
private:
    Source *source = nullptr;
    QTableWidget *table;
    MainWindow *mainWindow = nullptr;

    void refreshTable();
    void medicalHistoryScreen();

private slots:
    void exportPatient();
    void onPatientAdd();
    void onPatientEdit();
    void onPatientDelete();

public:
    PatientScreen(Source *source, MainWindow *mainWindow, QWidget *parent = nullptr);
};
