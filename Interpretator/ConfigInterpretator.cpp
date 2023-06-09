#include "ConfigInterpretator.hpp"

#include "qregularexpression.h"

ConfigInterpretator::ConfigInterpretator(
    StandCommander *stand, CameraCommander *cam, QObject *parent)
    : QObject {parent} {
  devices << "cam"
          << "stand";
  modules << "gpio"
          << "pin"
          << "pwm"
          << "adc"
          << "dac"
          << "motor"
          << "uart"
          << "i2c";

  // Внимание!!!
  // TODO: инициировать указатели cam и stand
  Variable::stand = stand;
  setCurrentCam(cam, 0);
}

QStringList &ConfigInterpretator::eval(QString expression) {
  // Очистка предыдущих выполненных строк
  strings.clear();
  queryResults.clear();
  // Удаление лишних пробелов
  // Разбить выражение на строки
  // Удаление комментариев
  QRegularExpression comments("#.*\n");
  strings = expression.remove(comments).toLower().split("\n");
  //strings = expression.simplified().toLower().split("\n");

  // Проход по всем строкам и их интерпретация
  for (auto &item : strings) {
    // Интерпретация строк
    QString string = item.simplified();

    // Если строка является запросом на получение значений и сравнение
    if (string.startsWith("?")) {
      QString queryResult;
      QString result;
      string.remove("?");
      // Сравнение с точным значением
      if (string.contains("=")) {
        //
        auto parts = string.split("=");
        auto lvar  = getValueFrom(parts[0]);
        auto rvar  = parseRValue(parts[1]);

        result      = cmpResult(lvar == rvar);
        queryResult = QString("%1=%2 == %3 ......... %4")
                          .arg(lvar.name)
                          .arg(lvar.string)
                          .arg(rvar.string)
                          .arg(result);

        // Проверка на вхождение в диапазон
      } else if (string.contains("<")) {
        //
        auto parts = string.split("<");
        auto lvar  = getValueFrom(parts[1]);
        auto min   = parseRValue(parts[0]).intVal;
        auto max   = parseRValue(parts[2]).intVal;

        result      = cmpResult(lvar.isInRange(min, max));
        queryResult = QString("%1 < %2=%3 < %4 ......... %5")
                          .arg(min)
                          .arg(lvar.name)
                          .arg(lvar.intVal)
                          .arg(max)
                          .arg(result);
      }
      queryResults << queryResult;
      emit oneRequestIsCompleted(queryResult);

      // Если строка является запросом на присвоение
    } else {
      //
      if (string.isEmpty()) continue;
      QStringList parts = string.split("=");
      QString lvalue    = parts[0].simplified();
      QString rvalue    = parts[1].simplified();

      if (variables.contains(lvalue)) {
        Variable *var = variables[lvalue];
        // выполнение команды, связанной с переменной
        if (var->type == "devref") {
          //
          var->setValue(rvalue);
        }
      } else {
        // добавляем переменную в каталог доступных
        Variable *newVar  = new Variable {parseRValue(rvalue)};
        newVar->name      = lvalue;
        variables[lvalue] = newVar;
      }
    }
  }
  return queryResults;
}

void ConfigInterpretator::reset() {}

Variable ConfigInterpretator::parseRValue(const QString &rvalue_) {
  // Всё таки лучше возвращать явное копируемое значение Variable
  Variable result(this);
  // Удаление лишних пробелов
  QString rvalue = rvalue_.simplified();

  // Всего возможно 4 варианта для rvalue:

  // Если rvalue = число (числовой литерал)
  if (isNumber(rvalue)) {
    result.intVal = rvalue.toInt();
    result.string = rvalue;
    result.type   = "int";
  }
  // Если rvalue = строка (в кавычках "..." строковой литерал)
  else if (isString(rvalue)) {
    QString temp  = rvalue;
    result.string = temp.remove('"');
    result.type   = "string";
  }
  // Если rvalue = ссылка на устройство.модульпорт
  else if (isDeviceRef(rvalue)) {
    result = getDeviceRefFrom(rvalue);
  }
  //Если rvalue = ссылка на другую переменную
  else if (isVarRef(rvalue)) {
    //
    result = *variables[rvalue];
  }

  return result;
}

Variable ConfigInterpretator::parseLValue(const QString &lvalue_) {
  Variable result(this);
  // Удаление лишних пробелов
  QString lvalue = lvalue_.simplified();

  // Всего возможно 2 варианта для lvalue:

  // Если rvalue = ссылка на существующую переменную
  if (isVarRef(lvalue)) {
    //
    result = *variables[lvalue];

    // Если rvalue = ссылка на устройство.модульпорт
  } else if (isDeviceRef(lvalue)) {
    //
    result = getDeviceRefFrom(lvalue);
  }

  return result;
}

Variable ConfigInterpretator::getValueFrom(const QString &string) {
  Variable var = parseLValue(string);
  if (var.type == "devref") var.getValue();

  return var;
}

bool ConfigInterpretator::isNumber(const QString &string) {
  QRegExp re("\\d+");
  // Если строка начинается с набора цифр
  if (re.indexIn(string.simplified()) == 0) return true;
  else
    return false;
}

bool ConfigInterpretator::isString(const QString &string) {
  if (string.contains("\"")) return true;
  else
    return false;
}

bool ConfigInterpretator::isDeviceRef(const QString &string) {
  if (string.split(".").size() == 2) return true;
  else
    return false;
}

bool ConfigInterpretator::isVarRef(const QString &string) {
  if (variables.contains(string.simplified())) return true;
  else
    return false;
}

QString ConfigInterpretator::cmpResult(bool condition) {
  if (condition) {
    return "<Ok>";
  } else {
    return "<Fail>";
  }
}

Variable ConfigInterpretator::getDeviceRefFrom(const QString &string) {
  Variable var(this);

  QStringList parts     = string.split(".");
  QString device        = parts[0].simplified();
  QString moduleAndPort = parts[1].simplified();

  QRegExp re("\\d+");
  re.indexIn(moduleAndPort);
  QStringList numbers = re.capturedTexts();

  QString port   = numbers[0];
  QString module = moduleAndPort.remove(re);

  if (devices.contains(device)) {
    var.device = device;
  } else {
    var.device = "";
  }

  if (modules.contains(module)) {
    var.module = module;
  } else {
    var.module = "";
  }

  var.port = port.toInt();
  var.type = "devref";

  return var;
}

void ConfigInterpretator::setCurrentCam(CameraCommander *cam, int current) {
  Variable::cam     = cam;
  Variable::current = current;
}

void ConfigInterpretator::testVariableInit() {
  QString variableInitString =
      "#--------------------------------------- \n\
      MAX_ADC = 1023 \n\
      MIN_ADC = 100 \n\
      TEST_STRING = \"Hello world\" \n\
      ANOTHER_MAX_ADC = MAX_ADC \n\
      ANOTHER_TEST_STRING = TEST_STRING \n\
      CAM_MOTOR_FOCUS = CAM.MOTOR0 \n\
        CAM_MOTOR_ZOOM = CAM.MOTOR1 \n\
        STAND_MOTOR_FOCUS = STAND.ADC1 \n\
        STAND_MOTOR_ZOOM = STAND.ADC2 \n\
        CAM_IRC_MOTOR_PLUS = CAM.GPIO78 \n\
        CAM_IRC_MOTOR_MINUS = CAM.GPIO79 \n\
        STAND_IRC_MOTOR_PLUS = STAND.GPIO1 \n\
        STAND_IRC_MOTOR_MINUS = STAND.GPIO2 \n\
        CAM_IR_DIM = CAM.PWM10 \n\
        CAM_IR_CDS = CAM.ADC2 \n\
        STAND_IR_DIM = STAND.ADC3 \n\
        STAND_IR_CDS = STAND.PWMx \n\
        CAM_I2C = CAM.I2C0 \n\
        STAND_I2C = STAND.I2C0 \n\
        CAM_IRIS = CAM.PWM20 \n\
        STAND_IRIS = STAND.ADC4 \n\
        CAM_UART = CAM.UART0 \n\
        STAND_UART = STAND.UART0 \n\
        CAM_ALM_IN = CAM.GPIO65 \n\
        CAM_ALM_OUT = CAM.GPIO66 \n\
        CAM_LED1 = CAM.GPIO77 \n\
        STAND_LED1 = STAND.GPIO3 \n\
        STAND_RES = STAND.GPIO4 \n\
        TEMP = 111 \n\
#------------------------------------- \n\
      CAM_MOTOR_FOCUS = 50 \n\
      ? 0 < STAND_MOTOR_FOCUS < MAX_ADC \n\
      CAM_MOTOR_ZOOM = 50 \n\
      ? 0 < STAND_MOTOR_ZOOM < MAX_ADC \n\
      CAM_IRC_MOTOR_MINUS = 1 \n\
      ? STAND_IRC_MOTOR_MINUS = 1 \n\
      CAM_IRC_MOTOR_PLUS = 1 \n\
      ? STAND_IRC_MOTOR_PLUS = 1 \n\
      CAM_IR_DIM = 50 \n\
      ? 0 < STAND_IR_DIM < MAX_ADC \n\
      STAND_IR_CDS = 50 \n\
      ? 0 < CAM_IR_CDS < MAX_ADC \n\
      CAM_I2C = \"TEST_STRING\" \n\
      ? STAND_I2C = \"TEST_STRING\" \n\
      CAM_IRIS = 100 \n\
      ? STAND_IRIS = 1000 \n\
      CAM_UART = \"TEST_STRING\" \n\
      ? STAND_UART = \"TEST_STRING\" \n\
      CAM_ALM_OUT = 1 \n\
      ? CAM_ALM_IN = 1 \n\
      CAM_LED1 = 1 \n\
      ? STAND_LED1 = 1 \n\
      STAND_RES = 1 \n\
                               ";
  eval(variableInitString);
}
