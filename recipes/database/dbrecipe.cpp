#include "dbrecipe.h"

#include <recipes/database/dbuser.h>
#include <recipes/database/database.h>

#include <QHash>
#include <QLocale>
#include <QUuid>
#include <QVariantList>

// units

static int getUnitId(const Locale2String & abbreviations)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "unitId "
	            "FROM "
	              "units_i18n "
	            "WHERE "
	             "language = :language AND abbreviation = :abbreviation"
	            );

	int retval = -1;
	for (const QLocale & lang: abbreviations.keys()) {
		query.bindValue(":language",     lang.name());
		query.bindValue(":abbreviation", abbreviations.value(lang));

		if (!Database::executeQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateUnitTranslations(const int unitId, const Unit & unit)
{
	TRANSACTION(ta);

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "units_i18n "
	        "("
	          "unitId, language, abbreviation "
	        ") VALUES ("
	          ":unitId, :language, :abbreviation "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "abbreviation=VALUES(abbreviation) "
	            );

	queryAbbrev.bindValue(":unitId", unitId);

	Locale2String abbrevs = unit.getAbbreviations();
	for (const QLocale & locale: abbrevs.keys()) {
		queryAbbrev.bindValue(":language",     locale.name());
		queryAbbrev.bindValue(":abbreviation", abbrevs.value(locale));
		Database::executeQuery(queryAbbrev);
	}

	QSqlQuery queryName(ta.db);
	queryName.prepare(
	        "INSERT INTO "
	          "units_i18n "
	        "("
	          "unitId, language, completeName "
	        ") VALUES ("
	          ":unitId, :language, :completeName "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "completeName=VALUES(completeName) "
	            );

	queryName.bindValue(":unitId", unitId);

	Locale2String names = unit.getCompleteNames();
	for (const QLocale & locale: abbrevs.keys()) {
		queryName.bindValue(":language",     locale.name());
		queryName.bindValue(":completeName", names.value(locale));
		Database::executeQuery(queryName);
	}

	return ta.commit();
}

void DB::addOrUpdateUnit(const Unit & unit)
{
	TRANSACTION(ta);

	int unitId = getUnitId(unit.getAbbreviations());

	QString queryString;
	if (unitId == -1) {
		queryString =
		        "INSERT INTO "
		          "units "
		        "("
		          "inGram, inMl"
		        ") VALUES ("
		          ":inGram, :inMl"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "units "
		        "SET "
		          "inGram=:inGram, "
		          "inMl=:inMl "
		        "WHERE id=:unitId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":inGram", unit.getGram());
	query.bindValue(":inMl",   unit.getMl());
	if (unitId != -1) {
		query.bindValue(":unitId", unitId);
	}
	if(!Database::executeQuery(query)) return;

	if (unitId == -1) {
		unitId = query.lastInsertId().toInt();
	}

	if (!addOrUpdateUnitTranslations(unitId, unit)) return;

	ta.commit();
}

QList<Unit> DB::getUnits(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	            "SELECT "
	              "u.id, u.inGram, u.inMl, i18n.language, i18n.abbreviation, i18n.completeName "
	            "FROM "
	              "units u, units_i18n i18n "
	            "WHERE "
	               "u.id = i18n.unitId"
	            ;

	if (id != -1) {
		queryStr += " AND u.id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!Database::executeQuery(query)) return QList<Unit>();

	QHash<int, Unit> retval;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(3).toString());

		Unit unit;
		if (retval.contains(id)) {
			unit = retval.value(id);
		} else {
			unit = Unit(query.value(1).toInt(), query.value(2).toInt());
		}
		unit.updateAbbreviation(lang, query.value(4).toString());
		unit.updateCompleteName(lang, query.value(5).toString());

		retval.insert(id, unit);
	}

	ta.commit();

	return retval.values();
}

// categories

static int getCategoryId(const Locale2String & names)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "categoryId "
	            "FROM "
	              "categories_i18n "
	            "WHERE "
	              "language = :language AND name = :name"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language", lang.name());
		query.bindValue(":name",     names.value(lang));
		if (!Database::executeQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateCategoryTranslation(const int categoryId, const Category & category)
{
	TRANSACTION(ta);

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "categories_i18n "
	        "("
	          "categoryId, language, name "
	        ") VALUES ("
	          ":categoryId, :language, :name "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "name=VALUES(name) "
	            );

	queryAbbrev.bindValue(":categoryId", categoryId);

	Locale2String names = category.getNames();
	foreach(const QLocale & locale, names.keys()) {
		queryAbbrev.bindValue(":language", locale.name());
		queryAbbrev.bindValue(":name",     names.value(locale));
		Database::executeQuery(queryAbbrev);
	}

	return ta.commit();
}

void DB::addOrUpdateCategory(const Category & category)
{
	TRANSACTION(ta);

	int categoryId = getCategoryId(category.getNames());

	QString queryString;
	if (categoryId == -1) {
		queryString =
		        "INSERT INTO "
		          "categories "
		        "("
		          "isFoodCategory"
		        ") VALUES ("
		          ":isFoodCategory"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "categories "
		        "SET "
		          "isFoodCategory=:isFoodCategory "
		        "WHERE id=:categoryId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":isFoodCategory", category.isFoodCategory());
	if (categoryId != -1) {
		query.bindValue(":categoryId", categoryId);
	}
	if(!Database::executeQuery(query)) return;

	if (categoryId == -1) {
		categoryId = query.lastInsertId().toInt();
	}

	if (!addOrUpdateCategoryTranslation(categoryId, category)) return;

	ta.commit();
}

QList<Category> DB::getCategories(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	        "SELECT "
	          "c.id, c.isFoodCategory, i18n.language, i18n.name "
	        "FROM "
	          "categories c, categories_i18n i18n "
	        "WHERE "
	          "c.id = i18n.categoryId";

	if (id != -1) {
		queryStr += " AND c.id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!Database::executeQuery(query)) return QList<Category>();

	QHash<int, Category> retval;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(2).toString());

		Category category;
		if (retval.contains(id)) {
			category = retval.value(id);
		} else {
			category = Category(query.value(1).toBool());
		}
		category.updateName(lang, query.value(3).toString());

		retval.insert(id, category);
	}

	ta.commit();
	return retval.values();
}

// Ingridients

static int getIngredientId(const Locale2String & names)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "ingredientId "
	            "FROM "
	              "ingredients_i18n "
	            "WHERE "
	              "language = :language AND name = :name"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language", lang.name());
		query.bindValue(":name",     names.value(lang));
		if (!Database::executeQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateIngredientTranslation(const int ingredientId, const Ingredient & ingredient)
{
	TRANSACTION(ta);

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "ingredients_i18n "
	        "("
	          "ingredientId, language, name "
	        ") VALUES ("
	          ":ingredientId, :language, :name "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "name=VALUES(name) "
	            );

	queryAbbrev.bindValue(":ingredientId", ingredientId);

	Locale2String names = ingredient.getNames();
	foreach(const QLocale & locale, names.keys()) {
		queryAbbrev.bindValue(":language", locale.name());
		queryAbbrev.bindValue(":name",     names.value(locale));
		Database::executeQuery(queryAbbrev);
	}

	return ta.commit();
}

void DB::addOrUpdateIngredient(const Ingredient & ingredient)
{
	TRANSACTION(ta);

	int ingredientId = getIngredientId(ingredient.getNames());

	QString queryString;
	if (ingredientId == -1) {
		queryString =
		        "INSERT INTO "
		          "ingredients "
		        "("
		          "isLiquid, foodCategoryId, containsGluten, containsLactose"
		        ") VALUES ("
		          ":isLiquid, :foodCategoryId, :containsGluten, :containsLactose"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "ingredients "
		        "SET "
		          "isLiquid=:isLiquid, "
		          "foodCategoryId=:foodCategoryId, "
		          "containsGluten=:containsGluten, "
		          "containsLactose=:containsLactose "
		        "WHERE id=:categoryId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":isLiquid",        ingredient.getIsLiquid());
	query.bindValue(":foodCategoryId",  getCategoryId(ingredient.getFoodCategory().getNames()));
	query.bindValue(":containsGluten",  ingredient.getContainsGluten());
	query.bindValue(":containsLactose", ingredient.getContainsLactose());
	if (ingredientId != -1) {
		query.bindValue(":categoryId", ingredientId);
	}
	if(!Database::executeQuery(query)) return;

	if (ingredientId == -1) {
		ingredientId = query.lastInsertId().toInt();
	}

	if (!addOrUpdateIngredientTranslation(ingredientId, ingredient)) return;

	ta.commit();
}

QList<Ingredient> DB::getIngredients(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	            "SELECT "
	              "i.id, i.foodCategoryId, i.isLiquid, i.containsGluten, i.containsLactose, i18n.language, i18n.name "
	            "FROM "
	              "ingredients i, ingredients_i18n i18n "
	            "WHERE "
	               "i.id = i18n.ingredientId"
	            ;

	if (id != -1) {
		queryStr += " AND i.id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!Database::executeQuery(query)) return QList<Ingredient>();

	QHash<int, Ingredient> retval;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(5).toString());

		Ingredient ingredient;
		if (retval.contains(id)) {
			ingredient = retval.value(id);
		} else {
			ingredient = Ingredient(query.value(2).toBool(), query.value(3).toBool(), query.value(4).toBool());
			ingredient.setFoodCategory(DB::getCategories(query.value(1).toInt()).first());
		}
		ingredient.updateName(lang, query.value(6).toString());

		retval.insert(id, ingredient);
	}

	ta.commit();

	return retval.values();
}

// portions

static int getPortionId(const Locale2String & names)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "portionId "
	            "FROM "
	              "portions_i18n "
	            "WHERE "
	              "language = :language AND description = :description"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language",    lang.name());
		query.bindValue(":description", names.value(lang));
		if (!Database::executeQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdatePortionTranslation(const int portionId, const Portion & portion)
{
	TRANSACTION(ta);

	QSqlQuery queryDescriptions(ta.db);
	queryDescriptions.prepare(
	        "INSERT INTO "
	          "portions_i18n "
	        "("
	          "portionId, language, description "
	        ") VALUES ("
	          ":portionId, :language, :description "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "description=VALUES(description) "
	            );

	queryDescriptions.bindValue(":portionId", portionId);

	Locale2String names = portion.getDescriptions();
	foreach(const QLocale & locale, names.keys()) {
		queryDescriptions.bindValue(":language",    locale.name());
		queryDescriptions.bindValue(":description", names.value(locale));
		Database::executeQuery(queryDescriptions);
	}

	return ta.commit();
}

void DB::addOrUpdatePortion(const Portion & portion)
{
	TRANSACTION(ta);

	int portionId = getPortionId(portion.getDescriptions());

	QString queryString;
	if (portionId == -1) {
		QSqlQuery query(ta.db);
		query.prepare("INSERT INTO portions () VALUES()");
		if(!Database::executeQuery(query)) return;
		portionId = query.lastInsertId().toInt();
	}

	if (!addOrUpdatePortionTranslation(portionId, portion)) return;

	ta.commit();
}

QList<Portion> DB::getPortions(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	            "SELECT "
	              "p.id, i18n.language, i18n.description "
	            "FROM "
	              "portions p, portions_i18n i18n "
	            "WHERE "
	               "p.id = i18n.portionId";

	if (id != -1) {
		queryStr += " AND p.id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!Database::executeQuery(query)) return QList<Portion>();

	QHash<int, Portion> retval;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(1).toString());

		Portion portion;
		if (retval.contains(id)) {
			portion = retval.value(id);
		} else {
			portion = Portion();
		}
		portion.updateDescriptions(lang, query.value(2).toString());

		retval.insert(id, portion);
	}

	ta.commit();
	return retval.values();
}

class Instructions
{
public:
	void clear() { steps.clear(); }
	void setPathToImg(quint8 pos, const QString & pathToImg) { steps[pos].setPicturePath(pathToImg); }
	void addDescription(quint8 pos, const QString & lang, const QString & description) { steps[pos].getDescription().add(QLocale(lang), description); }
	QList<InstructionStep> toList() const { return steps.values(); }
private:
	QMap<quint8, InstructionStep> steps;
};

bool getInstructionSteps(const int recipeId, Recipe & recipe)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "position, language, description, pathToImg "
	            "FROM "
	              "instructionStep "
	            "WHERE "
	              "recipeId = :recipeId"
	            );
	query.bindValue("recipeId", recipeId);

	if (!Database::executeQuery(query)) return false;

	Instructions steps;
	while (query.next()) {
		const quint8 position = quint8(query.value(0).toUInt());
		steps.addDescription(position, query.value(1).toString(), query.value(2).toString());
		steps.setPathToImg(position, query.value(3).toString());
	}
	recipe.setInstructionSteps(steps.toList());

	return ta.commit();
}

bool setInstructionSteps(const int recipeId, const QList<InstructionStep> & steps)
{
	TRANSACTION(ta);
	QSqlQuery queryRemoveOld(ta.db);
	queryRemoveOld.prepare("DELETE FROM instructionStep WHERE recipeId = :recipeId");

	if (!Database::executeQuery(queryRemoveOld)) return false;

	QSqlQuery queryInsert(ta.db);
	queryInsert.prepare(
	            "INSERT INTO "
	              "instructionStep "
	            "("
	              "recipeId, position, language, description, pathToImg "
	            ") VALUES ("
	              ":recipeId, :position, :language, :description, :pathToImg "
	            ") "
	            );

	queryInsert.bindValue(":recipeId", recipeId);

	for (const InstructionStep & step : steps) {
		queryInsert.bindValue(":position", step.getPosition());
		queryInsert.bindValue(":pathToImg", step.getPicturePath());

		const Locale2String descriptions = step.getDescription();
		for(const QLocale & locale: descriptions.keys()) {
			queryInsert.bindValue(":language",    locale.name());
			queryInsert.bindValue(":description", descriptions.value(locale));
			if (!Database::executeQuery(queryInsert)) return false;
		}
	}

	ta.commit();
	return true;
}

// recipes

static int addRecipe(Recipe & recipe)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	        "INSERT IGNORE INTO "
	          "recipes "
	        "("
	          "externId, portionId, portionCount, createdByUserId"
	        ") VALUES ("
	          ":externId, :portionId, :portionCount, :createdByUserId"
	        ")"
	            );

	query.bindValue(":portionId",       getPortionId(recipe.getPortion().getDescriptions()));
	query.bindValue(":portionCount",    recipe.getPortion().getCount());
	query.bindValue(":createdByUserId", DB::getUserId(recipe.getCreatedByUser().getLogin()).toDatabaseValue());
	QString externId;
	do {
		externId = QUuid::createUuid().toString();
		query.bindValue(":externId", externId);
		Database::executeQuery(query);
	} while(query.numRowsAffected() == 0);

	if (!ta.commit()) return -1;

	recipe.setExternId(externId);
	return query.lastInsertId().toInt();
}

static void updateRecipe(const Recipe & recipe)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	            "UPDATE "
	              "recipes "
	            "SET "
	              "portionId=:portionId, "
	              "portionCount=:portionCount, "
	              "createdByUserId=:createdByUserId "
	            "WHERE externId=:externId "
	        );

	query.bindValue(":externId",        recipe.getExternId());
	query.bindValue(":portionId",       getPortionId(recipe.getPortion().getDescriptions()));
	query.bindValue(":portionCount",    recipe.getPortion().getCount());
	query.bindValue(":createdByUserId", DB::getUserId(recipe.getCreatedByUser().getLogin()).toDatabaseValue());

	Database::executeQuery(query);
	ta.commit();
}

static int getRecipeId(const QString & externId)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "id "
	            "FROM "
	              "recipes "
	            "WHERE "
	               "externId=:externId");

	query.bindValue(":externId", externId);

	int retval = -1;
	if (!Database::executeQuery(query)) return retval;

	if (query.next()) {
		retval = query.value(0).toInt();
	}

	ta.commit();
	return retval;
}

static void addOrUpdateRecipeTranslations(const int recipeId, const Recipe & recipe)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	        "INSERT INTO "
	          "titles_i18n "
	        "("
	          "recipeId, language, title "
	        ") VALUES ("
	          ":recipeId, :language, :title "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "title=VALUES(title) "
	            );

	query.bindValue(":recipeId", recipeId);

	const Locale2String titles = recipe.getTitle();
	foreach(const QLocale & locale, titles.keys()) {
		query.bindValue(":language", locale.name());
		query.bindValue(":title",    titles.value(locale));
		Database::executeQuery(query);
	}

	ta.commit();
}

static void addOrUpdateRecipeIngredients(const int recipeId, const Recipe & recipe)
{
	TRANSACTION(ta);

	QSqlQuery delQuery(ta.db);
	delQuery.prepare(
	            "DELETE FROM "
	              "recipeToIngredient "
	            "WHERE "
	              "recipeId = :recipeId"
	                 );
	delQuery.bindValue(":recipeId", recipeId);
	Database::executeQuery(delQuery);

	QSqlQuery query(ta.db);
	query.prepare(
	        "INSERT INTO "
	          "recipeToIngredient "
	        "("
	          "recipeId, id, ingredientId, unitId, unitCount, isOptional "
	        ") VALUES ("
	          ":recipeId, :id, :ingredientId, :unitId, :unitCount, :isOptional "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "recipeId=VALUES(recipeId), "
	          "id=VALUES(id), "
	          "ingredientId=VALUES(ingredientId), "
	          "unitId=VALUES(unitId), "
	          "unitCount=VALUES(unitCount), "
	          "isOptional=VALUES(isOptional) "
	            );

	query.bindValue(":recipeId", recipeId);
	int i = 0;
	foreach(const IngredientPOD & pod, recipe.getIngredients()) {
		query.bindValue(":id",           i++);
		query.bindValue(":ingredientId", getIngredientId(pod.ingredient.getNames()));
		query.bindValue(":unitId",       getUnitId(pod.unit.getAbbreviations()));
		query.bindValue(":unitCount",    pod.count);
		query.bindValue(":isOptional",   pod.isOptional);

		Database::executeQuery(query);
	}

	ta.commit();
}

QString DB::addOrUpdateRecipe(const Recipe & recipe)
{
	if (!recipe.isValid()) {
		qWarning() << "recipe is not valid:" << recipe.toString();
		return QString();
	}

	TRANSACTION(ta);
	Recipe recipeCopy = recipe;
	if (recipeCopy.getExternId().isEmpty()) {
		qInfo() << "Adding recipe";
		addRecipe(recipeCopy);
	} else {
		qInfo() << "Updating recipe:" << recipeCopy.getExternId();
		updateRecipe(recipeCopy);
	}

	const int recipeId = getRecipeId(recipeCopy.getExternId());
	addOrUpdateRecipeTranslations(recipeId, recipeCopy);
	addOrUpdateRecipeIngredients(recipeId, recipeCopy);

	ta.commit();

	return recipeCopy.getExternId();
}

static void getIngredients(const int recipeId, Recipe & recipe)
{
	TRANSACTION(ta);

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "ingredientId, unitId, unitCount, isOptional "
	            "FROM "
	              "recipeToIngredient "
	            "WHERE "
	              "recipeId = :recipeId "
	            "ORDER BY "
	              "id"
	            );

	query.bindValue(":recipeId", recipeId);
	if (!Database::executeQuery(query)) return;

	recipe.clearIngredients();
	while (query.next()) {
		recipe.addIngredient(query.value(2).toInt(),
		                     DB::getUnits(query.value(1).toInt()).first(),
		                     DB::getIngredients(query.value(0).toInt()).first(),
		                     query.value(3).toBool());
	}

	ta.commit();
}

QList<Recipe> DB::getRecipes(const QString & searchTerm)
{
	TRANSACTION(ta);

	// TODO: improve the searchtems by adding some kind of regexp searchTerm conversion

	QSqlQuery titleSearch(ta.db);
	titleSearch.prepare("SELECT recipeId FROM titles_i18n WHERE title LIKE :searchTerm");
	titleSearch.bindValue(":searchTerm", "%" + searchTerm + "%");
	if (!Database::executeQuery(titleSearch)) return QList<Recipe>();

	QVariantList foundIds;
	while(titleSearch.next()) {
		foundIds << titleSearch.value(0).toInt();
	}

	QSqlQuery instructionStepSearch(ta.db);
	instructionStepSearch.prepare("SELECT recipeId FROM instructionStep WHERE description LIKE :searchTerm");
	instructionStepSearch.bindValue(":searchTerm", "%" + searchTerm + "%");
	if (!Database::executeQuery(instructionStepSearch)) return QList<Recipe>();

	while(instructionStepSearch.next()) {
		foundIds << instructionStepSearch.value(0).toInt();
	}

	QList<Recipe> retval = getRecipes(foundIds);
	ta.commit();
	return retval;
}

QList<Recipe> DB::getRecipes(const QVariantList & recipeIdList)
{
	TRANSACTION(ta);

	QString queryStr =
	        "SELECT "
	          "r.id, r.portionId, r.portionCount, r.createdByUserId, r.externId, "
	          "i18n.language, i18n.title "
	        "FROM "
	          "recipes r, titles_i18n i18n "
	        "WHERE "
	          "r.id = i18n.recipeId";

	if (!recipeIdList.isEmpty()) {
		queryStr += " AND r.id in (:recipeIds)";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (!recipeIdList.isEmpty()) {
		query.bindValue(":recipeIds", recipeIdList);
	}

	QList<Recipe> retval;
	if (!Database::executeQuery(query)) return retval;

	QHash<int, Recipe> dummy;
	while (query.next()) {
		const int recipeId = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(5).toString());

		Recipe recipe;
		if (dummy.contains(recipeId)) {
			recipe = dummy.value(recipeId);
		} else {
			Portion portion = DB::getPortions(query.value(1).toInt()).first();
			portion.setCount(query.value(2).toInt());
			const QList<User> users = DB::getAllUsers(query.value(3).toInt());
			recipe = Recipe();
			recipe.setPortion(portion);
			recipe.setCreatedByUser(users.first());
			recipe.setExternId(query.value(4).toString());
			getIngredients(recipeId, recipe);
			getInstructionSteps(recipeId, recipe);
		}
		recipe.updateTitle(lang, query.value(6).toString());

		dummy.insert(recipeId, recipe);
	}

	ta.commit();

	return dummy.values();
}
