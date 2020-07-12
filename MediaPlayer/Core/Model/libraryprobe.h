#ifndef LIBRARYPROBE_H
#define LIBRARYPROBE_H

#include <QDebug>
#include <QCryptographicHash>
#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSet>
#include <QTime>
#include <QThread>
#include <QElapsedTimer>

class LibraryProbe: public QThread
{
    Q_OBJECT

private:
    int m_counter;
    QFileInfoList m_all;
    QDateTime m_lastProbed;
    QFile stat;
protected:
    void run();

public:

    QStringList baseName;
    LibraryProbe();
    LibraryProbe(const LibraryProbe&);
    ~LibraryProbe();
    
    QStringList filter;

    void explore(QString = "./");
    QByteArray getMd5(QFileInfo);

    QDateTime lastProbed() const;
    void setLastProbed(QDateTime lastProbed);
    
    bool hasProbed() const;
    
signals:
    void s_add(QByteArray, QString);
    
public slots:
    void onEnd();
};

#endif // LIBRARYPROBE_H
