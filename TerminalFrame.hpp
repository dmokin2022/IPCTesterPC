#pragma once

#include <QFrame>
#include <QKeyEvent>

#include "Connector/Connector.hpp"

namespace Ui {
class TerminalFrame;
}

class TerminalFrame : public QFrame {
  Q_OBJECT

public:
  explicit TerminalFrame(
      QString title, Connector *connector, QStringList addresses, QWidget *parent = nullptr);
  ~TerminalFrame();
  void sendMessage(QString message);
  void addMessageToWindow(QString message);
  void updateAddressListWith(QStringList list);

protected:  //keyboard
  void keyPressEvent(QKeyEvent *event);

public slots:
  void onMessageTransmitted(QString message);
  void onMessageReceived(QString message);
  void onButtonClicked();
  void onChangeAddress();

signals:
  void transmitMessage(QString message);

private:
  Ui::TerminalFrame *ui;
  Connector *connector;

  QStringList commandsStorage;
  int lastCommand;
};
