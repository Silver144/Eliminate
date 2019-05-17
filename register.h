#pragma once

#include "user.h"
#include "socket.h"

class AcRegister : public QWidget
{
	Q_OBJECT

public:
	explicit AcRegister();
	~AcRegister();

private:
	QLabel *r_lab_name;
	QLabel *r_lab_passwd;
	QLineEdit *r_name_edit;
	QLineEdit *r_passwd_edit;
	QPushButton *r_submit_butn;
	QRadioButton *user_c;
	QRadioButton *user_p;
	QButtonGroup *user_type;
	void ui_init();
	void regi_pass();
	void regi_fail();
	void r_oper_unable();
	void r_oper_able();
	bool ac_cert();
	void closeEvent(QCloseEvent *event);

public slots:
	void on_click();
	void regi_done(QByteArray t_array);

signals:
	void close_window();
};