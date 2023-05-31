#include "ParameterView.hpp"

ParameterView::ParameterView(QObject *parent) : QObject {parent} {}

ParameterView::ParameterView(QLabel *name, QLabel *value, QLineEdit *minThreshold,
    QLineEdit *maxThreshold, QPushButton *button, TestData &testData, QObject *parent)
    : QObject {parent} {
  this->name         = name;
  this->value        = value;
  this->minThreshold = minThreshold;
  this->maxThreshold = maxThreshold;
  this->button       = button;
  this->testData     = testData;
}

void ParameterView::showParameterTestResults(TestData &testData) {
  this->testData = testData;
  if (name) { name->setText(testData.name); }
  if (value) { value->setText(testData.value.toString()); }
  if (minThreshold) { minThreshold->setText(QString::number(testData.min)); }
  if (maxThreshold) { maxThreshold->setText(QString::number(testData.max)); }

  if (button) {
    if (testData.isInRange) {
      button->setStyleSheet("QPushButton {background-color: green}");
    } else {
      button->setStyleSheet("QPushButton {background-color: red}");
    }
  }
}

//void ParameterView::onCommandResponseReceived() {}
