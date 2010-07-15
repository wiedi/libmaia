#include <QtCore>

#include "maiaObject.h"
#include "server.h"

int main( int argc, char *argv[] ) {
	QCoreApplication app( argc, argv );
	Server x;
	return app.exec();	
}
