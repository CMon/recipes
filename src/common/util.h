#pragma once

#include <QString>

namespace Util
{
    bool enableConsoleEcho(const bool enable);
    bool enableConsoleBufferedIo(const bool enable);
    QString enterPassword(const QString & prompt);
}
