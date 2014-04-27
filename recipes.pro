include(config.pri)

TEMPLATE = subdirs

CONFIG(common) {
    message("Building common")
    addSubdirs(src/common)
}

CONFIG(clients) {
    message("Building clients")
    addSubdirs(src/console-client src/common)
}

CONFIG(server) {
    message("Building server")
    addSubdirs(src/server)
}
