#pragma once

#include <QTest>

class DatabaseTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();

	void user_updateUser_getAllUsers();
	void user_updateUser_getUser();
	void user_checkPassword();

	void addOrUpdateUnit_getUnits();
	void addOrUpdateCategory_getCategories();
};
