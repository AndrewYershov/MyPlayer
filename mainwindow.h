#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QFileDialog>
#include <QSlider>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QProgressBar>
#include "videowidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionadd_triggered();

    void on_actionplay_triggered();

    void on_actionpause_triggered();

    void on_actionstop_triggered();

    void on_actionback_triggered();

    void on_actionnext_triggered();

    void on_actionrepeat_triggered();

    void on_actionrandom_triggered();

    void on_mute_clicked();

    void on_timeLine_sliderMoved(int position);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    VideoWidget *m_vw;
    QSlider *m_slider;
    QStandardItemModel *m_playlistModel;
    QVBoxLayout *m_layout;
    QProgressBar *m_bar;
};
#endif // MAINWINDOW_H
