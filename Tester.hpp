#pragma once

#include <Command/CameraCommand.hpp>
#include <Command/StandCommand.hpp>
#include <QFile>
#include <QObject>

#include "ConfigInterpretator.hpp"
#include "TestData.hpp"

class Tester : public QObject {
  Q_OBJECT
public:
  explicit Tester(QObject *parent = nullptr);

  // Служебные функции
  //-----------------------------
  void setCamIp(int camIndex, const QString &ip);
  void setCamsIp(QStringList ips);
  void setCamsPort(int port);

  void setStandIp(const QString &ip);
  void setStandPort(int port);

  // Работа с файлами
  QStringList getFilesListFromDir(const QString &dirName);
  QString getScriptTextFrom(const QString &scriptFile);
  QString testScriptDir;
  QStringList testScriptFiles;
  QString currentFile;
  QFile configFile;

  // Тесты
  //--------------------------
  void testFocusMotor();
  void testZoomMotor();
  void testIrCurtain();
  void testIrBackLight();
  void testI2c();
  void testIrisDiaphragm();
  void testAudioInOut();
  void testUart();
  void testAlarmInOut();
  void testEthernetLed();
  void testReset();

  void testCam(int currentCam);
  void testAllCams();
  void testCamInSeparateThreadWith(const QString &script);

  // Подключение к аппаратуре
  StandCommander *stand;
  QVector<CameraCommander *> cameras;
  ConfigInterpretator *interpretator;
  QString getVideoStreamUrlOfCam(int currentCam);

signals:
  void camTestIsReady(int currentCam, QMap<QString, TestData> &testResult);
  void camTestIsReady_(int currentCam, QStringList &testResults);
  void parameterTestIsReady(int currentCam, TestData &testData);

public slots:
  void setCurrentTestingCam(int currentCam);
  void saveConfigs(const QString &script);
  void openConfigs(const QString &script);
  CameraCommander *getCurrentTestingCam();
  // Тестирование через файл настроек
  //-------------------------------------
  bool testCamWith(const QString &script);
  void testAllCams_();

private slots:
  void onReceivedDat(const QStringList values);

private:
  //QVector<CameraCommander *> cameras;

  int currentCam;

  int adcValue;
  int gpioValue;

  bool isInRange(int value, int min, int max);
  QMap<QString, TestData> testResults;
  QList<QMap<QString, TestData>> camTests;
};
