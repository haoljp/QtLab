#ifndef WIDGET_H_
#define WIDGET_H_

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QObject>
#include <QColorDialog>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget():m_pos(-1),m_dragging(false),m_deleted(false),m_color(Qt::red)
    {
    setMouseTracking(true);
        m_meun = new QMenu;
        QAction *changeColorAction = new QAction(tr("change color"), this);
        QAction *delAction = new QAction(tr("delete"), this);
        m_meun->addAction(changeColorAction);
        m_meun->addAction(delAction);
        connect(delAction, SIGNAL(triggered()), this, SLOT(slotDel()));
		connect(changeColorAction, SIGNAL(triggered()), this, SLOT(slotColorChanged()));
    }

    virtual void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setPen(QPen(QBrush(m_color), 3));
        if (m_pos == -1) m_pos = rect().left() + 100;
        if (!m_deleted)
            painter.drawLine(QPoint(m_pos, rect().top()), QPoint(m_pos, rect().bottom()));
    }

    virtual void mouseMoveEvent(QMouseEvent *event)
    {
        if ((event->pos().x() >= m_pos-2 && event->pos().x() <= m_pos+2) || m_dragging)
            setCursor(Qt::SizeHorCursor);
        else
            setCursor(QCursor());

        if (m_dragging && rect().contains(event->pos()))
        {
            m_pos = event->pos().x();
            update();
        }
    }

    virtual void mousePressEvent(QMouseEvent *event)
    {
        if (event->pos().x() >= m_pos-1 && event->pos().x() <= m_pos+1)
            m_dragging = true;
    }

    virtual void mouseReleaseEvent(QMouseEvent *)
    {
        if (m_dragging)
            m_dragging = false;
    }

    virtual void contextMenuEvent(QContextMenuEvent *)
    {
        m_dragging = false;
        m_meun->exec(cursor().pos());
    }

	virtual void mouseDoubleClickEvent(QMouseEvent *event)
	{
		if (rect().contains(event->pos()) && m_deleted)
		{
			m_pos = event->pos().x();
			m_deleted = false;
			update();
		}
	}

private slots:
    void slotDel() { m_deleted = true; update(); }
	void slotColorChanged()
	{
		QColor color = QColorDialog::getColor(m_color, this, "Select Color", QColorDialog::DontUseNativeDialog);
		if (color.isValid())
		{
			m_color = color;
			update();
		}
	}

private:
    int m_pos;
    bool m_dragging;
    bool m_deleted;
    QMenu *m_meun;
	QColor m_color;
};

#endif