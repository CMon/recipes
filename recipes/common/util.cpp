#include "util.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include <QCoreApplication>
#include <QTextStream>

bool Util::enableConsoleEcho(const bool enable)
{
	bool originalValue = true;
#ifdef Q_OS_WIN
	DWORD  mode;
	HANDLE hConIn = GetStdHandle( STD_INPUT_HANDLE );
	GetConsoleMode( hConIn, &mode );
	originalValue = (mode & (ENABLE_ECHO_INPUT));
	mode = enable
	       ? (mode |   ENABLE_ECHO_INPUT )
	       : (mode & ~(ENABLE_ECHO_INPUT));
	SetConsoleMode( hConIn, mode );
#else
	struct termios settings;
	tcgetattr( STDIN_FILENO, &settings );
	originalValue = (settings.c_lflag & (ECHO));
	settings.c_lflag = enable
	                   ? (settings.c_lflag |   ECHO )
	                   : (settings.c_lflag & ~(ECHO));
	tcsetattr( STDIN_FILENO, TCSANOW, &settings );
#endif
	return originalValue;
}

bool Util::enableConsoleBufferedIo(const bool enable)
{
	bool originalValue = true;
#ifdef Q_OS_WIN
#   error IMPLEMENT ME
#else
	struct termios settings;
	tcgetattr(STDIN_FILENO, &settings);
	originalValue = (settings.c_lflag & (ICANON));
	settings.c_lflag = enable
	                   ? (settings.c_lflag |   ICANON )
	                   : (settings.c_lflag & ~(ICANON));
	tcsetattr(STDIN_FILENO, TCSANOW, &settings);
#endif
	return originalValue;
}

QString Util::enterPassword(const QString &prompt)
{
	QTextStream out(stdout);
	QTextStream in(stdin);

	const bool originalConsoleEcho = enableConsoleEcho(false);

	out << endl << prompt;
	out.flush();

	const QString password = in.readLine();
	out << endl;

	enableConsoleEcho(originalConsoleEcho);
	return password;}
