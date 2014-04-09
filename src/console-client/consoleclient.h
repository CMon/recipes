#pragma once

#include <console-client/console.h>

class QString;
class QStringList;

class ConsoleClient : public QObject
{
    Q_OBJECT
public:
    ConsoleClient(QObject *parent);

    void start();

private slots:
    void command(const QString & command, const QStringList & arguments);

private:
    Console console_;
};
