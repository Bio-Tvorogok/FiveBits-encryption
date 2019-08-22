#include "encryption.h"



Encryption::Encryption(QWidget *parent) : QThread ()
{

}

void Encryption::run(){

    QFile filein(Start_File);
    QFile fileout(End_File);
    if(filein.open(QIODevice::ReadOnly) && fileout.open(QIODevice::WriteOnly)){
        QDataStream in(&filein);
        QDataStream out(&fileout);
        uint8_t new_bits;
        int status = 0;
        uint64_t data;
        uint8_t  out_bits = 0;

        r1 = 52487 << 45;
        r1 |= key_64;
        r1 = r1 >> 45;

        r2 = 4194303 << 23;
        r2 |= key_64;
        r2 = r2 >> 23;

        r3 = 8388607 | key_64;

        for (qint64 var = 0; var < filein.size(); var++) {

            if (status != floor(((double)var/(double)filein.size())* 100)){
                status = floor(((double)var/(double)filein.size())* 100);
                emit Set_progress(status);
            }
            in >> new_bits;
            for (int i = 0; i < 8; i++){
                register_shift();
                out_bits |= (((new_bits & (1 << (7 - i))) >> (7 - i)) ^ ((r1 & (1 << 18)) >> 18) ^ ((r2 & (1 << 21)) >> 21) ^
                             ((r3 & (1 << 22)) >> 22)) << (7 - i);
                qDebug() << "r1: " << r1 << " r2: " << r2 << " r3: " << r3;

            }

            out << out_bits;
            out_bits = 0;
        }
        filein.close();
        fileout.close();
        emit Set_progress(100);
        emit End_ecryption();
    }

}

void Encryption::register_shift()
{
    uint8_t rx = ((1 << 8) & r1) >> 8;
    uint8_t ry = ((1 << 10) & r2) >> 10;
    uint8_t rz = ((1 << 10) & r3) >> 10;

    uint8_t f = rx & ry | rx & rz | ry & rz;

    if (rx == f){
        r1 = r1 << 1;
        r1 |= (((1 << 18) & r1) >> 18) ^
                (((1 << 17) & r1) >> 17) ^
                (((1 << 16) & r1) >> 16) ^
                (((1 << 13) & r1) >> 13);

    }
    if (ry == f) {
        r2 = r2 << 1;
        r2 |= (((1 << 21) & r2) >> 21) ^
                (((1 << 20) & r2) >> 20);
    }
    if (rz == f) {
        r3 = r3 << 1;
        r3 |= (((1 << 22) & r3) >> 22) ^
                (((1 << 21) & r3) >> 21) ^
                (((1 << 20) & r3) >> 20) ^
                (((1 << 7) & r3) >> 7);
    }
}


void Encryption::setting(QString First, QString Second, QString key){
    Start_File = First;
    End_File = Second;
    Key = key;

    for (int i = 0; i < 8; i++){
        key_64 = key_64 << 8;
        key_64 |= ushort(Key[i].toLatin1());
        qDebug() << key_64;
    }
}
