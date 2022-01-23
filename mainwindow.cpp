#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_player = new QMediaPlayer(this);
    m_playlist = new QMediaPlaylist(this);
    m_player->setPlaylist(m_playlist);

    // для відтворення відео в listView
    m_layout = new QVBoxLayout(this);
    m_vw = new VideoWidget(this);
    m_vw->resize(ui->listView->size());
    m_layout->addWidget(m_vw);
    ui->listView->setLayout(m_layout);
    m_player->setVideoOutput(m_vw);

    m_playlistModel = new QStandardItemModel(this);
    ui->tableView->setModel(m_playlistModel); // відображає файли додані в playlist tableView
    m_playlistModel->setHorizontalHeaderLabels(QStringList()<<tr("Playlist")<<tr("File Path")); // відображаємо заголовок tabView

    // відповідає за формування таблиці з треками в tableView
    ui->tableView->hideColumn(1);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // відображення таймлінії в toolBar
    m_slider = new QSlider(this);
    m_bar = new QProgressBar(this);

    m_slider->setOrientation(Qt::Horizontal);
    ui->toolBar->addWidget(m_slider);
    ui->toolBar->addWidget(m_bar);

    connect(m_player,&QMediaPlayer::durationChanged,m_slider,&QSlider::setMaximum);
    connect(m_player,&QMediaPlayer::positionChanged,m_slider,&QSlider::setValue);
    connect(m_slider,&QSlider::sliderMoved,m_player,&QMediaPlayer::setPosition);
    connect(m_player,&QMediaPlayer::durationChanged,m_bar,&QProgressBar::setMaximum);
    connect(m_player,&QMediaPlayer::positionChanged,m_bar,&QProgressBar::setValue);

    // при зміні індексу поточного файлу в підбірці, відображаємо назву файлу в статус барі
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index){
                ui->statusbar->showMessage(m_playlistModel->data(m_playlistModel->index(index, 0)).toString());});

    // при подвійному кліку по треку в таблиці встановлюємо файл в підбірку
    connect(ui->listView,&QTableView::doubleClicked,[this](const QModelIndex &index){m_playlist->setCurrentIndex(index.row());});
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
    delete m_playlist;
    delete m_playlistModel;
    delete m_vw;
    delete m_layout;
    delete m_slider;
    delete m_bar;
}


void MainWindow::on_actionadd_triggered()
{
    // Для того щоб в каталогах ПК можна було виділити більше одного файлу і його розширення
    QStringList filename = QFileDialog::getOpenFileNames(this,tr("Open a file"),QString(),tr("File(*.mp3 *.mp4 *.mkv)"));
    foreach(QString filePath,filename)
    {
        QList<QStandardItem*>items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        m_playlistModel->appendRow(items);
        m_playlist->addMedia(QUrl::fromLocalFile(filePath));
    }
}


void MainWindow::on_actionplay_triggered()
{
    m_player->play();
}


void MainWindow::on_actionpause_triggered()
{
    m_player->pause();
}


void MainWindow::on_actionstop_triggered()
{
    m_player->stop();
}


void MainWindow::on_actionback_triggered()
{
    m_playlist->previous();
}


void MainWindow::on_actionnext_triggered()
{
    m_playlist->next();
}


void MainWindow::on_actionrepeat_triggered()
{
    static bool flag = true;
    if(flag == true)
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
        flag = false;
    }
    else
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        flag = true;
    }
}


void MainWindow::on_actionrandom_triggered()
{
    static bool flag = true;
    if(flag == true)
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
        flag = false;
    }
    else
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        flag = true;
    }
}


void MainWindow::on_mute_clicked()
{
    bool mute = m_player->isMuted();
    m_player->setMuted(!mute);
}


void MainWindow::on_timeLine_sliderMoved(int position)
{
    m_player->setVolume(position);
}

// під час подвійного кліку в tableView (в списку доданих файлів) починає відтворюватись файл
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int rowNo = index.row();
    m_playlist->setCurrentIndex(rowNo);
    m_player->play();
}
