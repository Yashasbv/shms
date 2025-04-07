#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>

#include "screens/SuggestDoctorScreen.h"

SuggestDoctorScreen::SuggestDoctorScreen(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel("Suggest Doctor");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    layout->addWidget(titleLabel);

    symptomInput = new QLineEdit;
    symptomInput->setPlaceholderText("Enter symptoms (e.g. headache, fever)");
    symptomInput->setStyleSheet("padding: 6px; font-size: 14px;");
    layout->addWidget(symptomInput);

    QPushButton *suggestBtn = new QPushButton("Suggest");
    suggestBtn->setStyleSheet("padding: 6px; font-size: 14px;");
    layout->addWidget(suggestBtn);

    resultLabel = new QLabel("");
    resultLabel->setStyleSheet("font-size: 14px; color: green;");
    layout->addWidget(resultLabel);

    connect(suggestBtn, &QPushButton::clicked, this, &SuggestDoctorScreen::onSuggestClicked);
}

void SuggestDoctorScreen::onSuggestClicked()
{
    QString symptoms = symptomInput->text().toLower();
    QString doctor;

    // Simple rule-based mapping
    if (symptoms.contains("fever") || symptoms.contains("cold") || symptoms.contains("cough"))
    {
        doctor = "General Physician";
    }
    else if (symptoms.contains("headache") || symptoms.contains("migraine"))
    {
        doctor = "Neurologist";
    }
    else if (symptoms.contains("chest") || symptoms.contains("breath"))
    {
        doctor = "Cardiologist";
    }
    else if (symptoms.contains("skin") || symptoms.contains("rash"))
    {
        doctor = "Dermatologist";
    }
    else
    {
        doctor = "Please consult a General Physician first.";
    }

    resultLabel->setText("Suggested Doctor: " + doctor);
}
