# libmaia

libmaia is a easy-to-use XML-RPC library for Qt!


# compiling libmaia
	Modify src/build.pri if you want to build libmaia as a static lib.

	qmake
	make



# Qt Datatypes

	Allowed types for Argument and Return Values:

	C++/Qt-Types	XMLRPC-Types
	----------------------------------------
	* int           <int></int>
	* bool          <bool></bool>
	* double        <double></double>
	* QString       <string></string>
	* QDateTime     <datetime.iso8601></datetime.iso8601>
	* ByteArray     <base64></base64>
	* QVariantMap   <struct></struct>
	* QVariantList  <array></array>



# using libmaia

1. 	qmake: your Project file (.pro) should contain

		INCLUDEPATH += /path/to/libmaia/headers
		LIBS += -L/path/to/libmaia -lmaia
		QT   += xml network
		
	If you build libmaia as a shared lib add following line to project file:
			
		DEFINES += MAIA_USING_SHARED
		
2. in your header file include

		#include "maiaXmlRpcClient.h"
		
 	and / or
 
		#include "maiaXmlRpcServer.h"


3. create object
	
	server:
	
		MaiaXmlRpcServer *server = new MaiaXmlRpcServer(8080, this);

	client:
	
		MaiaXmlRpcClient *client = new MaiaXmlRpcClient(QUrl("http://localhost:8080/RPC2"), this);


4. register a method

	your method has to be a Qt Slot.
	

		// example method:
		QString MyClass::myMethod(int param1, QString param2) {
			if(param1 > 5)
				return param2;
			else
				return "not bigger than 5";
		}

		// register it:
		// "example.methodName" <- used to identify the method over xml-rpc
		// this <- pointer to the class which contains the method you would export
		// "myMethod" the name of the method
		server->addMethod("example.methodName", this, "myMethod");


5. call a method

	when calling a method you need three things:
	
	1. a Slot for the MethodResponse
	2. a Slot for the FaultResponse
	3. a QVariantList containig the arguments for the RPC-Method

	example code:
	
		void MyClientClass::myResponseMethod(QVariant &arg) {
			// do something with the arg
		}
	
		void MyClientClass::myFaultResponse(int error, const QString &message) {
			qDebug() << "An Error occoured, Code: " << error << " Message: " << message;
		}

		QVariantList args;
		args << 5;
		args << "second argument";

		rpcClient->call("example.methodName", args,
			this, SLOT(myResponseMethod(QVariant&)),
			this, SLOT(myFaultResponse(int, const QString &)));




