#include "IpValidator.hpp"

namespace NetUtils {

bool IpValidator::isIpValid(const QString &ip) { return IpValidator().validate(ip); }

IpValidator::IpValidator() {
  m_re.setPattern(
      // Начало строки.
      // 1-е число: (1 - 255) потом '.'
      // 2-е, 3-е число: (0 - 255) потом '.'
      // 4-е число: (0 - 255)
      // Конец строки.
      "^"
      "([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\."
      "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){2}"
      "([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])"
      "$");
}

bool IpValidator::validate(const QString &ip) {
  const QRegularExpressionMatch match = m_re.match(ip);
  return match.hasMatch();
}

bool IpValidator::validateWithIp0000(const QString &ip) {
  if (ip == "0.0.0.0") { return true; }

  return validate(ip);
}

}  // namespace NetUtils
