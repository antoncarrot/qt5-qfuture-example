#include <QDebug>
#include <csignal>

#include "example.hpp"


DataManager::DataManager(QObject* parent) : QObject(parent) {}

DataManager::~DataManager() {}

QList<Task> DataManager::doTask() {
  qDebug() << QThread::currentThreadId() << "!Start blocker task";
  QThread::sleep(5);
  qDebug() << QThread::currentThreadId() << "!Finish blocker task";

  QList<Task> tasks_list;
  Task task;
  task.str = "result task 1";
  tasks_list.append(task);

  Task task2;
  task2.str = "result task 2";
  tasks_list.append(task2);
  return tasks_list;
}


MyApp::MyApp(QObject* parent) : QObject(parent) {}

MyApp::~MyApp() {
}

int MyApp::main() {
  qDebug() << QThread::currentThreadId() << "Start main";

  data_manager = new DataManager(this);
  watcher = new QFutureWatcher<QList<Task>>(this);

  connect(watcher, &QFutureWatcherBase::finished, this, &MyApp::printResult);
  //connect(watcher, SIGNAL(finished()), this, SLOT(printResult()));

  //future = QtConcurrent::run(data_manager, &DataManager::doTask);
  //watcher->setFuture(future);
  startFuture();

  qDebug() << QThread::currentThreadId() << "Finish main";
  QTimer::singleShot(1000, this, &MyApp::tick);
}

void MyApp::printResult() {
  qDebug() << QThread::currentThreadId() << "Print blocker results:";

  QList<Task> tasks_list = watcher->result();
  for(int i = 0; i < tasks_list.size(); ++i) {
    qDebug() << QThread::currentThreadId() << tasks_list.at(i).str;
  }

  qDebug() << QThread::currentThreadId() << "Start future after 3 sec";
  QTimer::singleShot(3000, this, &MyApp::startFuture);
}

void MyApp::tick() {
  qDebug() << QThread::currentThreadId() << "Event tick";
  QTimer::singleShot(1000, this, &MyApp::tick);
}

void MyApp::startFuture() {
  future = QtConcurrent::run(data_manager, &DataManager::doTask);
  watcher->setFuture(future);
}


int main(int argc, char** argv) {
  std::signal(SIGINT, QCoreApplication::exit);
  std::signal(SIGHUP, QCoreApplication::exit);
  std::signal(SIGTERM, QCoreApplication::exit);
  std::signal(SIGQUIT, QCoreApplication::exit);

  QCoreApplication::setSetuidAllowed(true);
  QCoreApplication qapp(argc, argv);

  auto* my_app = new MyApp(&qapp);

  QTimer::singleShot(0, my_app, &MyApp::main);

  return qapp.exec();
}
