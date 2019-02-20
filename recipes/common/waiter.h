#pragma once

#include <QEventLoop>
#include <QTimer>

class Waiter : public QObject
{
	Q_OBJECT
public:
	Waiter(QObject* parent = nullptr);

	bool wait(int timeoutInMSecs = -1);
	bool timedOut() { return timedOut_; }

public slots:
	void stop(bool retValue = true);
	void succeed() { stop(true); }
	void fail() { stop(false); }

private slots:
	void slotTimeout();

private:
	QEventLoop eventLoop_;
	QTimer timer_;
	bool timedOut_;
};
