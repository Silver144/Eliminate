#include "WEli.h"
#include <QtWidgets/QApplication>
#include <QSharedMemory>

extern WSocket *tcp_socket;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tcp_socket = new WSocket();
	WEli w;
	w.login_window->show();
	return a.exec();
}
