#pragma once

#include <SList.h>
#include <AVLTree.h>
#include <QString>

class Source;

struct MedicalHistoryNode
{
    int id;
    int patient_id;
    QString diagnosis;
    QString prescription;
    QString date;
};

struct Patient
{
    int id;
    QString name;
    int age;
    QString condition;
    int severity;
    QString doctor;
    AVL<MedicalHistoryNode> *historyNode; // structure for medical history
    SLIST_ENTRY(Patient) next;
};
typedef struct Patient Patient;

SLIST_HEAD(PatientList, Patient);

class PatientManagementSystem
{
private:
    Source *source;
    struct PatientList head;

public:
    PatientManagementSystem(Source *source);
    ~PatientManagementSystem();
    void addPatient(const QString name, int age, const QString condition, int severity, const QString doctor);
    void updatePatient(int patientId, const QString name, int age, const QString condition, int severity, const QString doctor);
    void removePatient(int patientId);
    Patient *getPatient(int patientId) const;
    Patient *listAllPatients(Patient *current) const;
    MedicalHistoryNode *getMedicalHistory(Patient *current, int *len);
    void addPatientMedicalHistory(int patientId, QString diagnosis, QString prescription, QString date);
};
