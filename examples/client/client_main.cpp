#include <QtCore>

#include "client.h"

int main( int argc, char *argv[] ) {
	QCoreApplication app( argc, argv );
	Client x;
	return app.exec();	
}
