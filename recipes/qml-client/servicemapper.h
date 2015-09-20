#pragma once

#include <QObject>

class UserService : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool isLoggedIn READ loggedIn NOTIFY loggedInChanged)
public:
	UserService() : isLoggedIn_(false) {}

	Q_INVOKABLE bool login(const QString & username, const QString & password) const;
	bool loggedIn() { return isLoggedIn_; }

signals:
	void loggedInChanged();

private:
	void retLogin(bool isLoggedIn);

private:
	bool isLoggedIn_;
};
