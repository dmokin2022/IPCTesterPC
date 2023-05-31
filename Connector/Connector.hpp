#pragma once

#include <QDataStream>
#include <QObject>
#include <QtNetwork/QTcpSocket>

#include "Configs/StandConfig.h"

class Connector : public QObject {
  Q_OBJECT
public:
  explicit Connector(
      QString ip, int port = 1000, QString terminator = "", QObject *parent = nullptr);

  void setIp(const QString &ip);
  void setPort(int port);

  QString m_ipAddress;
  int m_ipPort;

signals:
  void readyToBeParsed(const QString string);
  void messageWasTransmitted(const QString string);

public slots:
  QString readStringFromBuffer();
  void send(QString string);
  void setAddress(QString ip);
  QString getAddress();

private:
  QTcpSocket *tcpSocket;
  QString stringTerminator;
  //QDataStream *in;
};
