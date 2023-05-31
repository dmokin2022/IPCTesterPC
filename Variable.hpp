#pragma once

#include <QObject>

#include "Command/CameraCommand.hpp"
#include "Command/StandCommand.hpp"

class Variable : public QObject {
  Q_OBJECT
public:
  explicit Variable(QObject *parent = nullptr);
  explicit Variable(
      CameraCommander *cam, StandCommander *stand, int current, QObject *parent = nullptr);
  Variable(const Variable &v);
  Variable operator=(const Variable &v);

  QString name;
  QString string;
  int intVal;
  QString device;
  QString module;
  int port;
  QString type;

  void setDevices(CameraCommander *cam, StandCommander *stand, int current);
  bool operator==(Variable &var);
  bool isInRange(int min, int max);

  Variable getValue();
  void setValue(const QString &value);

  static CameraCommander* cam;
  static StandCommander* stand;
  static int current;

signals:
};
