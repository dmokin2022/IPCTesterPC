#include <QRegularExpression>

#pragma once

namespace NetUtils {
class IpValidator {
public:
  static bool isIpValid(const QString &ip);

  IpValidator();

  bool validate(const QString &ip);
  bool validateWithIp0000(const QString &ip);

private:
  QRegularExpression m_re;
};
}  // namespace NetUtils
