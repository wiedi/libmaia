#ifndef SERVER_H
#define SERVER_H

#include "maiaXmlRpcServer.h"

class Server : public QObject {
	Q_OBJECT
	
	public:
		Server(QObject* parent = 0);
		
	public slots:
	
	private slots:
		QString callState(int i);
		QVariant birne(int x);
		void nix();
		QDateTime plusOneYear(QDateTime sometime);
	
	private:
		MaiaXmlRpcServer *server;
};

#endif
