#include "loginW.h"

extern WSocket *tcp_socket;

loginW::loginW(QWidget *parent)
{
	ui_init();
	connect(line_butn, &QPushButton::clicked, [this] { tcp_socket->socket->connectToHost(QHostAddress(line->text()), 1234); });
	connect(s_submit_butn, SIGNAL(clicked()), this, SLOT(user_cert()));
	connect(regi_butn, SIGNAL(clicked()), this, SLOT(user_register()));
	connect(&w, &AcRegister::close_window, [this] { w.hide(); this->s_oper_able(); this->show(); });
	connect(tcp_socket, SIGNAL(login_m(QByteArray)), this, SLOT(cert_done(QByteArray)));
}

void loginW::keyPressEvent(QKeyEvent *k)
{
	qDebug() << k->key() << '\n';
	if (k->key() == Qt::Key_Return)
		user_cert();
}

void loginW::ui_init()
{
	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 800) / 2, (height - 800) / 2);

	setFixedSize(800, 800);

	s_lab_name = new QLabel("username", this);
	s_lab_passwd = new QLabel("password", this);
	s_name_edit = new QLineEdit(this);
	s_passwd_edit = new QLineEdit(this);
	s_submit_butn = new QPushButton("login", this);
	regi_butn = new QPushButton("register", this);
	line_butn = new QPushButton("connect", this);
	line = new QLineEdit(this);

	s_lab_name->setGeometry(40, 40, 81, 16);
	s_lab_passwd->setGeometry(40, 110, 71, 20);
	s_name_edit->setGeometry(40, 70, 211, 31);
	s_passwd_edit->setGeometry(40, 140, 211, 31);
	s_submit_butn->setGeometry(40, 200, 91, 31);
	regi_butn->setGeometry(160, 200, 91, 31);
	line_butn->setGeometry(280, 200, 91, 31);
	line->setGeometry(400, 400, 400, 400);
	s_passwd_edit->setEchoMode(QLineEdit::Password);
}

void loginW::user_cert()
{
	QString user_name = s_name_edit->text();
	std::string user_passwd = s_passwd_edit->text().toStdString();

	QString encrypt_passwd = QString::fromStdString(sha256(user_passwd));
	QChar t_char = 1, l_char = user_name.length();
	QString send_data;
	send_data.append(t_char);
	send_data.append(l_char);
	send_data = send_data + user_name + encrypt_passwd;

	tcp_socket->socket->write(send_data.toUtf8().data());
}

void loginW::cert_done(QByteArray t_array)
{
	if (t_array.at(1) == '1' || t_array.at(1) == '0')
	{
		std::string c_name = QString(t_array.at(1)).toStdString() + s_name_edit->text().toStdString();
		login_pass(c_name);
	}
	else if (t_array.at(1) == 2)
		login_fail("Password error");
	else if (t_array.at(1) == 3)
		login_fail("User not found");
	else
		login_fail("This user has logged in");
}

void loginW::login_fail(QString msg)
{
	s_oper_unable();
	QMessageBox login_message(QMessageBox::NoIcon, "System message", msg, QMessageBox::Yes);
	if (login_message.exec() == QMessageBox::Yes)
		s_oper_able();
}

void loginW::login_pass(std::string c_name)
{
	s_oper_unable();
	QMessageBox login_message(QMessageBox::NoIcon, "System message", "Login success", QMessageBox::Yes);
	if (login_message.exec() == QMessageBox::Yes)
		emit ls_to(c_name);
}

void loginW::s_oper_able()
{
	s_name_edit->setEnabled(true);
	s_passwd_edit->setEnabled(true);
	s_submit_butn->setEnabled(true);
	regi_butn->setEnabled(true);
}

void loginW::s_oper_unable()
{
	s_name_edit->setEnabled(false);
	s_passwd_edit->setEnabled(false);
	s_submit_butn->setEnabled(false);
	regi_butn->setEnabled(false);
}

void loginW::user_register()
{
	this->hide();
	w.show();
}
