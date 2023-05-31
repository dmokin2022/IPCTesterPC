#include "TerminalFrame.hpp"

#include "ui_TerminalFrame.h"

TerminalFrame::TerminalFrame(
    QString title, Connector *connector, QStringList addresses, QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::TerminalFrame) {
  ui->setupUi(this);

  updateAddressListWith(addresses);

  setWindowTitle(title);
  ui->lineEdit_message->setFocus();

  this->connector = connector;

  // Сигналы для UI
  connect(ui->pushButton_transmit, &QPushButton::clicked, this, &TerminalFrame::onButtonClicked);
  connect(ui->comboBox_address, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      &TerminalFrame::onChangeAddress);

  // Сигналы от внешних событий (приём/получение)
  connect(connector, &Connector::messageWasTransmitted, this, &TerminalFrame::onMessageTransmitted);
  connect(connector, &Connector::readyToBeParsed, this, &TerminalFrame::onMessageReceived);

  // Сигнал от внутреннего события отправки сообщения
  connect(this, &TerminalFrame::transmitMessage, connector, &Connector::send);
}

TerminalFrame::~TerminalFrame() { delete ui; }

void TerminalFrame::sendMessage(QString message) {
  connector->send(message);
  //emit transmitMessage(message);
  ui->lineEdit_message->clear();
  ui->lineEdit_message->setFocus();
}

void TerminalFrame::addMessageToWindow(QString message) {
  //
  ui->textEdit->append(message);
}

void TerminalFrame::updateAddressListWith(QStringList list) {
  ui->comboBox_address->clear();
  ui->comboBox_address->addItems(list);
}

void TerminalFrame::onButtonClicked() { sendMessage(ui->lineEdit_message->text()); }

void TerminalFrame::onChangeAddress() {
  //
  QString newAddress = ui->comboBox_address->currentText();
  connector->setAddress(newAddress);
}

void TerminalFrame::keyPressEvent(QKeyEvent *event) {
  int keyPressed  = event->key();
  QString message = ui->lineEdit_message->text();

  if (keyPressed == Qt::Key_Return) {
    //
    commandsStorage.append(message);
    sendMessage(message);
    lastCommand = commandsStorage.size() - 1;
  }
  if (keyPressed == Qt::Key_Down) {
    if (lastCommand < commandsStorage.size() - 1) {
      lastCommand++;
      QString line = commandsStorage[lastCommand];
      ui->lineEdit_message->setText(line);
    }
  }
  if (keyPressed == Qt::Key_Up) {
    if (lastCommand >= 0) {
      QString line = commandsStorage[lastCommand];
      ui->lineEdit_message->setText(line);
      lastCommand--;
    }
  }
}

void TerminalFrame::onMessageTransmitted(QString message) {
  //
  addMessageToWindow(">> " + message);
}

void TerminalFrame::onMessageReceived(QString message) {
  //
  addMessageToWindow("<< " + message);
}
