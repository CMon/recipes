#include "log.h"

#include <QDateTime>

#include <stdio.h>
#include <libgen.h>

static FILE * logfile = nullptr;

bool RecipeLog::initFile(const QString & filename)
{
	logfile = fopen(qPrintable(filename), "a");
	if (!logfile) {
		fprintf(stdout, "Could not open logfile: %s\n", strerror(errno));
		return false;
	}
	return true;
}

static void logMessage(FILE * stream, QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	const QDateTime now = QDateTime::currentDateTimeUtc();
	char typeStr;
	switch (type) {
		case QtDebugMsg:    typeStr = 'D'; break;
		case QtInfoMsg:     typeStr = 'I'; break;
		case QtWarningMsg:  typeStr = 'W'; break;
		case QtCriticalMsg: typeStr = 'C'; break;
		case QtFatalMsg:    typeStr = 'F'; break;
	}
	fprintf(stream, "%s %c %20s:%4d %15s:  %s\n", qPrintable(now.toString(Qt::ISODate)), typeStr, basename(const_cast<char*>(context.file)), context.line, context.category, msg.toLocal8Bit().constData());
	fflush(stream);

}

void RecipeLog::consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	logMessage(stdout, type, context, msg);
}

void RecipeLog::fileMessageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	if (!logfile) {
		fprintf(stdout, "File logging requested, but file is not initialized\n");
		return;
	}

	logMessage(logfile, type, context, msg);
}
