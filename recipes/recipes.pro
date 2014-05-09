!include(config.pri):error("Could not find config.pri")

addSubdir(common)

CONFIG(clients) {
    message("Building clients")
    addSubdir(console-client, common)
}

CONFIG(server) {
    message("Building server")
    addSubdir(services, database, common)
    addSubdir(database)
    addSubdir(server, services database common)
}
