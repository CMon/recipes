include(cflib/include.pri)

cache()

addSubdir(cflib)
addSubdir(recipes, cflib)

OTHER_FILES = \
    doc/structure.md \
