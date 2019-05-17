#include "register.h"

extern WSocket *tcp_socket;

AcRegister::AcRegister()
{
	ui_init();
	connect(r_submit_butn, SIGNAL(clicked()), this, SLOT(on_click()));
	connect(tcp_socket, SIGNAL(regi_m(QByteArray)), this, SLOT(regi_done(QByteArray)));
}

AcRegister::~AcRegister()
{

}

void AcRegister::regi_done(QByteArray t_array)
{
	if (t_array.at(1) == 1)
		regi_fail();
	else
		regi_pass();
}

void AcRegister::r_oper_able()
{
	r_name_edit->setEnabled(true);
	r_passwd_edit->setEnabled(true);
	r_submit_butn->setEnabled(true);
}

void AcRegister::r_oper_unable()
{
	r_name_edit->setEnabled(false);
	r_passwd_edit->setEnabled(false);
	r_submit_butn->setEnabled(false);
}

void AcRegister::closeEvent(QCloseEvent *event)
{
	emit close_window();
}

void AcRegister::regi_fail()
{
	r_oper_unable();
	QMessageBox regi_message(QMessageBox::NoIcon, "System message", "Register fail", QMessageBox::Yes);
	if (regi_message.exec() == QMessageBox::Yes)
		r_oper_able();
}

void AcRegister::regi_pass()
{
	r_oper_unable();
	QMessageBox regi_message(QMessageBox::NoIcon, "System message", "Register success", QMessageBox::Yes);
	if (regi_message.exec() == QMessageBox::Yes)
	{
		r_oper_able();
		emit close_window();
	}
}

void AcRegister::ui_init()
{
	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 373) / 2, (height - 303) / 2);

	setFixedSize(373, 303);

	r_lab_name = new QLabel("号码", this);
	r_lab_passwd = new QLabel("密码", this);
	r_name_edit = new QLineEdit(this);
	r_passwd_edit = new QLineEdit(this);
	r_submit_butn = new QPushButton("提交", this);
	user_c = new QRadioButton("闯关者", this);
	user_p = new QRadioButton("出题者", this);
	user_type = new QButtonGroup(this);

	r_lab_name->setGeometry(80, 50, 81, 16);
	r_lab_passwd->setGeometry(80, 130, 71, 20);
	r_name_edit->setGeometry(80, 80, 211, 31);
	r_passwd_edit->setGeometry(80, 160, 211, 31);
	r_submit_butn->setGeometry(140, 250, 91, 31);
	user_c->setGeometry(80, 210, 111, 22);
	user_p->setGeometry(210, 210, 101, 22);

	user_type->addButton(user_c);
	user_type->addButton(user_p);
	user_type->setExclusive(true);

	r_passwd_edit->setEchoMode(QLineEdit::Password);
}

bool AcRegister::ac_cert()
{
	std::string user_name = r_name_edit->text().toStdString();
	user t_user(user_name);
	if (t_user.ac_cert())
		return true;
	else
		return false;
}

void AcRegister::on_click()
{
	if (!(user_p->isChecked()) && !(user_c->isChecked()))
		regi_fail();
	else
	{
		QString write_data;
		write_data.append(QChar(6));
		if (user_p->isChecked())
			write_data.append(QChar(1));
		else
			write_data.append(QChar(2));
		int name_len = r_name_edit->text().size();
		write_data.append(QChar(name_len));
		write_data.append(r_name_edit->text());
		write_data.append(QString::fromStdString(sha256(r_passwd_edit->text().toStdString())));

		tcp_socket->socket->write(write_data.toUtf8().data());
	}
}