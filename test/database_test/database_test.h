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
	void addOrUpdateIngredient_getIngredients();
	void addOrUpdatePortion_getPortions();
	void addOrUpdateRecipe_getRecipes();

	void permission_addPermission_data();
	void permission_addPermission();
	void permission_setDescription_data();
	void permission_setDescription();
	void permission_setTitle_data();
	void permission_setTitle();

	void permission_setTitleAndDescription_data();
	void permission_setTitleAndDescription();
};
