#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QMessageBox>
#include "encryption.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void Prog(int);
    void Ready_to_start();

    void on_start_decipher_clicked();

    void on_button_start_clicked();

    void on_keyEdit_textChanged(const QString &arg1);

    void on_About_clicked();

private:
    QString key;
    Encryption *cipher = new Encryption(this);
    Ui::MainWindow *ui;
    QFileSystemModel *dir_Model;
    QString Path;
};

#endif // MAINWINDOW_H
