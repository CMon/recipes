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

uint qHash(const QLocale & locale);
