#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QPushButton>
#include <QtCore/QMap>

#include "ConfigInterpretator.hpp"
#include "ParameterView.hpp"
#include "TerminalFrame.hpp"
#include "Tester.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setButtonColorAcordingToTestResults(QPushButton *button, QList<TestData> &testResult);
  void setButtonColorAcordingToTestResults(
      QPushButton *button, QMap<QString, TestData> &testResult);

  void setupConnectionsPage();
  void setupAdvancedTestPage();
  void setupAllCamsTestPage();
  void setupSingleCamTestPage();

  QMediaPlayer *setupCamVideoFrame(int CurrentCam, QFrame *videoFrame);

  void updateConnections();
  QStringList getCamsIpFromUI();

private slots:
  void on_pushButton_clicked();
  void onCamTestReady(int currentCam, QMap<QString, TestData> &testResult);
  void onCamTestReady_(int currentCam, QStringList &testResult);
  void onParameterTestReady(int currentCam, TestData &testData);
  void onTestRequestCompleted(const QString result);
  void onButtonCamXClicked();
  void on_tabWidget_currentChanged(int index);

  void on_pushButton_cameraTerminal_clicked();

  void on_pushButton_standTerminal_clicked();

private:
  Ui::MainWindow *ui;
  TerminalFrame *standTerminal;
  TerminalFrame *cameraTerminal;
  Tester *tester;
  ConfigInterpretator *interpretator;
  QMap<QString, ParameterView *> pviews;
};
