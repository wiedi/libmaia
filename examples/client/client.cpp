#include "client.h"

Client::Client(QObject* parent) : QObject(parent) {
//	rpc = new MaiaXmlRpcClient(QUrl("http://phpxmlrpc.sourceforge.net/server.php"), this);
	rpc = new MaiaXmlRpcClient(QUrl("http://localhost:8080/RPC2"), this);
	
	QTimer::singleShot(2000, this, SLOT(doClient()));
}

void Client::doClient() {
	QVariantList args;
	rpc->call("examples.nix", args,
				this, SLOT(testResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
	args << QVariant(7);
	rpc->call("examples.getStateName", args,
				this, SLOT(testResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
	rpc->call("examples.birne", args,
				this, SLOT(testResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
	args[0] = QVariant(-128);
	rpc->call("examples.birne", args,
				this, SLOT(testResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
	rpc->call("examples.notfound", args,
				this, SLOT(testResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
	QDateTime towelDay;
	towelDay.setDate(QDate(2008, 5, 25));
	args.clear();
	args << towelDay;
	rpc->call("examples.plusOneYear", args,
				this, SLOT(towelResponse(QVariant &)),
				this, SLOT(testFault(int, const QString &)));
}


void Client::testResponse(QVariant &arg) {
		qDebug() << arg.toString();
}

void Client::testFault(int error, const QString &message) {
		qDebug() << "EEE:" << error << "-" << message;
}

void Client::towelResponse(QVariant &arg) {
	qDebug() << "Next years Towel Day is on" << arg.toDateTime();
}
