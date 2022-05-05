#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

public:
    QString generalLoopText(const QString& controller, const QString& templater);
    QString recursiveLoop(int level, const QString &temp);

private slots:
    void on_generalButton_clicked();

    void on_copyButton_clicked();

private:
    Ui::MainWindow *ui;
    QSettings settings;

    QList<QList<int>> calcCounts;
};
#endif // MAINWINDOW_H
