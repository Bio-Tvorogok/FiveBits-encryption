#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <QThread>
#include <QFile>
#include <QDataStream>
#include <QtMath>
#include <QDebug>


class Encryption : public QThread
{
    Q_OBJECT
private:
    QString Start_File;
    QString End_File;
    QString Key;
    uint64_t key_64 = 0;
    uint r1 = 0, r2 = 0, r3 = 0;

    void run();
    void register_shift();
public:
    bool is_ready = true;
    explicit Encryption(QWidget *parent = nullptr);
    ~Encryption() {}

    void setting(QString First, QString Second, QString key);

signals:
    void Set_progress(int progress);
    void End_ecryption();
};

#endif // ENCRYPTION_H
