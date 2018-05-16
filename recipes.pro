include(config.pri)
cache()

addSubdir(recipes)
addSubdir(test, recipes)

OTHER_FILES = \
    doc/structure.md \
    database/db_scheme.sql \
    database/initial_data.sql
