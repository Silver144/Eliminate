#include "rank.h"
#include "socket.h"

QFontMetrics *r_matrics;
QFont r_font("Menlo", 18, QFont::Bold);
extern WSocket *tcp_socket;
int rank_cnt;

Rank::Rank(int t_flag, QWidget *parent) : flag(t_flag)
{
	ui_init();
	connect(return_btn, &QPushButton::clicked, [this] { emit close_window(); });
	connect(tcp_socket, SIGNAL(rank_mp(QByteArray)), this, SLOT(rank_pdone(QByteArray)));
	connect(tcp_socket, SIGNAL(rank_me(QByteArray)), this, SLOT(rank_edone(QByteArray)));
	rank_cnt = 0;
}

void Rank::handle_rank()
{
	if (rank_cnt > 0)
		return;
	QString send_data;
	if (flag)
		send_data.append(QChar(4));
	else
		send_data.append(QChar(9));
	tcp_socket->socket->write(send_data.toUtf8().data());
	rank_cnt++;
}

void Rank::rank_edone(QByteArray t_array)
{
	std::string examiner_info = QString(t_array).toStdString().substr(1);
	int inst = 0;
	while (examiner_info[inst] != 11)
	{
		int t_len = examiner_info[inst];
		std::string user_name = examiner_info.substr(inst + 1, t_len);
		inst = inst + t_len + 1;

		provider *t_examiner;
		t_examiner = new provider(user_name);

		t_len = examiner_info[inst];
		t_examiner->exp = std::stoi(examiner_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		t_len = examiner_info[inst];
		t_examiner->level = std::stoi(examiner_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		t_len = examiner_info[inst];
		t_examiner->word_cnt = std::stoi(examiner_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		p_info.push_back(t_examiner);
	}

	std::sort(p_info.begin(), p_info.end(), [](const provider *a1, const provider *a2)-> bool { return *a1 < *a2; });
	show_info(std::min(10, int(p_info.size())));
}

void Rank::rank_pdone(QByteArray t_array)
{
	std::string player_info = QString(t_array).toStdString().substr(1);
	int inst = 0;
	while (player_info[inst] != 11)
	{
		int t_len = player_info[inst];
		std::string user_name = player_info.substr(inst + 1, t_len);
		inst = inst + t_len + 1;

		challenger *t_player;
		t_player = new challenger(user_name);

		t_len = player_info[inst];
		t_player->exp = std::stoi(player_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		t_len = player_info[inst];
		t_player->high_score = std::stoi(player_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		t_len = player_info[inst];
		t_player->level = std::stoi(player_info.substr(inst + 1, t_len));
		inst = inst + t_len + 1;

		c_info.push_back(t_player);
	}

	std::sort(c_info.begin(), c_info.end(), [](const challenger *a1, const challenger *a2)-> bool { return *a1 < *a2; });
	show_info(std::min(10, int(c_info.size())));
}

void Rank::show_info(int size)
{
	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::white);

	QLabel *t_id = new QLabel(this);
	t_id->setText("No. ");
	t_id->setFont(r_font);
	t_id->setPalette(pa);
	id.push_back(t_id);

	QLabel *t_name = new QLabel(this);
	t_name->setFont(r_font);
	t_name->setText("Name");
	t_name->setPalette(pa);
	name.push_back(t_name);

	QLabel *t_exp = new QLabel(this);
	t_exp->setFont(r_font);
	t_exp->setText("Exp");
	t_exp->setPalette(pa);
	exp.push_back(t_exp);

	QLabel *t_score = new QLabel(this);
	t_score->setFont(r_font);
	t_score->setText(flag ? "Score" : "Word");
	t_score->setPalette(pa);
	score.push_back(t_score);

	QLabel *t_level = new QLabel(this);
	t_level->setFont(r_font);
	t_level->setText("Level");
	t_level->setPalette(pa);
	level.push_back(t_level);

	for (int i = 0; i < size; i++)
	{
		QLabel *t_id = new QLabel(this);
		t_id->setFont(r_font);
		t_id->setText(QString::number(i + 1));
		t_id->setPalette(pa);
		id.push_back(t_id);

		QLabel *t_name = new QLabel(this);
		t_name->setFont(r_font);
		t_name->setText(QString::fromStdString((flag ? c_info[i]->name.substr(1) : p_info[i]->name.substr(1))));
		t_name->setPalette(pa);
		name.push_back(t_name);

		QLabel *t_exp = new QLabel(this);
		t_exp->setFont(r_font);
		t_exp->setText(QString::number((flag ? c_info[i]->exp : p_info[i]->exp)) + "/" + 
			QString::number((flag ? c_info[i]->level * (c_info[i]->level * 100 - 50) : p_info[i]->level * (p_info[i]->level * 100 - 50))));
		t_exp->setPalette(pa);
		exp.push_back(t_exp);

		QLabel *t_score = new QLabel(this);
		t_score->setFont(r_font);
		t_score->setText(QString::number((flag ? c_info[i]->high_score : p_info[i]->word_cnt)));
		t_score->setPalette(pa);
		score.push_back(t_score);

		QLabel *t_level = new QLabel(this);
		t_level->setFont(r_font);
		t_level->setText(QString::number((flag ? c_info[i]->level : p_info[i]->level)));
		t_level->setPalette(pa);
		level.push_back(t_level);
	}

	for (int i = 0; i < size + 1; i++)
	{
		id[i]->setGeometry(100, 80 + i * 48, 50, 30);
		id[i]->show();
		name[i]->setGeometry(200, 80 + i * 48, 180, 30);
		name[i]->show();
		level[i]->setGeometry(400, 80 + i * 48, 80, 30);
		level[i]->show();
		exp[i]->setGeometry(540, 80 + i * 48, 160, 30);
		exp[i]->show();
		score[i]->setGeometry(710, 80 + i * 48, 120, 30);
		score[i]->show();
	}
}

void Rank::ui_init()
{
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 960) / 2, (height - 720) / 2);
	this->setFixedSize(960, 720);

	QPixmap bg(":/image/bgs.png");
	background = new QLabel(this);
	background->setPixmap(bg);


	QPixmap icon1(":/image/flake.png");
	return_btn = new QPushButton(this);
	return_btn->setGeometry(832, 592, 128, 128);
	return_btn->setIcon(icon1);
	return_btn->setIconSize(QSize(128, 128));
	return_btn->setFlat(true);
	return_btn->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
	return_btn->setCursor(QCursor(Qt::PointingHandCursor));
}