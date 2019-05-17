#include "WEli.h"

std::vector<word_inf> word_db;
std::mt19937 rand_num;

extern WSocket *tcp_socket;

WEli::WEli(QWidget *parent)
{
	login_window = new loginW(this);
	connect(login_window, SIGNAL(ls_to(std::string)), this, SLOT(ls_from(std::string)));
	connect(tcp_socket, SIGNAL(word_m(QByteArray)), this, SLOT(word_done(QByteArray)));
	connect(tcp_socket, SIGNAL(ip_m(QByteArray)), this, SLOT(challenger_done(QByteArray)));
	connect(tcp_socket, SIGNAL(ie_m(QByteArray)), this, SLOT(examiner_done(QByteArray)));
	/*connect(start_btn, SIGNAL(clicked()), this, SLOT(start_game()));
	connect(search_btn, SIGNAL(clicked()), this, SLOT(start_search()));*/
}

void WEli::word_done(QByteArray t_array)
{
	int word_clen = 1;
	std::string t_str = QString(t_array).toStdString().substr(1);
	int word_len = t_str[0];
	while (word_len < 20)
	{
		std::string wt_str = t_str.substr(word_clen, word_len);
		word_db.push_back(word_inf(wt_str));
		word_clen += word_len;
		word_len = t_str[word_clen];
		word_clen++;
	}
}

void WEli::ls_from(std::string c_name)
{
	game_win = new Game(this);
	connect(game_win, &Game::close_window, [this] { game_win->hide(); this->show(); });
	if (c_name[0] == '1')
		init_challenger(QString::fromStdString(c_name));
	else
		init_examiner(QString::fromStdString(c_name));
	init(c_name);
	delete login_window;
	this->show();
}

void WEli::init_examiner(QString c_name)
{
	QString write_data;
	write_data.append(QChar(8));
	write_data.append(QChar(c_name.size()));
	write_data.append(c_name);
	tcp_socket->socket->write(write_data.toUtf8().data());
}

void WEli::examiner_done(QByteArray t_array)
{
	std::string info = QString(t_array).toStdString().substr(1);
	int clen = 0;
	std::string name = info.substr(clen + 1, info[clen]);
	clen = clen + info[clen] + 1;

	game_win->examiner = new provider(name);
	game_win->cp = 0;

	game_win->examiner->exp = std::stoi(info.substr(clen + 1, info[clen]));
	clen = clen + info[clen] + 1;
	
	game_win->examiner->level = std::stoi(info.substr(clen + 1, info[clen]));
	clen = clen + info[clen] + 1;

	game_win->examiner->word_cnt = std::stoi(info.substr(clen + 1, info[clen]));
	clen = clen + info[clen] + 1;

	for (int i = 0; i < game_win->examiner->word_cnt; i++)
	{
		std::string c_str = info.substr(clen + 1, info[clen]);
		game_win->examiner->word_buf.push_back(c_str);
		clen = clen + info[clen] + 1;
	}
}

void WEli::init_challenger(QString c_name)
{
	QString write_data;
	write_data.append(QChar(5));
	write_data.append(QChar(c_name.size()));
	write_data.append(c_name);
	tcp_socket->socket->write(write_data.toUtf8().data());
}

void WEli::challenger_done(QByteArray t_array)
{
	std::string info = QString(t_array).toStdString().substr(1);
	int clen = 0;
	std::string name = info.substr(clen + 1, info[clen]);
	clen = clen + info[clen] + 1;
	game_win->player = new challenger(name);
	game_win->cp = 1;
	
	game_win->player->exp = std::stoi(info.substr(clen + 1, info[clen]));
	clen = clen + info[clen] + 1;
	game_win->player->high_score = std::stoi(info.substr(clen + 1, info[clen]));
	clen = clen + info[clen] + 1;
 	game_win->player->level = std::stoi(info.substr(clen + 1, info[clen]));

	QString send_data;
	send_data.append(QChar(2));
	tcp_socket->socket->write(send_data.toUtf8().data());
}

void WEli::init(std::string c_name)
{
	ui_init();
	search_win = new Search();
	connect(search_win, &Search::close_window, [this] { search_win->hide(); this->show(); });
	rank_win = new Rank(std::stoi(c_name.substr(0, 1)));
	connect(rank_win, &Rank::close_window, [this] { rank_win->hide(); this->show(); });
}

void WEli::start_game()
{
	game_win->show();
	this->hide();
}

void WEli::start_search()
{
	search_win->show();
	this->hide();
}

void WEli::start_rank()
{
	rank_win->handle_rank();
	rank_win->show();
	this->hide();
}

void WEli::ui_init()
{

	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 960) / 2, (height - 720) / 2);

	this->setFixedSize(960, 720);
	

	QPixmap bg(":/image/bg.png");
	QLabel *background = new QLabel(this);
	background->setPixmap(bg);

	/*start_btn = new QPushButton("start", this);
	start_btn->setGeometry(100, 500, 40, 40);

	search_btn = new QPushButton("search", this);
	search_btn->setGeometry(400, 500, 30, 60);*/
	QPixmap start_pix(":/image/start_a.png");
	start_lab = new QLabel(this);
	start_lab->setPixmap(start_pix);
	start_lab->setGeometry(699, 400, 149, 37);

	QPixmap search_pix(":/image/search_a.png");
	search_lab = new QLabel(this);
	search_lab->setPixmap(search_pix);
	search_lab->setGeometry(699, 448, 149, 37);

	QPixmap rank_pix(":/image/rank_a.png");
	rank_lab = new QLabel(this);
	rank_lab->setPixmap(rank_pix);
	rank_lab->setGeometry(699, 496, 149, 37);

	QPixmap exit_pix(":/image/exit_a.png");
	exit_lab = new QLabel(this);
	exit_lab->setPixmap(exit_pix);
	exit_lab->setGeometry(699, 544, 149, 37);

	QPixmap start_pix2(":/image/start_b.png");
	start_lab2 = new QLabel(this);
	start_lab2->setPixmap(start_pix2);
	start_lab2->setGeometry(699, 400, 149, 37);
	

	QPixmap search_pix2(":/image/search_b.png");
	search_lab2 = new QLabel(this);
	search_lab2->setPixmap(search_pix2);
	search_lab2->setGeometry(699, 448, 149, 37);

	QPixmap rank_pix2(":/image/rank_b.png");
	rank_lab2 = new QLabel(this);
	rank_lab2->setPixmap(rank_pix2);
	rank_lab2->setGeometry(699, 496, 149, 37);

	QPixmap exit_pix2(":/image/exit_b.png");
	exit_lab2 = new QLabel(this);
	exit_lab2->setPixmap(exit_pix2);
	exit_lab2->setGeometry(699, 544, 149, 37);
	
	start_lab->hide();
	search_lab2->hide();
	rank_lab2->hide();
	exit_lab2->hide();

	state = 1;
	// start_btn->setEnabled(false);
}


void WEli::keyPressEvent(QKeyEvent *k)
{
	int key = k->key();
	switch (key)
	{
		case Qt::Key_Return:
		{
			switch (state)
			{
			case 1:
			{
				start_game();
				break;
			}
			case 2:
			{
				start_search();
				break;
			}
			case 3:
			{
				start_rank();
				break;
			}
			default:
				exit(0);
			}
			break;
		}
		case Qt::Key_Up:
		{
			switch (state)
			{
				case 1:
				{
					state = 4;
					start_lab2->hide();
					start_lab->show();
					exit_lab->hide();
					exit_lab2->show();
					break;
				}
				case 2:
				{
					state = 1;
					search_lab2->hide();
					search_lab->show();
					start_lab->hide();
					start_lab2->show();
					break;
				}
				case 3:
				{
					state = 2;
					rank_lab2->hide();
					rank_lab->show();
					search_lab->hide();
					search_lab2->show();
					break;
				}
				default:
				{
					state = 3;
					exit_lab2->hide();
					exit_lab->show();
					rank_lab->hide();
					rank_lab2->show();
				}
			}
			break;
		}
		case Qt::Key_Down:
		{
			switch (state)
			{
				case 1:
				{
					state = 2;
					start_lab2->hide();
					start_lab->show();
					search_lab->hide();
					search_lab2->show();
					break;
				}
				case 2:
				{
					state = 3;
					search_lab2->hide();
					search_lab->show();
					rank_lab->hide();
					rank_lab2->show();
					break;
				}
				case 3:
				{
					state = 4;
					rank_lab2->hide();
					rank_lab->show();
					exit_lab->hide();
					exit_lab2->show();
					break;
				}
				default:
				{
					state = 1;
					exit_lab2->hide();
					exit_lab->show();
					start_lab->hide();
					start_lab2->show();
				}
			}
			break;
		}
		default: ;
	}
	qDebug() << state << "\n";
}