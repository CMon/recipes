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

	void unit_addOrUpdateUnit_getUnits();
	void unit_addOrUpdateCategory_getCategories();
};
