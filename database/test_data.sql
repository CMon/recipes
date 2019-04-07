INSERT INTO users(`id`, `login`, `passwordHash`, `passwordCrypto`, `firstName`, `lastName`) VALUES(1, 'admin', '$2a$12$gica7Dxxc.t.6h8FqcYOheB4KkmyfjHqS6QNPnmpMV8doF2iIWdcK', 'bcrypt', 'Admin', 'User');
INSERT INTO users(`id`, `login`, `passwordHash`, `passwordCrypto`, `firstName`, `lastName`) VALUES(2, 'test.user', '$2a$12$w98Lpg2FC8fuWYguifb20uPz5c8nfnryUVdhLcPP7LOsK./5i.286', 'bcrypt', 'Test', 'User');

INSERT INTO units(`inGram`, `inMl`) VALUES(1, 1);
INSERT INTO units_i18n(`unitId`, `language`, `abbreviation`, `completeName`) VALUES(1, 'de_DE', 'ml', 'mili Liter');

INSERT INTO categories(`isFoodCategory`) VALUES(TRUE);
INSERT INTO categories_i18n(`categoryId`, `language`, `name`) VALUES(1, 'de_DE', 'Nachtisch');
INSERT INTO categories(`isFoodCategory`) VALUES(FALSE);
INSERT INTO categories_i18n(`categoryId`, `language`, `name`) VALUES(2, 'de_DE', 'Süss');

INSERT INTO ingredients(`isLiquid`, `foodCategoryId`) VALUES(FALSE, 2);
INSERT INTO ingredients_i18n(`ingredientId`, `language`, `name`) VALUES(1, 'de_DE', 'Zucker');

INSERT INTO portions() VALUES();
INSERT INTO portions() VALUES();
INSERT INTO portions() VALUES();
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(1, 'de_DE', 'Stück');
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(2, 'de_DE', 'Teller');
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(3, 'de_DE', 'Portion');

--- Admin permissions
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 1);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 2);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 3);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 4);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 5);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 6);

--- testUser permissions
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 1);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 2);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 3);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 4);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 5);
INSERT INTO userPermission(`userId`, `permissionId`) VALUES(2, 6);
