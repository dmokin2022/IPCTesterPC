#include <QString>
#include <QVariant>

#pragma once

class TestData {
public:
  TestData();
  TestData(QString name, QVariant value, int min, int max);
  TestData(QString name, QVariant value, int correctValue);
  TestData(QString name, QVariant value, QString string);
  void toDebug();

  QString name;
  QVariant value;
  int min;
  int max;
  bool isInRange;
};
