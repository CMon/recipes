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

static const char * colorReset  = "\033[0m";
static const char * colorGreen  = "\033[0;32m";
static const char * colorYellow = "\033[0;33m";
static const char * colorRed    = "\033[0;31m";
static const char * colorBlink  = "\033[0;5m";

static void logMessage(FILE * stream, QtMsgType type, const QMessageLogContext &context, const QString &msg, bool withColor)
{
	const char * colorPrefix = colorReset;

	const QDateTime now = QDateTime::currentDateTimeUtc();
	char typeStr;
	switch (type) {
		case QtDebugMsg:    typeStr = 'D'; break;
		case QtInfoMsg:     typeStr = 'I'; colorPrefix = colorGreen;  break;
		case QtWarningMsg:  typeStr = 'W'; colorPrefix = colorYellow; break;
		case QtCriticalMsg: typeStr = 'C'; colorPrefix = colorRed;    break;
		case QtFatalMsg:    typeStr = 'F'; colorPrefix = colorBlink;  break;
	}
	if (withColor) {
		fprintf(stream, "%s%s %c %20s:%4d %15s:  %s%s\n", colorPrefix, qPrintable(now.toString(Qt::ISODate)), typeStr, basename(const_cast<char*>(context.file)), context.line, context.category, msg.toLocal8Bit().constData(), colorReset);
	} else {
		fprintf(stream, "%s %c %20s:%4d %15s:  %s\n", qPrintable(now.toString(Qt::ISODate)), typeStr, basename(const_cast<char*>(context.file)), context.line, context.category, msg.toLocal8Bit().constData());
	}
	fflush(stream);
}

void RecipeLog::consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	logMessage(stdout, type, context, msg, true);
}

void RecipeLog::fileMessageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	if (!logfile) {
		fprintf(stdout, "File logging requested, but file is not initialized\n");
		return;
	}

	logMessage(logfile, type, context, msg, false);
}
