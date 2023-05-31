#include "Connector.hpp"

Connector::Connector(QString ip, int port, QString terminator, QObject *parent) : QObject {parent} {
  m_ipAddress      = ip;
  m_ipPort         = port;
  stringTerminator = terminator;

  tcpSocket = new QTcpSocket(this);
  tcpSocket->bind(m_ipPort);
  connect(tcpSocket, &QTcpSocket::readyRead, this, &Connector::readStringFromBuffer);
  tcpSocket->connectToHost(m_ipAddress, m_ipPort);
}

void Connector::setIp(const QString &ip) { m_ipAddress = ip; }

void Connector::setPort(int port) { m_ipPort = port; }

void Connector::send(QString string) {
  QByteArray array = string.toUtf8() + stringTerminator.toUtf8();
  qDebug() << "Отправлено:" << array;

  tcpSocket->write(array);
  emit messageWasTransmitted(string);
}

// Переподключение в случае смены ip-адреса через интерфейс
void Connector::setAddress(QString ip) {
  //
  //tcpSocket->disconnect();
  //tcpSocket->reset();
  this->m_ipAddress = ip;
  //tcpSocket->resume();
  tcpSocket->connectToHost(m_ipAddress, m_ipPort);
}

QString Connector::getAddress() { return m_ipAddress; }

QString Connector::readStringFromBuffer() {
  QByteArray array = tcpSocket->readAll();
  QString string(array);

  QStringList stringList = string.split("\r\n");
  qDebug() << "Принято:" << array;

  string = stringList.first();
  emit readyToBeParsed(string);

  return string;
}
