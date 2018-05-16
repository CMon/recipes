!include(../config.pri):error("Could not find config.pri")

addSubdir(common)

CONFIG(clients) {
    message("Building clients")
    addSubdir(console-client, common)
    addSubdir(qml-client, common)
}

CONFIG(server) {
    message("Building server")
    addSubdir(servercommon)
    addSubdir(services, database common servercommon)
    addSubdir(database)
    addSubdir(server, services database common servercommon)
}
