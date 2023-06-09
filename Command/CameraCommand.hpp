#pragma once

#include <QEventLoop>
#include <QObject>

#include "Connector/Connector.hpp"

class CameraCommander : public QObject {
  Q_OBJECT
public:
  explicit CameraCommander(QString ip, int port, QObject *parent = nullptr);

  Connector *connector;

public slots:
  void parse(const QString incomingString);

  void waitForResponse();

  // Команды управления платой/камерой
  void setGpio(int pin, int value);
  int getGpio(int pin);
  int getAdc(int channel);
  void setPwm(int pin, int value);
  void setMotor(int number, int value);
  void setSound(int duration, int freq);
  int getSound();
  void sendUart(int uartNum, QString string);
  QString receiveUart(int uartNum);
  void sendI2c(int i2cNum, QString string);
  QString receiveI2c(int i2c);

  //  void setGpioSync(int pin, int value);
  //  int getGpioSync(int pin);
  //  int getAdcSync(int channel);
  //  void setPwmSync(int pin, int value);
  //  void setMotorSync(int number, int value);
  //  void setDacSync(int pin, int value);
  //  void sendUartSync(int uartNum, QString string);
  //  QString receiveUartSync(int uartNum);
  //  void sendI2cSync(int i2cNum, QString string);
  //  QString receiveI2cSync(int i2c);

signals:
  void receivedInt(const int value);
  void receivedString(const QString string);

private:
  QString command;
  QString lastSentCommand;
  bool responseIsReady;
  QString responseString;
  int intVal;
  QString strVal;
  QEventLoop *el;
};
