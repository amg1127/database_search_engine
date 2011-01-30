#include <QtGui/QApplication>
#include "frmMainWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("DETRAN-RS");
    QCoreApplication::setApplicationName("database-search-engine");
    QApplication a(argc, argv);
    frmMainWindow w;
    w.show();
    return a.exec();
}
