#pragma once

#include <QtMsgHandler>

class RecipeLog
{
public:
	static void consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
