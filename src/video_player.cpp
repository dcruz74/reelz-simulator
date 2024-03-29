#include "video_player.hpp"

VideoPlayer::VideoPlayer(){
	m_viewing_window = new QVideoWidget;
	m_viewing_window->setGeometry(1280, 720, 1280, 720);

	m_player = new QMediaPlayer;

	m_player->setVideoOutput(m_viewing_window);

	// Have not played a video when constructed
	m_last_played = -1;

	// Not currently playing video
	m_current_video = -1;

	const QSize sizeLong = QSize(200, 40);
	const QSize sizeSmall = QSize(100, 40);

	// Create the random button
	m_next = new QPushButton("Play");
	m_next->setFixedSize(sizeLong);
	m_next->setEnabled(false);

	// Create the Open... button
	m_open = new QPushButton("Open...");
	m_open->setFixedSize(sizeLong);

	// Create the About button
	m_about = new QPushButton("About");
	m_about->setFixedSize(sizeSmall);

	// Create the Help Button
	m_help = new QPushButton("Help");
	m_help->setFixedSize(sizeSmall);

	// Connect the button to a function
	connect(m_next, &QPushButton::clicked, this, &VideoPlayer::playMedia);
	connect(m_open, &QPushButton::clicked, this, &VideoPlayer::openFile);
	connect(m_about, &QPushButton::clicked, this, &VideoPlayer::aboutMenu);
	connect(m_help, &QPushButton::clicked, this, &VideoPlayer::helpMenu);

	// Connect m_player
	connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &VideoPlayer::mediaStatusChanged);
	
	// Creating the layout
	// For the randomize button
	QBoxLayout* buttons = new QHBoxLayout;
	buttons->setContentsMargins(0, 0, 0, 0);
	buttons->addWidget(m_next);
	buttons->addWidget(m_open);
	buttons->addWidget(m_about);
	buttons->addWidget(m_help);
	

	QBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(m_viewing_window);
	main_layout->addLayout(buttons);

	if(layout() == nullptr){
		this->setLayout(main_layout);
	}

	this->show();
}

void VideoPlayer::openFile(){
	m_player->pause();
	QStringList selection = QFileDialog::getOpenFileNames(this, "Open video..", QDir::homePath(),
														 "Videos (*.mp4)");

	if(selection.size() != 0){
		m_video_array = selection;
		m_player->stop();
		m_next->setText("Play");
		m_next->setEnabled(true);
	}
	else{
		m_player->play();
	}
}

void VideoPlayer::aboutMenu(){
	QMessageBox about(QMessageBox::NoIcon, QString("About"), QString("About"));
	about.setStyleSheet("QLabel{min-width: 300px;}");
	about.setStandardButtons(QMessageBox::Close);
	about.setTextFormat(Qt::RichText);
	about.setText(
				  "<p style='text-align:center'>"
				  "<b>Reelz Simulator</b><br>"
				  "version 1.0<br>"
				  "<br>"
				  "Developed exclusively for<br> ruly_m<br>"
				  "<br>"
				  "Copyright 2022<br>"
				  "Eat Butt Industries"
				  "</p>");
	about.exec();
}

void VideoPlayer::helpMenu(){
	QMessageBox help(QMessageBox::NoIcon, QString("Help"), QString("Help"));
	help.setStyleSheet("QLabel{min-width: 500px;}");
	help.setStandardButtons(QMessageBox::Close);		
	help.setTextFormat(Qt::RichText);
	help.setText(
				 "<ol>"
				 "<li> Click \"Open...\". </li>"
				 "<li> Select the first file, scroll all the way down, hold down the \"shift\" key & select the last file.</li>"
				"<li> Click \"select\" to select the files.</li>"
				 "<li> Click \"play\" to play the first reel.</li>"
				 "<li> Click \"next\" to go to the next reel.</li>"
				 "If this doesn't work, well, you're shit outta luck."
				 "</ol>"
				 );
	help.exec();
}


void VideoPlayer::getRandomVideo(){
	std::mt19937 rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());

	do{
		std::uniform_int_distribution<std::mt19937::result_type>dist6(0, m_video_array.size() - 1);
		m_current_video = dist6(rng);
	} while(m_current_video == m_last_played && m_video_array.size() > 1);
	
	m_last_played = m_current_video;

	this->m_player->setMedia(QUrl::fromLocalFile(this->m_video_array.at(m_current_video)));
}

void VideoPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus state){
	if(state == QMediaPlayer::EndOfMedia){
		this->m_player->setMedia(QUrl::fromLocalFile(this->m_video_array.at(m_current_video)));
		this->m_player->play();
	}
}


void VideoPlayer::playMedia(){
	this->m_viewing_window->show();
	m_next->setText("Next");
	
	this->getRandomVideo();
	this->m_player->play();
}
