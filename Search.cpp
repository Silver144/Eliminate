#include "search.h"

QFontMetrics *g_matrics;
QFont g_font("Menlo", 18);
extern WSocket *tcp_socket;

Search::Search(QWidget *parent)
{
	ui_init();
	connect(return_btn, &QPushButton::clicked, [this] { emit close_window(); });
	connect(search_btn, SIGNAL(clicked()), this, SLOT(search_user()));
	connect(tcp_socket, SIGNAL(search_m(QByteArray)), this, SLOT(search_done(QByteArray)));
}

void Search::ui_init()
{

	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 960) / 2, (height - 720) / 2);
	this->setFixedSize(960, 720);

	QPixmap bg(":/image/bgs.png");
	QLabel *background = new QLabel(this);
	background->setPixmap(bg);
	// background->show();

	user_name = new QLineEdit(this);
	search_btn = new QPushButton("出道户籍", this);
	info_lab = new QLabel(this);
	name_lab = new QLabel(this);
	score_lab = new QLabel(this);
	exp_lab = new QLabel(this);
	level_lab = new QLabel(this);

	g_matrics = new QFontMetrics(g_font);
	info_lab->setFont(g_font);
	name_lab->setFont(g_font);
	score_lab->setFont(g_font);
	exp_lab->setFont(g_font);
	level_lab->setFont(g_font);

	info_lab->setGeometry(0, 0, 0, 0);
	//info_lab->show();
	/*name_lab->setGeometry(0, 0, 0, 0);
	score_lab->setGeometry(0, 0, 0, 0);
	exp_lab->setGeometry(0, 0, 0, 0);
	level_lab->setGeometry(0, 0, 0, 0);*/
	user_name->setGeometry(90, 60, 221, 41);
	search_btn->setGeometry(340, 60, 131, 41);
	
	QPixmap icon1(":/image/flake.png");
	return_btn = new QPushButton(this);
	return_btn->setGeometry(832, 592, 128, 128);
	return_btn->setIcon(icon1);
	return_btn->setIconSize(QSize(128, 128));
	return_btn->setFlat(true);
	return_btn->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
	return_btn->setCursor(QCursor(Qt::PointingHandCursor));

	user_name->setPlaceholderText("输入你想出道户籍的壬");
}

void Search::search_user()
{
	int level, exp, hiscore, flag = -1;
	std::string t_str;
	QString name = user_name->text();

	QString send_data;
	send_data.append(QChar(3));
	send_data.append(name);

	tcp_socket->socket->write(send_data.toUtf8().data());
	
}

void Search::search_done(QByteArray t_array)
{
	int level, exp, hiscore, cnt = 0;
	std::string search_msg = QString(t_array).toStdString().substr(2);
	if (t_array.at(1) != 3)
	{
		exp = std::stoi(search_msg.substr(1, search_msg[0]));
		cnt = cnt + search_msg[0] + 1;
		level = std::stoi(search_msg.substr(cnt + 1, search_msg[cnt]));
		if (t_array.at(1) == 2)
		{
			cnt = cnt + search_msg[cnt] + 1;
			hiscore = std::stoi(search_msg.substr(cnt + 1, search_msg[cnt]));
		}
	}
	info_show(exp, level, hiscore, t_array.at(1));
}

void Search::info_show(int exp, int level, int hiscore, int flag)
{
	std::string name = user_name->text().toStdString();

	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::white);

	if (flag == 3)
	{
		info_lab->setText("No match");
		info_lab->setGeometry(100, 190, 151, 51);
		info_lab->show();
		name_lab->hide();
		level_lab->hide();
		exp_lab->hide();
		score_lab->hide();
	}
	else if (flag == 1)
	{
		info_lab->setText("Name:\nLevel:\nExp:");
		info_lab->setGeometry(100, 190, 121, 231);
		info_lab->setPalette(pa);

		name_lab->setText(QString::fromStdString(name));
		name_lab->setGeometry(250, 202, 200, 40);
		name_lab->setPalette(pa);


		level_lab->setText(QString::number(level));
		level_lab->setGeometry(250, 258, 200, 40);
		level_lab->setPalette(pa);

		exp_lab->setText(QString::number(exp) + "/" + QString::number(level * (level * 100 - 50)));
		exp_lab->setGeometry(250, 312, 200, 40);
		exp_lab->setPalette(pa);
	}
	else if (flag == 2)
	{
		info_lab->setText("Name\n\nLevel\n\nHiscore\n\nExp");
		info_lab->setGeometry(100, 190, 121, 231);
		info_lab->setPalette(pa);

		//info_lab->show();
		name_lab->setText(QString::fromStdString(name));
		name_lab->setGeometry(250, 202, 200, 40);
		name_lab->setPalette(pa);

		level_lab->setText(QString::number(level));
		level_lab->setGeometry(250, 258, 200, 40);
		level_lab->setPalette(pa);

		score_lab->setText(QString::number(hiscore));
		score_lab->setGeometry(250, 312, 200, 40);
		score_lab->setPalette(pa);

		exp_lab->setText(QString::number(exp) + "/" + QString::number(level * (level * 100 - 50)));
		exp_lab->setGeometry(250, 372, 200, 40);
		exp_lab->setPalette(pa);
	}
}

void Search::closeEvent(QCloseEvent *event)
{
	emit close_window();
}