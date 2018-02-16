#ifndef QT_EXAMPLE_HPP
#define QT_EXAMPLE_HPP

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include <QTimer>


class Task {
public:
    QString str;
};

class DataManager : public QObject {
Q_OBJECT

public:
    explicit DataManager(QObject* parent = Q_NULLPTR);
    ~DataManager();

public:
    QList<Task> doTask();
};


class MyApp : public QObject {
Q_OBJECT

public:
    explicit MyApp(QObject* parent = Q_NULLPTR);
    ~MyApp();

public slots:
    int main();
    void printResult();
    void tick();

private:
    void startFuture();

private:
    QFuture<QList<Task>> future;
    QFutureWatcher<QList<Task>>* watcher;
    DataManager* data_manager;
};

#endif //QT_EXAMPLE_HPP
