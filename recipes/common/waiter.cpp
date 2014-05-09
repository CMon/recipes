#include "waiter.h"

#include <QAbstractEventDispatcher>

Waiter::Waiter(QObject* parent)
    : QObject(parent),
      timedOut_(false)
{
    timer_.setSingleShot(true);
    connect(&timer_, SIGNAL(timeout()), SLOT(slotTimeout()));
}

bool Waiter::wait(int timeoutInMSecs)
{
    timedOut_ = false;
    if (timeoutInMSecs != -1) {
        timer_.start(timeoutInMSecs);
    }
    return eventLoop_.exec(QEventLoop::ExcludeUserInputEvents) == 0;
}

void Waiter::stop(bool retValue)
{
    timer_.stop();
    QAbstractEventDispatcher * eventDispatcher = QAbstractEventDispatcher::instance();
    if (eventDispatcher) {
        while (eventDispatcher->processEvents(QEventLoop::AllEvents));
    }
    eventLoop_.exit(retValue ? 0 : 1);
}

void Waiter::slotTimeout()
{
    timedOut_ = true;
    eventLoop_.exit(1);
}
