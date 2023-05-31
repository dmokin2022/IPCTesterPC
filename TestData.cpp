#include "TestData.hpp"

#include "qdebug.h"

TestData::TestData() {
  this->name      = "";
  this->value     = 0;
  this->min       = 0;
  this->max       = 0;
  this->isInRange = false;

  //toDebug();
}

TestData::TestData(QString name, QVariant value, int min, int max) {
  this->name  = name;
  this->value = value;
  this->min   = min;
  this->max   = max;

  if ((min <= value.toInt()) && (value.toInt() <= max)) {
    this->isInRange = true;
  } else
    this->isInRange = false;

  //toDebug();
}

TestData::TestData(QString name, QVariant value, int correctValue) {
  this->name  = name;
  this->value = value;
  this->min   = correctValue;
  this->max   = 0;

  if (value == correctValue) {
    this->isInRange = true;
  } else
    this->isInRange = false;

  //toDebug();
}

TestData::TestData(QString name, QVariant value, QString string) {
  this->name  = name;
  this->value = value;
  this->min   = 0;
  this->max   = 0;

  if (value == string) {
    this->isInRange = true;
  } else
    this->isInRange = false;

  toDebug();
}

void TestData::toDebug() {
  //
  qDebug() << name << ": "
           << "value=" << value.toInt() << "min=" << min << "max=" << max;
}
