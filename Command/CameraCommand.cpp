#include "CameraCommand.hpp"

#include "Configs/CameraConfig.h"
#include "Connector/Connector.hpp"

CameraCommander::CameraCommander(QString ip, int port, QObject *parent) : QObject {parent} {
  //
  connector = new Connector(ip, port, QString(CAMERA_COMMAND_TERMINATOR), parent);
  connect(connector, &Connector::readyToBeParsed, this, &CameraCommander::parse);
  responseIsReady = false;
  el              = new QEventLoop();
}

// Парсинг приходящего ответа
void CameraCommander::parse(const QString incomingString) {
  QStringList tokens = incomingString.split(" ");

  if (tokens.size() < CAMERA_WORDS_IN_RESPONSE) { return; }

  QString param  = tokens[0];
  int number     = tokens[1].toInt();
  responseString = tokens[2];

  if (connector->lastSentCommand.startsWith("get")) {
    intVal = responseString.toInt();
    emit receivedInt(intVal);
  } else if (connector->lastSentCommand.startsWith("receive")) {
    // Внимание!!! Требуется проверка. Возможно ли использовать такую конструкцию
    // Строка может быть разной длины!!!
    strVal = responseString;  // ATTENTION!!!
    emit receivedString(strVal);
  }

  responseIsReady = true;
  el->exit();
}

void CameraCommander::waitForResponse() {
  // Внимание!!! Потом раскомментировать. Вернуть как было
  // TODO:
  // Блокирующий вариант (не использовать)
  //  responseIsReady = false;
  //  while (responseIsReady == false) {}
  //  responseIsReady = false;

  // Блокирующий вариант с сохранением реакции на события пользователя
  el->exec();
}

void CameraCommander::setGpio(int pin, int value) {
  command = QString("set gpio %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

int CameraCommander::getGpio(int pin) {
  command = QString("get gpio %1").arg(pin);
  connector->send(command);
  waitForResponse();

  return intVal;
}

int CameraCommander::getAdc(int channel) {
  command = QString("get adc %1").arg(channel);
  connector->send(command);
  waitForResponse();
  return intVal;
}

void CameraCommander::setPwm(int pin, int value) {
  command = QString("set pwm %1 %2").arg(pin).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCommander::setMotor(int number, int value) {
  command = QString("set motor %1 %2").arg(number).arg(value);
  connector->send(command);
  waitForResponse();
}

void CameraCommander::setSound(int duration, int freq) {
  command = QString("set sound %1 %2").arg(duration).arg(freq);
  connector->send(command);
  waitForResponse();
}

int CameraCommander::getSound() {
  command = QString("get sound 0");
  connector->send(command);
  waitForResponse();
  return intVal;
}

void CameraCommander::sendUart(int uartNum, QString string) {
  command = QString("send uart %1 %2").arg(uartNum).arg(string);
  connector->send(command);
  waitForResponse();
}

QString CameraCommander::receiveUart(int uartNum) {
  command = QString("receive uart %1").arg(uartNum);
  connector->send(command);
  waitForResponse();
  return strVal;
}

void CameraCommander::sendI2c(int i2cNum, QString string) {
  command = QString("send i2c %1 %2").arg(i2cNum).arg(string);
  connector->send(command);
  waitForResponse();
}

QString CameraCommander::receiveI2c(int i2cNum) {
  command = QString("send i2c %1").arg(i2cNum);
  connector->send(command);
  waitForResponse();
  return strVal;
}
