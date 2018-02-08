#include <QDebug>

#include "example.hpp"



AbstractHandler::AbstractHandler(QObject* parent) : QObject(parent) {}

AbstractHandler::~AbstractHandler() {
  qDebug() << QThread::currentThreadId() << "AbstractHandler destr!";
}


void AbstractHandler::messageReceived(Message msg) {
  doWork(msg);
}



SomeHandler1::SomeHandler1(QObject* parent) : AbstractHandler(parent) {}

void SomeHandler1::doWork(Message msg) {
  qDebug() << QThread::currentThreadId() << "SomeHandler1 received new message:" << msg.str;
  msg.str = "SomeHandler1 do message!";
  emit sendMessage(msg);
}



BlokerHandler2::BlokerHandler2(QObject* parent) : AbstractHandler(parent) {}

void BlokerHandler2::doWork(Message msg) {
  qDebug() << QThread::currentThreadId() << "BlokerHandler2 received new message:" << msg.str;
  int i = 0;
  while(i < 10) {
    QThread::sleep(1);
    ++i;
  }
  msg.str = "BlokerHandler2 do message!";
  emit sendMessage(msg);
}



Manager::Manager(QObject *parent) : QObject(parent) {}

Manager::~Manager() {}

void Manager::addHandler(AbstractHandler *handler) {
  connect(this, &Manager::sendMessage, handler, &AbstractHandler::messageReceived, Qt::QueuedConnection);
  connect(handler, &AbstractHandler::sendMessage, this, &Manager::messageReceived, Qt::QueuedConnection);
  handlers_list.append(handler);
}

void Manager::messageReceived(Message msg) {
  qDebug() << QThread::currentThreadId() << "Manager received return message:" << msg.str;
}

void Manager::run() {
  timer = new QTimer(this);
  doWork();
}

void Manager::doWork() {
  Message msg;
  msg.str = "DO IT!";
  emit sendMessage(msg);

  timer->singleShot(1000 * 5, this, &Manager::doWork);
}



App::App(QObject* parent) : QObject(parent) {}

App::~App() {
  bloker_thread->terminate();
  delete(bloker_handler2);
}

int App::main() {
  qRegisterMetaType<Message>("Message");

  manager = new Manager(this);

  some_handler1 = new SomeHandler1(this);
  manager->addHandler(some_handler1);

  bloker_thread = new QThread(this);
  bloker_handler2 = new BlokerHandler2();
  bloker_handler2->moveToThread(bloker_thread);
  bloker_thread->start();

  manager->addHandler(bloker_handler2);
  manager->run();
}



int main(int argc, char** argv) {
  std::signal(SIGINT, QCoreApplication::exit);
  std::signal(SIGHUP, QCoreApplication::exit);
  std::signal(SIGTERM, QCoreApplication::exit);
  std::signal(SIGQUIT, QCoreApplication::exit);

  QCoreApplication::setSetuidAllowed(true);
  QCoreApplication qapp(argc, argv);

  auto* my_app = new App(&qapp);

  QTimer::singleShot(0, my_app, &App::main);

  return qapp.exec();
}
