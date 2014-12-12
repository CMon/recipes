include(recipes/config.pri)

cache()

addSubdir(cflib)
addSubdir(recipes, cflib)

addSubdir(test, recipes cflib)

OTHER_FILES = \
    doc/structure.md \
    database/db_scheme.sql \
    database/initial_data.sql
