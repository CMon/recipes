#pragma once

#include <QLocale>
#include <QString>

namespace Util
{
bool enableConsoleEcho(const bool enable);
bool enableConsoleBufferedIo(const bool enable);
QString enterPassword(const QString & prompt);
QString boolToString(const bool b);
}

typedef QHash<QLocale, QString> Locale2String;
uint qHash(const QLocale & locale);
