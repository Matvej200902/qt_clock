#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QFontDialog>
#include <QContextMenuEvent>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void tick();
    void on_btnHideControls_clicked();
    void on_cbAutostart_stateChanged(int state);
    void on_pushButton_clicked();

    void changeFont();
    void toggleFullscreen();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    QPoint dragPosition;

    void setAutostart(bool enable);
    void saveFont(QFont font);
    void loadFont();
    bool expanded = false;
    QSize normalSize;
    QSize expandedSize;

};

#endif
