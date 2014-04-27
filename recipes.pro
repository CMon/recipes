include(config.pri)

OTHER_FILES = doc/structure.md

TEMPLATE = subdirs

CONFIG(common) {
    message("Building common")
    addSubdirs(src/common)
}

CONFIG(database) {
    addSubdirs(src/database)
}

CONFIG(clients) {
    message("Building clients")
    addSubdirs(src/console-client src/common)
}

CONFIG(server) {
    message("Building server")
    addSubdirs(src/server src/database src/common)
}
