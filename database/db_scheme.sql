--------------------------
-- scheme version 1     --
--------------------------

CREATE TABLE `users` (
    `id`             BIGINT(11) NOT NULL AUTO_INCREMENT,
    `login`          VARCHAR(255) NOT NULL,
    `passwordHash`   VARCHAR(255) NOT NULL,
    `passwordCrypto` VARCHAR(255) NOT NULL,
    `firstName`      VARCHAR(255) DEFAULT NULL,
    `lastName`       VARCHAR(255) DEFAULT NULL,
    `isDeleted`      TINYINT(3) DEFAULT 1,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `permission` (
    `id`       INT(10) NOT NULL AUTO_INCREMENT,
    `techName` VARCHAR(255) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `permission_i18n` (
    `permissionId` INT(11) NOT NULL,
    `language`     VARCHAR(255) NOT NULL,
    `title`        VARCHAR(255) DEFAULT NULL,
    `description`  TEXT DEFAULT NULL,
    PRIMARY KEY (`permissionId`, `language`),
    CONSTRAINT `permission_i18n_key1` FOREIGN KEY (`permissionId`) REFERENCES `permission` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `userPermission` (
    `userId`       BIGINT(11) NOT NULL,
    `permissionId` INT(10) NOT NULL,
    PRIMARY KEY (`userId`, `permissionId`),
    CONSTRAINT `userPermission_key1` FOREIGN KEY (`userId`) REFERENCES `users` (`id`),
    CONSTRAINT `userPermission_key2` FOREIGN KEY (`permissionId`) REFERENCES `permission` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `units` (
    `id`      INT(3) NOT NULL AUTO_INCREMENT,
    `inGram`  INT(5) DEFAULT NULL,
    `inMl`    INT(5) DEFAULT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `units_i18n` (
    `unitId`       INT(3) NOT NULL,
    `language`     VARCHAR(255) NOT NULL,
    `abbreviation` VARCHAR(255) DEFAULT NULL,
    `completeName` VARCHAR(255) DEFAULT NULL,
    PRIMARY KEY (`unitId`, `language`),
    CONSTRAINT `unit_key1` FOREIGN KEY (`unitId`) REFERENCES `units` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `categories` (
    `id`             INT(5) NOT NULL AUTO_INCREMENT,
    `isFoodCategory` TINYINT(3) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `categories_i18n` (
    `categoryId` INT(5) NOT NULL,
    `language`   VARCHAR(255) NOT NULL,
    `name`       VARCHAR(255) NOT NULL,
    PRIMARY KEY (`categoryId`, `language`),
    CONSTRAINT `categories_i18n_key1` FOREIGN KEY (`categoryId`) REFERENCES `categories` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `ingredients` (
    `id`              INT(5) NOT NULL AUTO_INCREMENT,
    `isLiquid`        TINYINT(3) NOT NULL,
    `foodCategoryId`  INT(5) NOT NULL,
    `containsGluten`  TINYINT(3) DEFAULT FALSE,
    `containsLactose` TINYINT(3) DEFAULT FALSE,
    PRIMARY KEY (`id`),
    CONSTRAINT `ingredients_key1` FOREIGN KEY (`foodCategoryId`) REFERENCES `categories` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `ingredients_i18n` (
    `ingredientId` INT(5) NOT NULL,
    `language`     VARCHAR(255) NOT NULL,
    `name`         VARCHAR(255) NOT NULL,
    PRIMARY KEY (`ingredientId`, `language`),
    CONSTRAINT `ingredients_i18n_key1` FOREIGN KEY (`ingredientId`) REFERENCES `ingredients` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `portions` (
    `id` INT(5) NOT NULL AUTO_INCREMENT,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `portions_i18n` (
    `portionId`  INT(5) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `description` VARCHAR(255) NOT NULL,
    PRIMARY KEY (`portionId`, `language`),
    CONSTRAINT `portions_i18n_key1` FOREIGN KEY (`portionId`) REFERENCES `portions` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `recipes` (
    `id`             BIGINT(11) NOT NULL AUTO_INCREMENT,
    `externId`       VARCHAR(255) NOT NULL,
    `portionId`      INT(5) NOT NULL,
    `portionCount`   INT(3) NOT NULL,
    `createdByUserId` BIGINT(11) NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `recipes_key1` (`externId`),
    CONSTRAINT `recipes_key2` FOREIGN KEY (`portionId`) REFERENCES `portions` (`id`),
    CONSTRAINT `recipes_key3` FOREIGN KEY (`createdByUserId`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `titles_i18n` (
    `recipeId`    BIGINT(11) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `title`       VARCHAR(255) DEFAULT NULL,
    PRIMARY KEY (`recipeId`, `language`),
    CONSTRAINT `recipeTitle_i18n_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `recipePictures` (
    `recipeId`    BIGINT(11) NOT NULL,
    `pathToImg`   VARCHAR(255) NOT NULL,
    PRIMARY KEY (`recipeId`, `pathToImg`),
    CONSTRAINT `recipePictures_i18n_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `recipeToIngredient` (
    `recipeId`        BIGINT(11) NOT NULL,
    `id`              BIGINT(11) NOT NULL,
    `ingredientId`    INT(5) NOT NULL,
    `unitId`          INT(3) NOT NULL,
    `unitCount`       INT(3) NOT NULL,
    `isOptional`      TINYINT(3) DEFAULT 1,
    PRIMARY KEY (`recipeId`, `id`),
    CONSTRAINT `recipeToIngredients_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`),
    CONSTRAINT `recipeToIngredients_key2` FOREIGN KEY (`ingredientId`) REFERENCES `ingredients` (`id`),
    CONSTRAINT `recipeToIngredients_key3` FOREIGN KEY (`unitId`) REFERENCES `units` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `receiptToCategory` (
    `recipeId`   BIGINT(11) NOT NULL,
    `categoryId` INT(5) NOT NULL,
    PRIMARY KEY (`recipeId`, `categoryId`),
    CONSTRAINT `receiptToCategory_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`),
    CONSTRAINT `receiptToCategory_key2` FOREIGN KEY (`categoryId`) REFERENCES `categories` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `instructionStep` (
    `recipeId`    BIGINT(11) NOT NULL,
    `position`    INT(10) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `description` TEXT DEFAULT NULL,
    `pathToImg`   VARCHAR(255) NOT NULL,
    PRIMARY KEY (`recipeId`, `position`, `language`),
    CONSTRAINT `instructionStep_key2` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--------------------------
-- scheme version 2     --
--------------------------

INSERT INTO permission(`id`, `techName`) VALUES(1, 'administrator');
INSERT INTO permission(`id`, `techName`) VALUES(2, 'create_recipe');
INSERT INTO permission(`id`, `techName`) VALUES(3, 'read_recipes');
INSERT INTO permission(`id`, `techName`) VALUES(4, 'add_ingredients');
INSERT INTO permission(`id`, `techName`) VALUES(5, 'add_units');
INSERT INTO permission(`id`, `techName`) VALUES(6, 'add_categories');

INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(1, 'de_DE', 'Administrator', 'Benutzt zur Erstellung neuer Nutzer und anpassung der Rechte');
INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(2, 'de_DE', 'Rezepte erstellen', 'Hat die Berechtigung Rezepte zu erstellen oder zu bearbeiten');
INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(3, 'de_DE', 'Rezepte lesen', 'Hat die Berechtigung Rezepte zu lesen');
INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(4, 'de_DE', 'Zutaten hinzufügen', 'Hat das Recht Zutaten hinzuzufügen oder zu bearbeiten');
INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(5, 'de_DE', 'Einheiten hinzufügen', 'Hat das Recht neue Einheiten und deren Umrechnungen hinzuzufügen oder zu bearbeiten');
INSERT INTO permission_i18n(`permissionId`, `language`, `title`, `description`) VALUES(6, 'de_DE', 'Kategorien hinzufügen', 'Hat das Recht neue arten von Kategorien hinzuzufügen oder zu bearbeiten');
