#pragma once

#include <cflib/http/jsservice.h>

#include <common/user.h>

class UserService : public QObject, public cflib::http::JSService
{
    Q_OBJECT
    SERIALIZE_CLASS
public:
    UserService();
    ~UserService();

rmi:
    bool login(const QString & login, const QString & password, User & user);
    bool logout();
    bool addUser(const User & login, QString password);
    QList<User> getUsers();

public:
    virtual void clientExpired(uint clId);

public slots:
    void getCurrentUser(uint clId, User & user);

protected:
    virtual void preCallInit(const cflib::http::Request & request, uint clId);

private:
    QHash<uint, User> currentUsers_;
    User user_;
};
