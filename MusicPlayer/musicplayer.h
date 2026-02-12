#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QFileDialog>
#include <QDirIterator>
#include <QStandardItemModel>  // 用来存储乐曲路径
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MusicPlayer;
}
QT_END_NAMESPACE

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

private slots:
    void on_openDir_btn_clicked();

    void on_musicListView_doubleClicked(const QModelIndex &index);

    void on_play_btn_clicked();

    void on_next_btn_clicked();

    void on_pre_btn_clicked();

    void on_volume_btn_clicked();

    void on_volumeSlider_valueChanged();

private:

    Ui::MusicPlayer *ui;
    QStandardItemModel *m_listModel;
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    QSlider *m_volumeSlider;
    int m_cur_index;   // 当前播放的乐曲
    int m_number;   // 列表中的乐曲数量

    void playMusic();

    bool eventFilter(QObject *obj, QEvent *event);

    void createVolumeSlider(); // 创建音量调节滑动条
    void on_volumeBtn_entered();  // 进入
    void on_volumeBtn_left();   // 离开
    void showVolumeSlider();   // 显示
    void hideVolumeSlider();   // 隐藏

};
#endif // MUSICPLAYER_H
