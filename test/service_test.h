#pragma once

#include <test/testserver.h>

#include <QTest>

class ServicesTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();

	void userServiceTest();

private:
	TestServer testServer_;
};
