#include "server.h"

Server::Server(QObject* parent) : QObject(parent) {
	
	server = new MaiaXmlRpcServer(8080, this);
	server->addMethod("examples.getStateName", this, "callState");
	server->addMethod("examples.birne", this, "birne");
	server->addMethod("examples.nix", this, "nix");
	server->addMethod("examples.plusOneYear", this, "plusOneYear");
}


QString Server::callState(int i) {
	if(i < 42) 
		return "Banane";
	else
		return "Orange";
}

QVariant Server::birne(int x) {
	if(x < 0)
		return QVariant::fromValue(MaiaFault(7, "Birne is doof"));
	else
		return "Tolle Birne";
}

void Server::nix() {
	qDebug() << "i got called";
}

QDateTime Server::plusOneYear(QDateTime sometime) {
	return sometime.addYears(1);
}
