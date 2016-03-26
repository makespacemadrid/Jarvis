#ifndef GSIMPLEGRAPH_H
#define GSIMPLEGRAPH_H

#include <QFrame>
#include <QMouseEvent>
#include <QVector>

namespace Ui {
class gSimpleGraph;
}

class gSimpleGraph : public QFrame
{
    Q_OBJECT

public:
    explicit gSimpleGraph(QWidget *parent = 0,QVector<QString> fields = QVector<QString>(),quint16 maxdata=200);
    ~gSimpleGraph();
    void setFields(QVector<QString> fields);
    QVector<QString> fields() {return m_fields;}
    void appendData(QVector<double> doubles);
    void addMark(QColor c=Qt::red);
private:
    Ui::gSimpleGraph *ui;
    void paintEvent(QPaintEvent *pe);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void draw_graph(QPainter* p, QRect rect);

    quint8  m_loopCount;
    quint16 m_maxData;
    bool m_draggingEvent;
    QVector<QString> m_fields;
    QVector<QVector<double> >   m_data;
    QVector<QColor>  m_colors;
    QVector<int>     m_marks;
    QVector<QColor>  m_marksColors;
    quint16 m_lastMX;
    quint16 m_lastMY;
};

#endif // GSIMPLEGRAPH_H
