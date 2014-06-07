CREATE TABLE `users` (
    `id`           BIGINT(11) NOT NULL AUTO_INCREMENT,
    `login`        VARCHAR(255) NOT NULL,
    `passwordHash` VARCHAR(255) NOT NULL,
    `permissions`  INT(10) unsigned NOT NULL,
    `firstName`    VARCHAR(255) DEFAULT NULL,
    `lastName`     VARCHAR(255) DEFAULT NULL,
    `isDeleted`    TINYINT(3) DEFAULT 1,
    PRIMARY KEY (`id`)
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
    `id`              BIGINT(11) NOT NULL AUTO_INCREMENT,
    `portionId`      INT(5) NOT NULL,
    `portionCount`   INT(3) NOT NULL,
    `createdByUserId` BIGINT(11) NOT NULL,
    PRIMARY KEY (`id`),
    CONSTRAINT `recipes_key1` FOREIGN KEY (`portionId`) REFERENCES `portions` (`id`),
    CONSTRAINT `recipes_key2` FOREIGN KEY (`createdByUserId`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `recipeTitle_i18n` (
    `recipeId`    BIGINT(11) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `description` VARCHAR(255) NOT NULL,
    PRIMARY KEY (`recipeId`, `language`),
    CONSTRAINT `recipeTitle_i18n_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `recipeDescription_i18n` (
    `recipeId`    BIGINT(11) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `description` TEXT NOT NULL,
    PRIMARY KEY (`recipeId`, `language`),
    CONSTRAINT `recipeDescription_i18n_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `recipePictures` (
    `recipeId`    BIGINT(11) NOT NULL,
    `language`    VARCHAR(255) NOT NULL,
    `pathToImg`   VARCHAR(255) NOT NULL,
    PRIMARY KEY (`recipeId`, `language`),
    CONSTRAINT `recipePictures_i18n_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `receiptToIngredient` (
    `recipeId`        BIGINT(11) NOT NULL,
    `id`              BIGINT(11) NOT NULL,
    `ingredientId`    INT(5) NOT NULL,
    `unitId`          INT(3) NOT NULL,
    `unitCount`       INT(3) NOT NULL,
    `isOptional`      TINYINT(3) DEFAULT 1,
    PRIMARY KEY (`recipeId`, `id`),
    CONSTRAINT `receiptToIngredients_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`),
    CONSTRAINT `receiptToIngredients_key2` FOREIGN KEY (`ingredientId`) REFERENCES `ingredients` (`id`),
    CONSTRAINT `receiptToIngredients_key3` FOREIGN KEY (`unitId`) REFERENCES `units` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `receiptToCategory` (
    `recipeId`   BIGINT(11) NOT NULL,
    `categoryId` INT(5) NOT NULL,
    PRIMARY KEY (`recipeId`, `categoryId`),
    CONSTRAINT `receiptToCategory_key1` FOREIGN KEY (`recipeId`) REFERENCES `recipes` (`id`),
    CONSTRAINT `receiptToCategory_key2` FOREIGN KEY (`categoryId`) REFERENCES `categories` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
