#include <QCoreApplication>

int main(int argc, char ** argv)
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Receipts Server");

    return a.exec();
}
