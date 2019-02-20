#include "log.h"

#include <QDateTime>

#include <stdio.h>
#include <libgen.h>

void RecipeLog::consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	const QDateTime now = QDateTime::currentDateTime();
	char typeStr;
	switch (type) {
		case QtDebugMsg:    typeStr = 'D'; break;
		case QtInfoMsg:     typeStr = 'I'; break;
		case QtWarningMsg:  typeStr = 'W'; break;
		case QtCriticalMsg: typeStr = 'C'; break;
		case QtFatalMsg:    typeStr = 'F'; break;
	}
	fprintf(stdout, "%s %c %20s:%4d %15s:  %s\n", qPrintable(now.toString(Qt::ISODate)), typeStr, basename(const_cast<char*>(context.file)), context.line, context.category, msg.toLocal8Bit().constData());
	fflush(stdout);
}
