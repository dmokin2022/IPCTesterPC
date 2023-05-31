#include "StandCommand.hpp"

#include "Configs/StandConfig.h"

StandCommander::StandCommander(QString ip, int port, QString terminator, QObject *parent)
    : QObject {parent} {
  connector = new Connector(ip, port, STAND_COMMAND_TERMINATOR, this);
  connect(connector, &Connector::readyToBeParsed, this, &StandCommander::parse);

  responseIsReady = false;
}

// Разбор принятого ответа
void StandCommander::parse(const QString incomingString) {
  QStringList tokens = incomingString.split(" ");
  QString tag        = tokens.takeFirst();
  QString param      = tokens[0];
  QString type       = tokens[1];

  if (tag == "DAT") {
    emit receivedDat(tokens);
  } else if (tag == "INF") {
    responseString = tokens.last();
    //if ((param == "GPIO") && (type == "STATE")) {
    if ((param == "GPIO")) {
      emit receivedInfGpio(responseString);
      //} else if ((param == "ADC") && (type == "VAL")) {
    } else if ((param == "ADC")) {
      emit receivedInfAdc(responseString);
    } else if (command.startsWith("ReceiveUART")) {
      emit receivedInfUart(responseString);
    } else if (command.startsWith("ReceiveI2C")) {
      emit receivedInfI2c(responseString);
    }
  } else if (tag == "LOG") {
  } else if (tag == "SHC") {
    emit receivedShc(tokens);
  } else if (tag == "ZER") {
  } else if (tag == "CON") {
  }

  responseIsReady = true;
}

void StandCommander::waitForResponse() {
  //while (!responseIsReady) {}
  //responseIsReady = false;
}

// Общие команды
//---------------------------------------
void StandCommander::Calibrate() {
  command = QString("Calibrate");
  connector->send(command);
}

void StandCommander::Continue() {
  command = QString("Continue");
  connector->send(command);
}

void StandCommander::ShowConst() {
  command = QString("ShowConst");
  connector->send(command);
}

void StandCommander::Return() {
  command = QString("Return");
  connector->send(command);
}

void StandCommander::ChsOn() {
  command = QString("ChsOn");
  connector->send(command);
}

void StandCommander::ChsOff() {
  command = QString("ChsOff");
  connector->send(command);
}

void StandCommander::RawModeOn() {
  command = QString("RawModeOn");
  connector->send(command);
}

void StandCommander::RawModeOff() {
  command = QString("RawModeOff");
  connector->send(command);
}

void StandCommander::ShowLogo() {
  command = QString("ShowLogo");
  connector->send(command);
}

// Команды взаимодействия с ведомыми контроллером
//----------------------------------------------------------------
void StandCommander::GetGPIO(int controller, int gpio) {
  command = QString("GetGPIO %1 %2").arg(controller).arg(gpio);
  connector->send(command);
}

void StandCommander::GetADC(int controller, int adcChannel) {
  command = QString("GetADC %1 %2").arg(controller).arg(adcChannel);
  connector->send(command);
}

void StandCommander::SetGPIO(int controller, int gpio, int state) {
  command = QString("SetGPIO %1 %2 %3").arg(controller).arg(gpio).arg(state);
  connector->send(command);
}

void StandCommander::SetPWM(int controller, int width) {
  command = QString("SetPWM %1 %2").arg(controller).arg(width);
  connector->send(command);
}

void StandCommander::SendUART(int controller, QString string) {
  command = QString("SendUART %1 %2").arg(controller).arg(string);
  connector->send(command);
}

void StandCommander::ReceiveUART(int controller) {
  command = QString("ReceiveUART %1").arg(controller);
  connector->send(command);
}

void StandCommander::SendI2C(int controller, QString string) {
  command = QString("SendI2C %1 %2").arg(controller).arg(string);
  connector->send(command);
}

void StandCommander::ReceiveI2C(int controller) {
  command = QString("ReceiveI2C %1").arg(controller);
  //stand->send(command);
}

// Синхронные команды
int StandCommander::GetGPIOsync(int controller, int gpio) {
  command = QString("GetGPIO %1 %2").arg(controller).arg(gpio);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

int StandCommander::GetADCsync(int controller, int adcChannel) {
  command = QString("GetADC %1 %2").arg(controller).arg(adcChannel);
  connector->send(command);
  waitForResponse();
  return responseString.toInt();
}

void StandCommander::SetGPIOsync(int controller, int gpio, int state) {
  command = QString("SetGPIO %1 %2 %3").arg(controller).arg(gpio).arg(state);
  connector->send(command);
  waitForResponse();
}

void StandCommander::SetPWMsync(int controller, int width) {
  command = QString("SetPWM %1 %2").arg(controller).arg(width);
  connector->send(command);
  waitForResponse();
}

void StandCommander::SendUARTsync(int controller, QString string) {
  command = QString("SendUART %1 %2").arg(controller).arg(string);
  connector->send(command);
  waitForResponse();
}

QString StandCommander::ReceiveUARTsync(int controller) {
  command = QString("ReceiveUART %1").arg(controller);
  connector->send(command);
  waitForResponse();
  return responseString;
}

void StandCommander::SendI2Csync(int controller, QString string) {
  command = QString("SendI2C %1 %2").arg(controller).arg(string);
  connector->send(command);
  waitForResponse();
}

QString StandCommander::ReceiveI2Csync(int controller) {
  command = QString("ReceiveI2C %1").arg(controller);
  connector->send(command);
  waitForResponse();
  return responseString;
}
