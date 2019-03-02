INSERT INTO users(`id`, `login`, `passwordHash`, `passwordCrypto`, `firstName`, `lastName`) VALUES(1, 'admin', 'hashHere', 'pwCryptoHere', 'Admin', 'User');

INSERT INTO units(`inGram`, `inMl`) VALUES(1, 1);
INSERT INTO units_i18n(`unitId`, `language`, `abbreviation`, `completeName`) VALUES(1, 'de_DE', 'ml', 'mili Liter');

INSERT INTO categories(`isFoodCategory`) VALUES(TRUE);
INSERT INTO categories_i18n(`categoryId`, `language`, `name`) VALUES(1, 'de_DE', 'Nachtisch');
INSERT INTO categories(`isFoodCategory`) VALUES(FALSE);
INSERT INTO categories_i18n(`categoryId`, `language`, `name`) VALUES(2, 'de_DE', 'Süss');

INSERT INTO ingridients(`isLiquid`, `foodCategory`) VALUES(FALSE, 2);
INSERT INTO ingridients_i18n(`ingridientId`, `language`, `name`) VALUES(1, 'de_DE', 'Zucker');

INSERT INTO portions() VALUES();
INSERT INTO portions() VALUES();
INSERT INTO portions() VALUES();
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(1, 'de_DE', 'Stück');
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(2, 'de_DE', 'Teller');
INSERT INTO portions_i18n(`portionId`, `language`, `description`) VALUES(3, 'de_DE', 'Portion');

INSERT INTO userPermission(`userId`, `permissionId`) VALUES(1, 1);
