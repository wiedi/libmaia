// maia includes
#include "maiaFault.h"
#include "maiaXmlRpcServer.h"

#include "server.h"

Server::Server( QObject *parent )
    : QObject(parent)
{
    server = new MaiaXmlRpcServer(8082, this);
    server->addMethod("examples.getStateName", this, "callState");
    server->addMethod("examples.birne", this, "birne");
    server->addMethod("examples.nix", this, "nix");
    server->addMethod("examples.plusOneYear", this, "plusOneYear");

} // ctor

QString Server::callState( int i )
{
    if( i < 42 ) {
        return "Banane";
    }
    return "Orange";

} // QString callState( int i )

QVariant Server::birne( int x )
{
    if( x < 0 ) {
        return QVariant::fromValue(MaiaFault(7, "Birne is doof"));
    }
    return "Tolle Birne";

} // QVariant birne( int x )

void Server::nix()
{
    qDebug() << "i got called";

} // void nix()

QDateTime Server::plusOneYear( QDateTime sometime )
{
    return sometime.addYears(1);

} // QDateTime plusOneYear( QDateTime sometime )
