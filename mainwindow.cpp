#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(cipher, &Encryption::Set_progress, this, &MainWindow::Prog);
    QObject::connect(cipher, &Encryption::End_ecryption, this, &MainWindow::Ready_to_start);
    QString mPath = "C:/";
    dir_Model = new QFileSystemModel(this);
    dir_Model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
    dir_Model->setRootPath(mPath);
    ui->treeView->setModel(dir_Model);
    ui->button_start->setEnabled(false);
    ui->start_decipher->setEnabled(false);
    ui->progressBar->setRange(0,100);
    ui->progressBar->reset();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QFileInfo file(dir_Model->fileInfo(index).absoluteFilePath());
    if (file.isFile()){
        if(cipher->is_ready){
            if (key.count() == 8) {
                ui->button_start->setEnabled(true);
                ui->start_decipher->setEnabled(true);
            }
        }
        Path = dir_Model->fileInfo(index).absoluteFilePath();
    }
    else {
        ui->start_decipher->setEnabled(false);
        ui->button_start->setEnabled(false);
    }
}

void MainWindow::on_start_decipher_clicked()
{
    QString sec = Path;
    sec.chop(5);
    ui->start_decipher->setEnabled(false);
    ui->button_start->setEnabled(false);
    ui->progressBar->reset();
    cipher->setting(Path, sec, key);
    cipher->start();
}

void MainWindow::on_button_start_clicked()
{
    ui->start_decipher->setEnabled(false);
    ui->button_start->setEnabled(false);
    ui->progressBar->reset();
    cipher->setting(Path, Path + ".rpks", key);
    cipher->start();
}

void MainWindow::Prog(int new_progress){
    ui->progressBar->setValue(new_progress);
}

void MainWindow::Ready_to_start(){
    QFileInfo file(Path);
    if (file.isFile()){
        ui->button_start->setEnabled(true);
        ui->start_decipher->setEnabled(true);
    }
}

void MainWindow::on_keyEdit_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    key = arg1;
    if (key.count() == 8) {
        QFileInfo file(Path);
        if (file.isFile() && cipher->is_ready){
            ui->button_start->setEnabled(true);
            ui->start_decipher->setEnabled(true);
        }
    } else {
        ui->button_start->setEnabled(false);
        ui->start_decipher->setEnabled(false);
    }
}

void MainWindow::on_About_clicked()
{
    QMessageBox::about(this, "About", "Программа шифрует файл алгоритмом A5/1.\nДля работы требуется выбрать файл и ввести ключ из 8 символом.\nКлюч из 64 битов преобразуется в 3 значения (19, 22 и 23 бита).\nАлгоритм шифрует побитово.\nПосле каждого шифрования бита происходит сдвиг регистра значений плученных из ключа.Программа выдает файл с расширением rpks, котрый затем можно дншифровать.\nСтудент группы 8-Т3О-303Б-16\nКирдяшкин Игорь Вячеславович");
}
