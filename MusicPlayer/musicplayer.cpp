#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QWidget(parent)
    , m_cur_index(-1)
    , m_number(0)
    , ui(new Ui::MusicPlayer)
    , m_listModel(new QStandardItemModel)
    , m_mediaPlayer(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);
    ui->musicListView->setModel(m_listModel);
    m_mediaPlayer->setAudioOutput(m_audioOutput);

    // 创建音量调节滑动条
    createVolumeSlider();
    ui->volume_btn->installEventFilter(this);  // 事件过滤器
    ui->volume_btn->setMouseTracking(true);

    // 连接音乐时长和标签与进度条
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, [=](qint64 ms){
        ui->totaltime->setText(QString("%1:%2").arg(ms/60000, 2, 10, QChar('0')).arg(ms/1000%60, 2, 10, QChar('0')));  //  m s
        ui->play_slider->setMaximum(ms / 100);
    });
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, [=](qint64 ms){
        ui->realtime->setText(QString("%1:%2").arg(ms/60000, 2, 10, QChar('0')).arg(ms/1000%60, 2, 10, QChar('0')));
        ui->play_slider->setValue(ms / 100);
    });
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

// 事件过滤
bool MusicPlayer::eventFilter(QObject *obj, QEvent *event)
{
    // 控制音量调节滑动条的显示和隐藏
    if (obj == ui->volume_btn) {
        if (event->type() == QEvent::Enter) {
            showVolumeSlider();
            return true;
        }
        else if (event->type() == QEvent::Leave) {
            QTimer::singleShot(250, [this](){
                if (!m_volumeSlider->underMouse())
                    hideVolumeSlider();
            });

            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

// 打开文件夹
void MusicPlayer::on_openDir_btn_clicked()
{
    // 打开文件夹对话框
    QString path = QFileDialog::getExistingDirectory(this, "选择文件夹");
    if (path.isEmpty()) {
        return;
    }

    // 获取目录下的文件
    QDirIterator it(path, {"*.mp3", "*.wav", "*.ogg", "*m4a"});
    while (it.hasNext()) {
        auto info = it.nextFileInfo();

        // 名称为文件名，数据为url
        QStandardItem *item = new QStandardItem(info.fileName());
        item->setData(QUrl::fromLocalFile(info.canonicalFilePath()), Qt::UserRole + 1);  // 将完整路径储存起来

        m_listModel->appendRow(item);
        ++m_number;
        qInfo() << info;
    }
}

// 双击获取乐曲索引，调用播放函数
void MusicPlayer::on_musicListView_doubleClicked(const QModelIndex &index)
{
    m_cur_index = index.row();
    playMusic();
}

// 播放和暂停
void MusicPlayer::on_play_btn_clicked()
{
    if (!m_mediaPlayer->isPlaying())
        m_mediaPlayer->play();
    else
        m_mediaPlayer->pause();
}

// 上一首
void MusicPlayer::on_pre_btn_clicked()
{
    if (m_cur_index < 0)
        return;

    m_cur_index = (m_cur_index - 1) % m_number;
    playMusic();
}

// 下一首
void MusicPlayer::on_next_btn_clicked()
{
    if (m_cur_index < 0)
        return;

    m_cur_index = (m_cur_index + 1) % m_number;
    playMusic();
}

// 根据索引播放音乐
void MusicPlayer::playMusic()
{
    if (m_cur_index < 0)
        return;

    QModelIndex index = m_listModel->index(m_cur_index, 0);
    ui->musicListView->setCurrentIndex(index);
    auto url = index.data(Qt::UserRole + 1).toUrl();
    m_mediaPlayer->setSource(url);
    m_mediaPlayer->play();
}

// 静音功能
void MusicPlayer::on_volume_btn_clicked()
{
    if (m_audioOutput->isMuted())
        m_audioOutput->setMuted(false);
    else
        m_audioOutput->setMuted(true);
}



// 音量调节功能
// 滑动条创建
void MusicPlayer::createVolumeSlider()
{
    m_volumeSlider = new QSlider(Qt::Vertical, this);

    m_volumeSlider->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint); // 关键！
    // m_volumeSlider->setAttribute(Qt::WA_TranslucentBackground); // 可选：透明背景
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setValue(100);
    m_volumeSlider->setFixedSize(32, 140);

    // 连接滑动事件
    connect(m_volumeSlider, &QSlider::valueChanged, this, &MusicPlayer::on_volumeSlider_valueChanged);
}

// 进入
void MusicPlayer::on_volumeBtn_entered()
{
    showVolumeSlider();
}

// 离开
void MusicPlayer::on_volumeBtn_left()
{
    hideVolumeSlider();
}

void MusicPlayer::showVolumeSlider()
{
    QPoint pos = ui->volume_btn->mapToGlobal(QPoint(0, 0));
    int x = pos.x();
    int y = pos.y() - m_volumeSlider->height();

    m_volumeSlider->move(x, y);
    m_volumeSlider->show();
    m_volumeSlider->raise();
    m_volumeSlider->activateWindow();
    m_volumeSlider->setFocus();
}

void MusicPlayer::hideVolumeSlider()
{
    m_volumeSlider->hide();
}

// 调节功能实现
void MusicPlayer::on_volumeSlider_valueChanged()
{
    float volume = m_volumeSlider->value() / 100.0f;
    m_audioOutput->setVolume(volume);
}
