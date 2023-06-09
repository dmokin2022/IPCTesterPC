#include "MainWindow.hpp"

#include <QtConcurrent/QtConcurrent>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>

#include "Configs/CameraConfig.h"
#include "Configs/Config.h"
#include "Configs/TestNames.h"
#include "TerminalFrame.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  tester = new Tester(this);
  connect(tester, &Tester::camTestIsReady, this, &MainWindow::onCamTestReady);

  setupConnectionsPage();
  setupAdvancedTestPage();
  setupAllCamsTestPage();
  setupSingleCamTestPage();

  cameraTerminal = new TerminalFrame(
      "Терминал камеры", tester->getCurrentTestingCam()->connector, getCamsIpFromUI());

  QStringList camNames;
  standTerminal = new TerminalFrame("Терминал стенда", tester->stand->connector, camNames);

  //  interpretator = new ConfigInterpretator(tester->stand, tester->cameras[0], this);
  //  interpretator->testVariableInit();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setButtonColorAcordingToTestResults(
    QPushButton *button, QMap<QString, TestData> &testResult) {
  Q_UNUSED(button)
  Q_UNUSED(testResult)
  //  if (testResult[0].isInRange) {
  //    button->setStyleSheet("QPushButton {background-color: green}");
  //  } else {
  //    button->setStyleSheet("QPushButton {background-color: red}");
  //  }
}

void MainWindow::on_pushButton_clicked() {
  //
  QtConcurrent::run(tester, &Tester::testAllCams);
}

void MainWindow::onCamTestReady(int currentCam, QMap<QString, TestData> &testResult) {
  switch (currentCam) {
    case 1: setButtonColorAcordingToTestResults(ui->pushButton_cam1, testResult); break;
    case 2: setButtonColorAcordingToTestResults(ui->pushButton_cam2, testResult); break;
    case 3: setButtonColorAcordingToTestResults(ui->pushButton_cam3, testResult); break;
    case 4: setButtonColorAcordingToTestResults(ui->pushButton_cam4, testResult); break;
    case 5: setButtonColorAcordingToTestResults(ui->pushButton_cam5, testResult); break;
    case 6: setButtonColorAcordingToTestResults(ui->pushButton_cam6, testResult); break;
  }
}

void MainWindow::onCamTestReady_(int currentCam, QStringList &testResult) {
  //testResult.
  //ui->textEdit_testResults->setText(testResult.);
}

void MainWindow::onParameterTestReady(int currentCam, TestData &testData) {
  //
  Q_UNUSED(currentCam)
  pviews[testData.name]->showParameterTestResults(testData);
}

void MainWindow::onTestRequestCompleted(const QString result) {}

void MainWindow::onButtonCamXClicked() {
  // Получаем указатель на пославший сигнал объект (кнопку)
  QPushButton *senderButton = static_cast<QPushButton *>(sender());

  ui->comboBox_cams_2->setCurrentText(senderButton->text());
  ui->tabWidget->setCurrentWidget(ui->tab_SingleTest);
}

void MainWindow::setupConnectionsPage() {
  ui->lineEdit_cam1Ip->setText(CAMERA1_IP_ADDRESS);
  ui->lineEdit_cam2Ip->setText(CAMERA2_IP_ADDRESS);
  ui->lineEdit_cam3Ip->setText(CAMERA3_IP_ADDRESS);
  ui->lineEdit_cam4Ip->setText(CAMERA4_IP_ADDRESS);
  ui->lineEdit_cam5Ip->setText(CAMERA5_IP_ADDRESS);
  ui->lineEdit_cam6Ip->setText(CAMERA6_IP_ADDRESS);
  ui->lineEdit_camsPort->setText(QString::number(CAMERA_IP_PORT));

  ui->lineEdit_standIp->setText(STAND_IP_ADDRESS);
  ui->lineEdit_standPort->setText(QString::number(STAND_IP_PORT));

  updateConnections();
}

void MainWindow::setupAdvancedTestPage() {
  for (int i = 0; i < CAMERA_MAX_COUNT; i++) {
    ui->comboBox_cams->addItem(QString("Camera %1").arg(i), i);
  }
  // Есть 2 разных сигнала currentIndexChanged с разными аргументами, поэтому нужно конкретизировать
  // какой именно сигнал (с какими аргументами) использовать
  ui->comboBox_cams->setCurrentIndex(0);
  connect(ui->comboBox_cams, QOverload<int>::of(&QComboBox::currentIndexChanged), tester,
      &Tester::setCurrentTestingCam);

  TestData defaultData;
  ParameterView *focusView = new ParameterView(ui->label_motor_focus_name, ui->label_motor_focus,
      ui->lineEdit_motor_focus_threshold, nullptr, ui->pushButton_motor_focus, defaultData, this);

  ParameterView *zoomView = new ParameterView(ui->label_motor_zoom_name, ui->label_motor_zoom,
      ui->lineEdit_motor_zoom_threshold, nullptr, ui->pushButton_motor_zoom, defaultData, this);

  ParameterView *ircView = new ParameterView(ui->label_IRC_name, ui->label_IRC_GPIO1, nullptr,
      nullptr, ui->pushButton_IRC, defaultData, this);

  ParameterView *irView = new ParameterView(ui->label_IR_name, ui->label_IR_DIM_ADC3, nullptr,
      nullptr, ui->pushButton_IR, defaultData, this);

  ParameterView *i2cView = new ParameterView(ui->label_I2C_name, nullptr, ui->lineEdit_I2C_message,
      nullptr, ui->pushButton_I2C, defaultData, this);

  ParameterView *irisView = new ParameterView(ui->label_IRIS_name, ui->label_IRIS,
      ui->lineEdit_IRIS_threshold, nullptr, ui->pushButton_IRIS, defaultData, this);

  ParameterView *uartView = new ParameterView(ui->label_UART_name, nullptr,
      ui->lineEdit_UART_message, nullptr, ui->pushButton_UART, defaultData, this);

  ParameterView *ledView = new ParameterView(ui->label_LED1_name, ui->label_LED1, nullptr, nullptr,
      ui->pushButton_UART, defaultData, this);

  ParameterView *resetView = new ParameterView(ui->label_RESET_name, ui->label_RESET, nullptr,
      nullptr, ui->pushButton_RESET, defaultData, this);

  // Порядок расположения важен
  pviews[FOCUS_TEST_NAME] = focusView;  // нулевая камера игнорируется. Нумерация начинается с 1
  pviews[ZOOM_TEST_NAME]  = zoomView;
  pviews[IRC_TEST_NAME]   = ircView;
  pviews[IR_TEST_NAME]    = irView;
  pviews[I2C_TEST_NAME]   = i2cView;
  pviews[IRIS_TEST_NAME]  = irisView;
  pviews[UART_TEST_NAME]  = uartView;
  pviews[LED1_TEST_NAME]  = ledView;
  pviews[RESET_TEST_NAME] = resetView;

  connect(ui->pushButton_motor_focus, &QPushButton::clicked, tester, &Tester::testFocusMotor);
  connect(ui->pushButton_motor_zoom, &QPushButton::clicked, tester, &Tester::testZoomMotor);
  connect(ui->pushButton_IRC, &QPushButton::clicked, tester, &Tester::testIrCurtain);
  connect(ui->pushButton_IR, &QPushButton::clicked, tester, &Tester::testIrBackLight);
  connect(ui->pushButton_I2C, &QPushButton::clicked, tester, &Tester::testI2c);
  connect(ui->pushButton_IRIS, &QPushButton::clicked, tester, &Tester::testIrisDiaphragm);
  connect(ui->pushButton_UART, &QPushButton::clicked, tester, &Tester::testUart);
  connect(ui->pushButton_LED1, &QPushButton::clicked, tester, &Tester::testEthernetLed);
  connect(ui->pushButton_RESET, &QPushButton::clicked, tester, &Tester::testReset);

  connect(ui->pushButton_showLogo, &QPushButton::clicked, tester->stand, &StandCommander::ShowLogo);

  connect(tester, &Tester::parameterTestIsReady, this, &MainWindow::onParameterTestReady);
}

void MainWindow::setupAllCamsTestPage() {
  ui->pushButton_cam1->setStyleSheet("QPushButton {background-color: red}");
  ui->pushButton_cam2->setStyleSheet("QPushButton {background-color: green}");
  ui->pushButton_cam3->setStyleSheet("QPushButton {background-color: green}");
  ui->pushButton_cam4->setStyleSheet("QPushButton {background-color: green}");
  ui->pushButton_cam5->setStyleSheet("QPushButton {background-color: green}");
  ui->pushButton_cam6->setStyleSheet("QPushButton {background-color: green}");

  connect(ui->pushButton_cam1, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);
  connect(ui->pushButton_cam2, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);
  connect(ui->pushButton_cam3, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);
  connect(ui->pushButton_cam4, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);
  connect(ui->pushButton_cam5, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);
  connect(ui->pushButton_cam6, &QPushButton::clicked, this, &MainWindow::onButtonCamXClicked);

  setupCamVideoFrame(0, ui->frame_video_cam1);
  setupCamVideoFrame(1, ui->frame_video_cam2);
  setupCamVideoFrame(6, ui->frame_video_cam3);
  setupCamVideoFrame(7, ui->frame_video_cam4);
  setupCamVideoFrame(8, ui->frame_video_cam5);
  setupCamVideoFrame(9, ui->frame_video_cam6);

  //  QLayout *layout = new QHBoxLayout;
  //  ui->frame_video_cam6->setLayout(layout);
  //  QVideoWidget *videoWidget = new QVideoWidget(this);

  //  layout->addWidget(videoWidget);

  //  QMediaPlayer *player_cam = new QMediaPlayer(this);
  //  player_cam->setMedia(QUrl::fromEncoded("rtsp://admin:admin@10.30.7.16/primary"));
  //  player_cam->setVideoOutput(videoWidget);
  //  player_cam->play();
}

void MainWindow::setupSingleCamTestPage() {
  for (int i = 0; i < CAMERA_MAX_COUNT; i++) {
    ui->comboBox_cams_2->addItem(QString("Камера %1").arg(i), i);
  }
  connect(ui->comboBox_cams_2, QOverload<int>::of(&QComboBox::currentIndexChanged), tester,
      &Tester::setCurrentTestingCam);

  ui->comboBox_settingsFiles->addItems(tester->getFilesListFromDir(TEST_SCRIPTS_DIR));
  //ui->comboBox_settingsFiles->addItem("добавить...");
  ui->comboBox_settingsFiles->setCurrentIndex(0);
  //connect(ui->comboBox_settingsFiles, &QComboBox::currentTextChanged, tester, &Tester::openConfigs);
  connect(ui->comboBox_settingsFiles, &QComboBox::currentTextChanged, this, [this]() {
    QString text = tester->getScriptTextFrom(ui->comboBox_settingsFiles->currentText());
    ui->textEdit_testSettings->setText(text);
  });

  ui->textEdit_testSettings->setText(
      tester->getScriptTextFrom(ui->comboBox_settingsFiles->currentText()));

  ui->textEdit_testResults->setText("");
  //connect(this->tester->interpretator, &ConfigInterpretator::oneRequestIsCompleted, this, &MainWindow::onTestRequestCompleted);
  connect(tester->interpretator, &ConfigInterpretator::oneRequestIsCompleted, this,
      [this](const QString result) { ui->textEdit_testResults->append(result); });

  connect(ui->pushButton_saveSettings, &QPushButton::clicked, this,
      [this]() { tester->saveConfigs(ui->textEdit_testSettings->toPlainText()); });
  connect(ui->pushButton_testCam, &QPushButton::clicked, tester, [this]() {
    ui->textEdit_testResults->append("");
    QString resultsHeader = QString("%1: скрипт %2")
                                .arg(ui->comboBox_cams_2->currentText())
                                .arg(ui->comboBox_settingsFiles->currentText());
    ui->textEdit_testResults->append(resultsHeader);
    ui->textEdit_testResults->append("---------------------------------------------------");
    //tester->testCamInSeparateThreadWith(ui->textEdit_testSettings->toPlainText());
    tester->testCamWith(ui->textEdit_testSettings->toPlainText());
  });
}

QMediaPlayer *MainWindow::setupCamVideoFrame(int CurrentCam, QFrame *videoFrame) {
  QLayout *layout = new QHBoxLayout;
  videoFrame->setLayout(layout);

  QVideoWidget *videoWidget = new QVideoWidget(this);
  layout->addWidget(videoWidget);

  QMediaPlayer *player = new QMediaPlayer(this);
  //QUrl::fromEncoded("rtsp://admin:admin@10.30.7.16/primary")
  QString url = tester->getVideoStreamUrlOfCam(CurrentCam);
  player->setMedia(QUrl::fromEncoded(QByteArray(url.toStdString().c_str())));
  player->setVideoOutput(videoWidget);
  player->play();

  return player;
}

void MainWindow::updateConnections() {
  tester->setCamsIp(getCamsIpFromUI());

  tester->setCamsPort(ui->lineEdit_camsPort->text().toInt());

  tester->setStandIp(ui->lineEdit_standIp->text());
  tester->setStandPort(ui->lineEdit_camsPort->text().toInt());
}

QStringList MainWindow::getCamsIpFromUI() {
  QStringList ips;
  ips.append(ui->lineEdit_cam1Ip->text());
  ips.append(ui->lineEdit_cam2Ip->text());
  ips.append(ui->lineEdit_cam3Ip->text());
  ips.append(ui->lineEdit_cam4Ip->text());
  ips.append(ui->lineEdit_cam5Ip->text());
  ips.append(ui->lineEdit_cam6Ip->text());

  return ips;
}

void MainWindow::on_tabWidget_currentChanged(int index) {
  Q_UNUSED(index)
  this->updateConnections();
}

void MainWindow::on_pushButton_cameraTerminal_clicked() {
  if (cameraTerminal) {
    cameraTerminal->updateAddressListWith(getCamsIpFromUI());
    cameraTerminal->show();
  }
}

void MainWindow::on_pushButton_standTerminal_clicked() {
  if (standTerminal) standTerminal->show();
}
