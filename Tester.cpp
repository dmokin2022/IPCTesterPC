#include "Tester.hpp"

#include <QDir>
#include <QDirIterator>

#include "Configs/CameraConfig.h"
#include "Configs/Config.h"
#include "Configs/StandConfig.h"
#include "Configs/TestNames.h"
#include "Configs/Thresholds.h"

/*
Настройка чтобы отправлялись пакеты по Ethernet
	Проверить существующие ip-адреса рабочего компьютера: ip a
	Если там есть ip = 192.168.1.254/24
		Удалить этот ip: sudo ip a del 192.168.1.251/24 dev lo
*/

Tester::Tester(QObject *parent) : QObject {parent} {
  stand = new StandCommander(STAND_IP_ADDRESS, STAND_IP_PORT, STAND_COMMAND_TERMINATOR, parent);

  testScriptDir   = TEST_SCRIPTS_DIR;
  testScriptFiles = getFilesListFromDir(testScriptDir);
  //configFile.setFileName("337_1.py");
  //if (configFile.open(QIODevice::ReadWrite | QIODevice::Text)) {}

  cameras.append(new CameraCommander(CAMERA1_IP_ADDRESS, CAMERA_IP_PORT, parent));
  cameras.append(new CameraCommander(CAMERA2_IP_ADDRESS, CAMERA_IP_PORT, parent));
  cameras.append(new CameraCommander(CAMERA3_IP_ADDRESS, CAMERA_IP_PORT, parent));
  cameras.append(new CameraCommander(CAMERA4_IP_ADDRESS, CAMERA_IP_PORT, parent));
  cameras.append(new CameraCommander(CAMERA5_IP_ADDRESS, CAMERA_IP_PORT, parent));
  cameras.append(new CameraCommander(CAMERA6_IP_ADDRESS, CAMERA_IP_PORT, parent));

  setCurrentTestingCam(1);
  interpretator = new ConfigInterpretator(stand, cameras[1], this);

  getFilesListFromDir(TEST_SCRIPTS_DIR);
}

bool Tester::testCamWith(const QString &script) {
  interpretator->setCurrentCam(cameras[currentCam], currentCam);
  QStringList results = interpretator->eval(script);
  emit camTestIsReady_(currentCam, results);

  if (results.contains("<Fail>")) return false;
  else
    return true;
}

void Tester::testAllCams_() {
  QString script = getScriptTextFrom(currentFile);

  for (int i = 0; i < CAMERA_MAX_COUNT; ++i) {
    setCurrentTestingCam(i);
    testCamWith(script);
  }
}

void Tester::testFocusMotor() {
  //cameras[currentCam]->setGpioSync(FOCUS_MOTOR, 1);
  auto value = stand->GetADCsync(currentCam, FOCUS_ADC1);

  TestData testData(FOCUS_TEST_NAME, value, FOCUS_MIN_ADC1, FOCUS_MAX_ADC1);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testZoomMotor() {
  //cameras[currentCam]->setGpioSync(ZOOM_MOTOR, 1);
  auto value = stand->GetADCsync(currentCam, ZOOM_ADC2);

  TestData testData(ZOOM_TEST_NAME, value, ZOOM_MIN_ADC2, ZOOM_MAX_ADC2);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testIrCurtain() {
  //cameras[currentCam]->setGpioSync(IRC, 1);
  auto value1 = stand->GetGPIOsync(currentCam, IRC_GPIO1);
  //auto value2 = stand->GetGPIOsync(currentCam, IRC_GPIO2);

  TestData testData1(IRC_TEST_NAME, value1, IRC_CORRECT_GPIO1);
  //TestData testData2(IRC_TEST_NAME2, value2, IRC_CORRECT_GPIO2);
  testResults[testData1.name] = testData1;
  //testResults[testData2.name] = testData2;
  emit parameterTestIsReady(currentCam, testData1);
}

void Tester::testIrBackLight() {
  stand->SetPWMsync(currentCam, IR_DIM_CDS_MID);
  int value = 50;
  //auto value = stand->GetADCsync(currentCam, IR_DIM_ADC3);

  TestData testData(IR_TEST_NAME, value, IR_DIM_MIN_ADC3, IR_DIM_MAX_ADC3);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testI2c() {
  QString outputString = TEST_STRING;
  //cameras[currentCam]->sendI2cSync(CAMERA_I2C, outputString);
  auto value = stand->ReceiveI2Csync(currentCam);

  TestData testData(I2C_TEST_NAME, value, outputString);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testIrisDiaphragm() {
  //cameras[currentCam]->setGpioSync(IRIS_PWM, 1);
  auto value = stand->GetADCsync(currentCam, IRIS_ADC4);

  TestData testData(IRIS_TEST_NAME, value, IRIS_MIN_ADC4, IRIS_MAX_ADC4);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testAudioInOut() {
  // TODO
  // Уточнить как правильно тестировать звуковые вход/выход

  //cameras[currentCam]->setDac(AUDIO_DAC, TEST_AUDIO_DAC_VALUE);
  //cameras[currentCam]->getAdc(AUDIO_ADC, adcValue);
  //emit parameterTestIsReady(currentCam, testData);
}

void Tester::testUart() {
  QString outputString = TEST_STRING;
  //cameras[currentCam]->sendUartSync(CAMERA_UART, outputString);
  auto value = stand->ReceiveUARTsync(currentCam);

  TestData testData(UART_TEST_NAME, value, outputString);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testAlarmInOut() {
  cameras[currentCam]->setGpioSync(ALARM_PIN_OUT, ALARM_ACTIVE_VALUE);
  auto value = cameras[currentCam]->getGpioSync(ALARM_PIN_IN);

  TestData testData(ALARM_TEST_NAME, value, ALARM_ACTIVE_VALUE);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testEthernetLed() {
  //cameras[currentCam]->setGpioSync(LED1_PIN, LED1_ACTIVE_VALUE);
  auto value = stand->GetGPIOsync(currentCam, LED1_GPIO3);

  TestData testData(LED1_TEST_NAME, value, LED1_ACTIVE_VALUE);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testReset() {
  stand->SetGPIOsync(currentCam, RESET_GPIO4, ACTIVE_RESET_LEVEL);

  TestData testData(RESET_TEST_NAME, LED1_ACTIVE_VALUE, LED1_ACTIVE_VALUE);
  testResults[testData.name] = testData;
  emit parameterTestIsReady(currentCam, testData);
}

void Tester::testCam(int currentCam) {
  setCurrentTestingCam(currentCam);

  testFocusMotor();
  testZoomMotor();
  testIrCurtain();
  testIrBackLight();
  testI2c();
  testIrisDiaphragm();
  testAudioInOut();
  testUart();
  testAlarmInOut();
  testEthernetLed();
  testReset();

  emit camTestIsReady(currentCam, testResults);
}

void Tester::setCurrentTestingCam(int currentCam) {
  //
  this->currentCam = currentCam;
}

CameraCommander *Tester::getCurrentTestingCam() { return cameras[currentCam]; }

void Tester::testAllCams() {
  camTests.clear();

  for (int i = 0; i < CAMERA_MAX_COUNT; ++i) {
    //
    testResults.clear();
    testCam(i);

    camTests.append(testResults);
  }
  //for (auto &cam : cameras) {}
}

void Tester::makeTestInSeparateThread() {
  //
}

QString Tester::getVideoStreamUrlOfCam(int currentCam) {
  if (currentCam >= cameras.count())
    return QString("file:/home/dmitriy/Videos/%1.mp4").arg(currentCam);

  QString address =
      QString("rtsp://admin:admin@%1/primary").arg(cameras[currentCam]->connector->getAddress());

  //  switch (currentCam) {
  //    case 1: return "rtsp://admin:admin@10.30.7.16/primary";
  //    case 2: return "rtsp://admin:admin@10.30.7.50/primary";
  //    case 3: return "rtsp://admin:admin@192.168.1.50/primary";
  //    case 4: return "rtsp://admin:admin@192.168.1.120/primary";

  //    case 1: return "/home/dmitriy/Videos/5.mp4";
  //    case 2: return "/home/dmitriy/Videos/6.mp4";
  //    case 3: return "/home/dmitriy/Videos/7.mp4";
  //    case 4: return "/home/dmitriy/Videos/8.mp4";
  //    case 5: return "/home/dmitriy/Videos/9.mp4";
  //    case 6: return "/home/dmitriy/Videos/10.mp4";
  //    default: return "/home/dmitriy/Videos/1.mp4";
  //  }

  return address;
}

void Tester::setCamIp(int camIndex, const QString &ip) {
  // TODO: Провести нормальную проверку на правильность ip адреса
  if (!ip.isEmpty()) { cameras[camIndex]->connector->m_ipAddress = ip; }
}

void Tester::setCamsIp(QStringList ips) {
  for (auto cam : cameras) {
    //
    cam->connector->m_ipAddress = ips.takeFirst();
  }
}

void Tester::setCamsPort(int port) {
  for (auto &camera : cameras) {
    //
    camera->connector->m_ipPort = port;
  }
}

void Tester::saveConfigs(const QString &script) {
  configFile.reset();
  configFile.write(script.toStdString().c_str());
  configFile.flush();
}

void Tester::openConfigs(const QString &script) {
  //
  configFile.setFileName(script);
  configFile.open(QIODevice::ReadWrite | QIODevice::Text);
}

void Tester::setStandIp(const QString &ip) { this->stand->connector->m_ipAddress = ip; }

void Tester::setStandPort(int port) { this->stand->connector->m_ipPort = port; }

QStringList Tester::getFilesListFromDir(const QString &dirName) {
  QStringList list;

  // Каталог со скриптами должен лежать в одном каталоге вместе с исполняемой программой
  testScriptDir = dirName;
  QDir dir(dirName);

  if (!dir.exists()) return list;

  QDirIterator it(dir);
  QFileInfo finfo;

  while (it.hasNext()) {
    finfo.setFile(it.next());
    QString fileName = finfo.fileName();
    if (fileName == "." || fileName == "..") continue;

    list.push_back(fileName);
  }

  return list;
}

QString Tester::getScriptTextFrom(const QString &scriptFile) {
  //
  currentFile  = scriptFile;
  QString path = QString("./%1/%2").arg(TEST_SCRIPTS_DIR).arg(scriptFile);
  configFile.setFileName(path);
  configFile.open(QIODevice::ReadWrite | QIODevice::Text);
  configFile.reset();
  return configFile.readAll();
}

void Tester::onReceivedDat(const QStringList values) {
  //
  camTests.clear();

  for (auto &value : values) {
    //
    TestData testData(CURRENT_TEST_NAME, (int)value.toDouble() * 1000, (int)CURRENT_MAX * 1000);
    testResults[testData.name] = testData;
    camTests.append(testResults);
  }

  int currentCam = 1;
  emit parameterTestIsReady(currentCam, camTests[1][CURRENT_TEST_NAME]);
}

bool Tester::isInRange(int value, int min, int max) {
  if ((min <= value) && (value <= max)) {
    return true;
  } else {
    return false;
  }
}
