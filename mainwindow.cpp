#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QClipboard>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      settings(QSettings::IniFormat, QSettings::UserScope, "iwxyi", "LoopGenerator", this)
{
    ui->setupUi(this);

    ui->controllerEdit->setPlainText(settings.value("data/controller").toString());
    ui->templateEdit->setPlainText(settings.value("data/template").toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::generalLoopText(const QString &controller, const QString &templater)
{
    // 解析计数值
    calcCounts.clear();
    const QStringList& lines = controller.split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < lines.size(); i++)
    {
        const QStringList& cells = lines.at(i).split(" ", QString::SkipEmptyParts);
        if (cells.size() < 3)
        {
            QMessageBox::warning(this, "解析计数格式失败", "无法解析行格式：" + lines.at(i));
            return "";
        }

        int vals[3];
        for (int j = 0; j < 3; j++)
        {
            bool ok = true;
            vals[j] = cells.at(j).toInt(&ok);
            if (!ok)
            {
                QMessageBox::warning(this, "解析计数值失败", "无法将字符串转换为整型：" + cells.at(j) + "\n所在行：" + lines.at(i));
                return "";
            }
        }

        calcCounts.push_back({vals[0], vals[1], vals[2]});
        qInfo() << "计数" << i << ":" << vals[0] << vals[1] << vals[2];
    }

    // 生成文字
    return recursiveLoop(0, templater);
}

QString MainWindow::recursiveLoop(int level, const QString& temp)
{
    if (level >= calcCounts.size())
        return temp;

    QList<int> vals = calcCounts.at(level);
    int start = vals[0], step = vals[1], count = vals[2];

    int val = start;
    QString result;
    for (int i = 0; i < count; i++)
    {
        QString text = temp;
        text.replace("$" + QString::number(level+1), QString::number(val));
        result.append(recursiveLoop(level + 1, text));
        val += step;
    }

    return result;
}

void MainWindow::on_generalButton_clicked()
{
    settings.setValue("data/controller", ui->controllerEdit->toPlainText());
    settings.setValue("data/template", ui->templateEdit->toPlainText());
    ui->resultEdit->setPlainText(generalLoopText(ui->controllerEdit->toPlainText(), ui->templateEdit->toPlainText()));
}

void MainWindow::on_copyButton_clicked()
{
    QApplication::clipboard()->setText(ui->resultEdit->toPlainText());
}
