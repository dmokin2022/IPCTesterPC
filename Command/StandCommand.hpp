#pragma once

#include <Connector/Connector.hpp>
#include <QObject>

class StandCommander : public QObject {
  Q_OBJECT
public:
  explicit StandCommander(QString ip, int port, QString terminator, QObject *parent = nullptr);

  Connector *connector;

public slots:
  void parse(const QString incomingString);

  // Общие команды
  void Calibrate();
  void Continue();
  void ShowConst();
  void Return();
  void ChsOn();
  void ChsOff();
  void RawModeOn();
  void RawModeOff();
  void ShowLogo();

  // АСИНХРОННЫЕ Команды взаимодействия с ведомыми контроллером ( команды)
  void GetGPIO(int controller, int gpio);
  void GetADC(int controller, int adcChannel);
  void SetGPIO(int controller, int gpio, int state);
  void SetPWM(int controller, int width);
  void SendUART(int controller, QString string);
  void ReceiveUART(int controller);
  void SendI2C(int controller, QString string);
  void ReceiveI2C(int controller);

  // СИНХРОННЫЕ Команды взаимодействия с ведомыми контроллером ( команды)
  int GetGPIOsync(int controller, int gpio);
  int GetADCsync(int controller, int adcChannel);
  void SetGPIOsync(int controller, int gpio, int state);
  void SetPWMsync(int controller, int width);
  void SendUARTsync(int controller, QString string);
  QString ReceiveUARTsync(int controller);
  void SendI2Csync(int controller, QString string);
  QString ReceiveI2Csync(int controller);

  void waitForResponse();

signals:
  void receivedDat(const QStringList values);
  void receivedInfGpio(const QString value);
  void receivedInfAdc(const QString value);
  void receivedInfUart(const QString string);
  void receivedInfI2c(const QString string);
  void receivedShc(const QStringList values);

private:
  QString command;
  QString controller;
  QString responseString;
  int responseIsReady;
};
