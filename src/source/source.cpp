#include <QDebug>
#include <source/Source.h>
#include <source/AppointmentSystem.h>
#include <source/EmergencySystem.h>
#include <source/PatientManagementSystem.h>
#include <source/RoomManagementSystem.h>

Source::Source()
{
    // make db connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydata.db");

    if (!db.open())
    {
        qDebug() << "Database error occurred:" << db.lastError().text();
        return;
    }
    qDebug() << "Database connected.";
    query = new QSqlQuery(db);

    // create tables if not exist
    query->exec("CREATE TABLE IF NOT EXISTS patient ( \
        patient_id INTEGER PRIMARY KEY AUTOINCREMENT, \
        name VARCHAR(50) NOT NULL, \
        age INT NOT NULL, \
        symptom VARCHAR(255) NOT NULL, \
        severity INT NOT NULL, \
        doctor VARCHAR(50) NOT NULL \
    );");
    query->exec("CREATE TABLE IF NOT EXISTS appointment ( \
        appointment_id INTEGER PRIMARY KEY AUTOINCREMENT, \
        patient_id INT NOT NULL, \
        FOREIGN KEY (patient_id) REFERENCES patient(patient_id) \
    );");
    query->exec("CREATE TABLE IF NOT EXISTS emergency ( \
        emergency_id INTEGER PRIMARY KEY AUTOINCREMENT, \
        patient_id INT NOT NULL, \
        FOREIGN KEY (patient_id) REFERENCES patient(patient_id) \
    );");
    query->exec("CREATE TABLE IF NOT EXISTS room ( \
        room_number INT NOT NULL, \
        occupied BOOLEAN NOT NULL DEFAULT FALSE, \
        patient_id INT, \
        FOREIGN KEY (patient_id) REFERENCES patient(patient_id) \
    );");
    query->exec("CREATE TABLE medical_history ( \
        history_id INTEGER PRIMARY KEY AUTOINCREMENT, \
        patient_id INT NOT NULL, \
        diagnosis VARCHAR(255) NOT NULL, \
        treatment VARCHAR(255) NOT NULL, \
        date VARCHAR(255) NOT NULL, \
        FOREIGN KEY (patient_id) REFERENCES patient(patient_id) \
    );");

    // create instances of the systems
    patientManagementSystem = new PatientManagementSystem(this);
    appointmentSystem = new AppointmentSystem(this);
    emergencySystem = new EmergencySystem(this);
    roomManagementSystem = new RoomManagementSystem(this);
}

Source::~Source()
{
    delete patientManagementSystem;
    delete appointmentSystem;
    delete emergencySystem;
    delete roomManagementSystem;

    // close db connection
    query->clear();
    db.close();
}
