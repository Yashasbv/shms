#include <QtSql/QSqlQuery>
#include <source/Source.h>
#include <source/PatientManagementSystem.h>

PatientManagementSystem::PatientManagementSystem(Source *source) : source(source)
{
    head = SLIST_HEAD_INITIALIZER(head);

    // Initialize the patient management system
    QSqlQuery *query = this->source->getQuery();
    query->exec("SELECT patient_id, name, age, symptom, severity, doctor FROM patient order by patient_id");
    while (query->next())
    {
        int id = query->value(0).toInt();
        QString name = query->value(1).toString();
        int age = query->value(2).toInt();
        QString condition = query->value(3).toString();
        int severity = query->value(4).toInt();
        QString doctor = query->value(5).toString();

        Patient *newPatient = new Patient{id, name, age, condition, severity, doctor, nullptr};
        SLIST_INSERT_TAIL(&head, newPatient, Patient, next);
        qDebug() << "ID:" << id << ", Name:" << name;
    }
    qDebug() << "Patients loaded from database.";

    Patient *patient = nullptr;
    SLIST_FOREACH(patient, &head, next)
    {
        QSqlQuery *query = this->source->getQuery();
        query->prepare("SELECT history_id, diagnosis, treatment, date FROM medical_history where patient_id = ? order by history_id");
        query->addBindValue(patient->id);
        AVL<MedicalHistoryNode> *medicalHistoryHead = new AVL<MedicalHistoryNode>;

        if (!query->exec())
        {
            qDebug() << "Error getting last select:" << query->lastError().text();
            return;
        }
        while (query->next())
        {
            int id = query->value(0).toInt();
            QString diagnosis = query->value(1).toString();
            QString prescription = query->value(2).toString();
            QString date = query->value(3).toString();

            MedicalHistoryNode newMedicalHistoryNode = {id, patient->id, diagnosis, prescription, date};
            medicalHistoryHead->insert(newMedicalHistoryNode);
        }
        qDebug() << "Medical history loaded from database.";

        patient->historyNode = medicalHistoryHead;
    }
}

PatientManagementSystem::~PatientManagementSystem()
{
    // Clean up the linked list of patients
    Patient *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        // TODO: delete the tree
        delete tmp1;
    }
    qDebug() << "PatientManagementSystem destroyed.";
}

void PatientManagementSystem::addPatient(const QString name, int age, const QString condition, int severity, const QString doctor)
{
    // Add a new patient to the system
    int patientId = 0;
    QSqlQuery *query = this->source->getQuery();

    query->prepare("INSERT INTO patient (name, age, symptom, severity, doctor) VALUES (?, ?, ?, ?, ?)");
    query->addBindValue(name);
    query->addBindValue(age);
    query->addBindValue(condition);
    query->addBindValue(severity);
    query->addBindValue(doctor);
    if (!query->exec())
    {
        qDebug() << "Error adding patient:" << query->lastError().text();
        return;
    }

    // get the last inserted ID
    query->prepare("SELECT last_insert_rowid()");
    if (!query->exec())
    {
        qDebug() << "Error getting last insert ID:" << query->lastError().text();
        return;
    }
    if (query->next())
    {
        patientId = query->value(0).toInt();
    }

    Patient *newPatient = new Patient{patientId, name, age, condition, severity, doctor, nullptr, nullptr};
    SLIST_INSERT_TAIL(&head, newPatient, Patient, next);
    qDebug() << "Added patient:" << name << "with ID:" << patientId;
}

void PatientManagementSystem::updatePatient(int patientId, const QString name, int age, const QString condition, int severity, const QString doctor)
{
    QSqlQuery *query = this->source->getQuery();

    // Update an existing patient's information
    query->prepare("UPDATE patient SET name = ?, age = ?, symptom = ?, severity = ?, doctor = ? WHERE patient_id = ?");
    query->addBindValue(name);
    query->addBindValue(age);
    query->addBindValue(condition);
    query->addBindValue(severity);
    query->addBindValue(doctor);
    query->addBindValue(patientId);

    if (!query->exec())
    {
        qDebug() << "Error updating patient:" << query->lastError().text();
        return;
    }

    Patient *current;
    SLIST_FOREACH(current, &head, next)
    {
        if (current->id == patientId)
        {
            current->name = name;
            current->age = age;
            current->condition = condition;
            current->severity = severity;
            current->doctor = doctor;
            break;
        }
    }
    qDebug() << "Updated patient with ID:" << patientId;
}

void PatientManagementSystem::removePatient(int patientId)
{
    QSqlQuery *query = this->source->getQuery();

    // Remove a patient from the system
    query->prepare("DELETE FROM patient WHERE patient_id = ?");
    query->addBindValue(patientId);
    if (!query->exec())
    {
        qDebug() << "Error removing patient:" << query->lastError().text();
        return;
    }

    Patient *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        if (tmp1->id == patientId)
        {
            SLIST_REMOVE(&head, tmp1, Patient, next);
            delete tmp1;
            break;
        }
    }
    qDebug() << "Removed patient with ID:" << patientId;
}

Patient *PatientManagementSystem::getPatient(int patientId) const
{
    // View a patient's information
    Patient *current;
    SLIST_FOREACH(current, &head, next)
    {
        if (current->id == patientId)
        {
            return current;
        }
    }
    return nullptr;
}

Patient *PatientManagementSystem::listAllPatients(Patient *current) const
{
    // Get the next patient in the linked list
    // Yield logic
    if (current == nullptr)
    {
        return SLIST_FIRST(&head);
    }
    return SLIST_NEXT(current, next);
}

MedicalHistoryNode *PatientManagementSystem::getMedicalHistory(Patient *current, int *len)
{
    AVL<MedicalHistoryNode> *medicalHistoryHead = current->historyNode;
    if (medicalHistoryHead == nullptr)
    {
        *len = 0;
        return nullptr;
    }
    return medicalHistoryHead->levelOrder(len);
}

void PatientManagementSystem::addPatientMedicalHistory(int patientId, QString diagnosis, QString prescription, QString date)
{
    Patient *current;
    SLIST_FOREACH(current, &head, next)
    {
        if (current->id == patientId)
        {
            break;
        }
    }

    if (current == nullptr)
    {
        qDebug() << "Patient not found";
        return;
    }

    int id = 0;
    QSqlQuery *query = this->source->getQuery();

    query->prepare("INSERT INTO medical_history (patient_id, diagnosis, treatment, date) VALUES (?, ?, ?, ?)");
    query->addBindValue(patientId);
    query->addBindValue(diagnosis);
    query->addBindValue(prescription);
    query->addBindValue(date);
    if (!query->exec())
    {
        qDebug() << "Error adding medical_history:" << query->lastError().text();
        return;
    }

    // get the last inserted ID
    query->prepare("SELECT last_insert_rowid()");
    if (!query->exec())
    {
        qDebug() << "Error getting last insert ID:" << query->lastError().text();
        return;
    }
    if (query->next())
    {
        id = query->value(0).toInt();
    }

    AVL<MedicalHistoryNode> *avl = current->historyNode;
    if (avl == nullptr)
    {
        avl = new AVL<MedicalHistoryNode>;
        current->historyNode = avl;
    }
    MedicalHistoryNode medicalHistory = {id, patientId, diagnosis, prescription, date};
    avl->insert(medicalHistory);
}
