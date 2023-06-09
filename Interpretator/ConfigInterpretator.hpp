#pragma once

#include <Command/CameraCommand.hpp>
#include <Command/StandCommand.hpp>
#include <QMap>
#include <QObject>
#include <QSet>

#include "Variable.hpp"

class ConfigInterpretator : public QObject {
  Q_OBJECT
public:
  explicit ConfigInterpretator(
      StandCommander *stand, CameraCommander *cam, QObject *parent = nullptr);
  void setCurrentCam(CameraCommander *cam, int current);

  QStringList &eval(QString expression);
  void reset();

  void testVariableInit();
  void testVariableExecution();
signals:
  void oneRequestIsCompleted(const QString result);

private:
  Variable parseRValue(const QString &rvalue_);
  Variable parseLValue(const QString &lvalue_);

  bool isNumber(const QString &string);
  bool isString(const QString &string);
  bool isDeviceRef(const QString &string);
  bool isVarRef(const QString &string);

  QString cmpResult(bool condition);

  Variable getDeviceRefFrom(const QString &string);
  Variable getValueFrom(const QString &string);

  QStringList strings;
  QMap<QString, Variable *> variables;
  QStringList queryResults;

  QSet<QString> devices;
  QSet<QString> modules;
  QSet<QString> availablePins;

  //CameraCommander *cam;
  //StandCommander *stand;
  //int current;
};
