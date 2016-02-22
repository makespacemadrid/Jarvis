#ifndef GRTTTLPLAYER_H
#define GRTTTLPLAYER_H

#include <QWidget>
#include "sjarvisnodecomponent.h"


namespace Ui {
class gRtttlPlayer;
}

class gRtttlPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit gRtttlPlayer(sJarvisNodeComponent* comp);
    ~gRtttlPlayer();

    const QString Pacman    = "PacMan:d=16,o=6,b=140:b5,b,f#,d#,8b,8d#,c,c7,g,f,8c7,8e,b5,b,f#,d#,8b,8d#,32d#,32e,f,32f,32f#,g,32g,32g#,a,8b";
    const QString Simpsons  = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
    //char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
    const QString Tetris    = "tetris:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a,a" ;
private:
    Ui::gRtttlPlayer *ui;
    sJarvisNodeComponent*   m_component;

protected slots:
    void on_playBtn_clicked();
    void on_pacmanBtn_clicked();
    void on_tetrisBtn_clicked();
    void on_simpsonsBtn_clicked();
};

#endif // GRTTTLPLAYER_H
