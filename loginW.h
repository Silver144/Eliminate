#pragma once

#include "register.h"
#include "user.h"
#include "socket.h"

class loginW : public QWidget
{
	Q_OBJECT

public:
	explicit loginW(QWidget *parent = Q_NULLPTR);

private:
	QLabel *s_lab_name;
	QLabel *s_lab_passwd;
	QLineEdit *s_name_edit;
	QLineEdit *s_passwd_edit;
	QPushButton *s_submit_butn;
	QPushButton *regi_butn;
	QPushButton *line_butn;
	QLineEdit *line;
	void ui_init();
	void login_fail(QString msg);
	void login_pass(std::string c_name);
	void keyPressEvent(QKeyEvent *k);
	AcRegister w;
	

public slots:
	void user_cert();
	void user_register();
	void s_oper_unable();
	void s_oper_able();
	void cert_done(QByteArray t_array);

signals:
	void ls_to(std::string c_name);
};