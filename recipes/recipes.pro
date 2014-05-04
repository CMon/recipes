!include(config.pri):error("Could not find config.pri")

CONFIG(clients) {
    message("Building clients")
    addSubdir(common)
    addSubdir(console-client, common)
}

CONFIG(server) {
    message("Building server")
    addSubdir(common)
    addSubdir(services, database, common)
    addSubdir(database)
    addSubdir(server, services database common)
}
