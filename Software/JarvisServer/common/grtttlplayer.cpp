#include "grtttlplayer.h"
#include "ui_grtttlplayer.h"

gRtttlPlayer::gRtttlPlayer(sJarvisNodeComponent* comp) :
    QWidget(),
    ui(new Ui::gRtttlPlayer)
{
    ui->setupUi(this);
    m_component = comp;
    connect(comp,SIGNAL(destroyed(QObject*)),this,SLOT(setDisabled(bool)));
}

gRtttlPlayer::~gRtttlPlayer()
{
    delete ui;        
}


void gRtttlPlayer::on_playBtn_clicked()
{
    if(ui->lineEdit->text().isEmpty()) return;

    m_component->playRtttl(ui->lineEdit->text());
}


void gRtttlPlayer::on_pacmanBtn_clicked()
{
    ui->lineEdit->setText(Pacman);
    m_component->playRtttl(Pacman);
}

void gRtttlPlayer::on_tetrisBtn_clicked()
{
    ui->lineEdit->setText(Tetris);
    m_component->playRtttl(Tetris);
}

void gRtttlPlayer::on_simpsonsBtn_clicked()
{
    ui->lineEdit->setText(Simpsons);
    m_component->playRtttl(Simpsons);
}
