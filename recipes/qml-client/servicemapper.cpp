#include "servicemapper.h"

#include <recipes/qml-client/logcategory.h>

Q_LOGGING_CATEGORY(GUI_CLIENT, "client.services")

bool UserService::login(const QString & username, const QString & password) const
{
	if (username.isEmpty() || password.isEmpty()) return false;

	// use the upcoming interface for logging in
	qCDebug(GUI_CLIENT) << "Login" << username << password;

	return true;
}

void UserService::retLogin(bool isLoggedIn)
{
	isLoggedIn_ = isLoggedIn;
	emit loggedInChanged();
}

