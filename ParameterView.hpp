#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>

#include "TestData.hpp"

class ParameterView : public QObject {
  Q_OBJECT
public:
  explicit ParameterView(QObject *parent = nullptr);
  explicit ParameterView(QLabel *name, QLabel *value, QLineEdit *minThreshold,
      QLineEdit *maxThreshold, QPushButton *button, TestData &testData, QObject *parent = nullptr);
  void showParameterTestResults(TestData &testData);

signals:

public slots:
  // по прохождению теста
  //void onCommandResponseReceived();
  //void onParameterTestIsReady(int currentCam, TestData &testData);
private:
  QLabel *name;
  QLabel *value;
  QLineEdit *minThreshold;
  QLineEdit *maxThreshold;
  QPushButton *button;

  TestData testData;
};
