#include "Variable.hpp"

CameraCommander *Variable::cam;
StandCommander *Variable::stand;
int Variable::current;

Variable::Variable(QObject *parent) : QObject {parent} {
  name   = "";
  type   = "";
  string = "";
  intVal = 0;
  device = "";
  module = "";
  port   = 0;
}

Variable::Variable(const Variable &v) {
  this->name = v.name;
  this->type = v.type;

  this->string = v.string;
  this->intVal = v.intVal;

  this->device = v.device;
  this->module = v.module;
  this->port   = v.port;
}

Variable Variable::operator=(const Variable &v) {
  this->name = v.name;
  this->type = v.type;

  this->string = v.string;
  this->intVal = v.intVal;

  this->device = v.device;
  this->module = v.module;
  this->port   = v.port;

  return *this;
}

Variable::Variable(CameraCommander *cam, StandCommander *stand, int current, QObject *parent)
    : QObject {parent} {
  setDevices(cam, stand, current);
}

void Variable::setDevices(CameraCommander *cam, StandCommander *stand, int current) {
  this->cam     = cam;
  this->stand   = stand;
  this->current = current;
}

bool Variable::operator==(Variable &var) {
  if (type == var.type) {
    if (type == "string") {
      if (string == var.string) return true;
      else
        return false;
    }
    if (type == "int") {
      if (intVal == var.intVal) return true;
      else
        return false;
    }
  } else
    return false;

  return false;
}

bool Variable::isInRange(int min, int max) {
  if (type == "string") return false;
  if ((min < intVal) && (intVal < max)) return true;
  else
    return false;
}

Variable Variable::getValue() {
  intVal = -1;
  //QString string = "";

  if (device == "stand") {
    if (module == "gpio") {
      intVal = stand->GetGPIOsync(current, port);
    } else if (module == "adc") {
      intVal = stand->GetADCsync(current, port);
    } else if (module == "uart") { /*string =*/
      stand->ReceiveUART(current);
    } else if (module == "i2c") { /*string =*/
      stand->ReceiveUART(current);
    }

  } else if (device == "cam") {
    if (module == "gpio") {
      intVal = cam->getGpio(port);
    } else if (module == "adc") {
      intVal = cam->getAdc(port);
    } else if (module == "sound") {
      intVal = cam->getSound();
    } else if (module == "uart") {
      string = cam->receiveUart(0);
    } else if (module == "i2c") {
      string = cam->receiveUart(0);
    }
  }

  if (intVal != -1) {
    string     = QString::number(intVal);
    this->type = "int";
  }
  return *this;
}

void Variable::setValue(const QString &value) {
  if (device == "stand") {
    if (module == "gpio") {
      stand->SetGPIOsync(current, port, value.toInt());
    } else if (module == "pwm") {
      stand->SetPWMsync(current, value.toInt());
    } else if (module == "uart") {
      stand->SendUARTsync(current, value);
    } else if (module == "i2c") {
      stand->SendI2C(current, value);
    }
  } else if (device == "cam") {
    if (module == "gpio") {
      cam->setGpio(port, value.toInt());
    } else if (module == "pwm") {
      cam->setPwm(port, value.toInt());
    } else if (module == "motor") {
      cam->setMotor(port, value.toInt());
    } else if (module == "uart") {
      cam->sendUart(0, value);
    } else if (module == "i2c") {
      cam->sendI2c(0, value);
    } else if (module == "sound") {
      int duration = port;
      int freq     = value.toInt();
      cam->setSound(duration, freq);
    }
  }
}
