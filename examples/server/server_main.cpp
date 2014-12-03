// CORE includes
#include <QCoreApplication>

#include "server.h"

int main( int argc, char *argv[] )
{
    QCoreApplication app(argc, argv);

    Server x;
    if( !x.listen(QHostAddress::Any, 8082) ) {
        return 1;
    }

    return app.exec();

} // int main( int argc, char *argv[] )
