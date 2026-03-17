#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QScreen>
#include <QGuiApplication>
#include <QFontDatabase>
#include <QCoreApplication>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("ClockApp", "ClockApp");

    ui->cbShowDate->setChecked(settings.value("showDate", false).toBool());
    ui->cbShowWeekday->setChecked(settings.value("showWeekday", false).toBool());

    normalSize = size();
    expandedSize = QSize(400, 200);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::tick);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    timer->start(1000);

    int fontId = QFontDatabase::addApplicationFont(":/fonts/SuboleyaRegular.ttf");

    if (fontId != -1)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(fontFamily);
        font.setPointSize(28);

        ui->label_time->setFont(font);
    }

    loadFont();

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = screenGeometry.width() - this->width();
    int y = 0;

    move(x, y);
}

MainWindow::~MainWindow()
{
    QSettings settings("ClockApp", "ClockApp");

    settings.setValue("showDate", ui->cbShowDate->isChecked());
    settings.setValue("showWeekday", ui->cbShowWeekday->isChecked());

    delete ui;
}

void MainWindow::tick()
{
    ui->label_time->setText(QDateTime::currentDateTime().toString("HH:mm:ss"));

    if (ui->cbShowDate->checkState())
        ui->label_time->setText(ui->label_time->text() + "\n" +
                                QDateTime::currentDateTime().toString("yyyy:MM:dd"));

    if (ui->cbShowWeekday->checkState())
        ui->label_time->setText(ui->label_time->text() + "\n" +
                                QDateTime::currentDateTime().toString("dddd"));
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    QAction *fontAction = menu.addAction("Выбрать шрифт");
    QAction *fullscreenAction = menu.addAction("Развернуть / вернуть");

    connect(fontAction, &QAction::triggered, this, &MainWindow::changeFont);
    connect(fullscreenAction, &QAction::triggered, this, &MainWindow::toggleFullscreen);

    menu.exec(event->globalPos());
}

void MainWindow::changeFont()
{
    bool ok;

    QFont font = QFontDialog::getFont(&ok, ui->label_time->font(), this);

    if (ok)
    {
        ui->label_time->setFont(font);
        saveFont(font);
    }
}

void MainWindow::toggleFullscreen()
{
    if (!expanded)
    {
        normalSize = size();
        resize(expandedSize);
        expanded = true;
    }
    else
    {
        resize(normalSize);
        expanded = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragPosition = event->globalPos() - frameGeometry().topLeft();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
        move(event->globalPos() - dragPosition);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QFont font = ui->label_time->font();

    int size = font.pointSize();

    if (event->angleDelta().y() > 0)
        size++;
    else
        size--;

    if (size < 5)
        size = 5;

    font.setPointSize(size);

    ui->label_time->setFont(font);
    saveFont(font);
}

void MainWindow::saveFont(QFont font)
{
    QSettings settings("ClockApp", "ClockApp");
    settings.setValue("font", font);
}

void MainWindow::loadFont()
{
    QSettings settings("ClockApp", "ClockApp");

    if (settings.contains("font"))
    {
        QFont font = settings.value("font").value<QFont>();
        ui->label_time->setFont(font);
    }
}

void MainWindow::on_btnHideControls_clicked()
{
    ui->cbShowDate->hide();
    ui->cbShowWeekday->hide();
    ui->btnHideControls->hide();
}

void MainWindow::setAutostart(bool enable)
{
    QSettings settings("", QSettings::NativeFormat);

    QString appName = "ClockApp";
    QString appPath = QCoreApplication::applicationFilePath().replace("/", "\\");

    if (enable)
        settings.setValue(appName, appPath);
    else
        settings.remove(appName);
}

void MainWindow::on_cbAutostart_stateChanged(int state)
{
    setAutostart(state);
}

void MainWindow::on_pushButton_clicked()
{
    QApplication::quit();
}
