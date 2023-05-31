#include "CameraCommand.hpp"

#include "Configs/CameraConfig.h"
#include "Connector/Connector.hpp"

CameraCommander::CameraCommander(QString ip, int port, QObject *parent) : QObject {parent} {
  //
  connector = new Connector(ip, port, QString(CAMERA_COMMAND_TERMINATOR), parent);
  connect(connector, &Connector::readyToBeParsed, this, &CameraCommander::parse);
  responseIsReady = false;
}

void CameraCommander::parse(const QString incomingString) {
  QStringList tokens = incomingString.split(" ");

  if (tokens.size() < CAMERA_WORDS_IN_RESPONSE) { return; }

  QString param  = tokens[0];
  int number     = tokens[1].toInt();
  responseString = tokens[2];

  if (command.startsWith("get")) {
    *intVal = responseString.toInt();
    emit receivedInt(*intVal);
  } else if (command.startsWith("receive")) {
    // Внимание!!! Требуется проверка. Возможно ли использовать такую конструкцию
    // Строка может быть разной длины!!!
    *strVal = responseString;  // ATTENTION!!!
    emit receivedString(*strVal);
  }

  responseIsReady = true;
}

void CameraCommander::waitForResponse() {
  // Внимание!!! Потом раскомментировать. Вернуть как было
  // TODO:
  //  while (!responseIsReady) {}
//  responseIsReady = false;
}

void CameraCommander::setGpio(int pin, int value) {
  command = QString("set gpio %1 %2").arg(pin).arg(value);
  connector->send(command);
}

int CameraCommander::getGpio(int pin, int &value) {
  command = QString("get gpio %1 %2").arg(pin);
  intVal  = &value;
  connector->send(command);
  return 0;
}

int CameraCommander::getAdc(int channel, int &value) {
  command = QString("get adc %1").arg(channel);
  intVal  = &value;
  connector->send(command);
  return 0;
}

void CameraCommander::setPwm(int pin, int value) {
  command = QString("set pwm %1 %2").arg(pin).arg(value);
  connector->send(command);
}

void CameraCommander::setMotor(int number, int value) {
  command = QString("set motor %1 %2").arg(number).arg(value);
  connector->send(command);
}

void CameraCommander::setDac(int pin, int value) {
  command = QString("get dac %1 %2").arg(pin).arg(value);
  connector->send(command);
}

void CameraCommander::sendUart(int uartNum, QString string) {
  command = QString("send uart %1 %2").arg(uartNum).arg(string);
  connector->send(command);
}

QString CameraCommander::receiveUart(int uartNum, QString &string) {
  command = QString("receive uart %1").arg(uartNum);
  strVal  = &string;
  connector->send(command);
  return string;
}

void CameraCommander::sendI2c(int i2cNum, QString string) {
  command = QString("send i2c %1 %2").arg(i2cNum).arg(string);
  connector->send(command);
}

QString CameraCommander::receiveI2c(int i2cNum, QString &string) {
  command = QString("send i2c %1").arg(i2cNum);
  strVal  = &string;
  connector->send(command);
  return string;
}

// Синхронные команды
void CameraCommander::setGpioSync(int pin, int value) {
  command = QString("set gpio %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

int CameraCommander::getGpioSync(int pin) {
  command = QString("get gpio %1 %2").arg(pin);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

int CameraCommander::getAdcSync(int channel) {
  command = QString("get adc %1").arg(channel);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

void CameraCommander::setPwmSync(int pin, int value) {
  command = QString("set pwm %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCommander::setMotorSync(int number, int value) {
  command = QString("set motor %1 %2").arg(number).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCommander::setDacSync(int pin, int value) {
  command = QString("set dac %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCommander::sendUartSync(int uartNum, QString string) {
  command = QString("send uart %1 %2").arg(uartNum).arg(string);
  connector->send(command);
}

QString CameraCommander::receiveUartSync(int uartNum) {
  command = QString("receive uart %1").arg(uartNum);
  connector->send(command);
  waitForResponse();
  return responseString;
}

void CameraCommander::sendI2cSync(int i2cNum, QString string) {
  command = QString("send i2c %1 %2").arg(i2cNum).arg(string);
  connector->send(command);
  waitForResponse();
}

QString CameraCommander::receiveI2cSync(int i2cNum) {
  command = QString("send i2c %1").arg(i2cNum);
  connector->send(command);
  waitForResponse();
  return responseString;
}
