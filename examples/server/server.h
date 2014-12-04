#ifndef SERVER_H
#define SERVER_H

// CORE includes
#include <QDateTime>
#include <QObject>
#include <QVariant>

// NETWORK includes
#include <QHostAddress>

// fwd
class MaiaXmlRpcServer;

class Server : public QObject
{
    Q_OBJECT

public:
    Server( QObject *parent = 0 );

    bool listen( const QHostAddress &address, quint16 port );

private slots:
    QString callState( int i );
    QVariant birne( int x );
    void nix();
    QDateTime plusOneYear( QDateTime sometime );

private:
    MaiaXmlRpcServer *server;

};

#endif
