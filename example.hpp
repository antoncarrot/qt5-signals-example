#ifndef QT_SIGNALS_EXAMPLE_HPP
#define QT_SIGNALS_EXAMPLE_HPP

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QList>

#include <csignal>



class Message {
public:
    QString str;
};



class AbstractHandler : public QObject {
Q_OBJECT

public:
    explicit AbstractHandler(QObject* parent = Q_NULLPTR);
    ~AbstractHandler();

public slots:
    void messageReceived(Message msg);

signals:
    void sendMessage(Message msg);

protected:
    virtual void doWork(Message msg) = 0;
};



class SomeHandler1 : public AbstractHandler {
public:
    explicit SomeHandler1(QObject* parent = Q_NULLPTR);

private:
    void doWork(Message msg);
};



class BlokerHandler2 : public AbstractHandler {
public:
    explicit BlokerHandler2(QObject* parent = Q_NULLPTR);

private:
    void doWork(Message msg);
};



class Manager : public QObject {
Q_OBJECT

public:
    explicit Manager(QObject* parent = Q_NULLPTR);
    ~Manager();

public:
    void addHandler(AbstractHandler* handler);
    void run();

public slots:
    void messageReceived(Message msg);

signals:
    void sendMessage(Message msg);

private:
    void doWork();

private:
    QTimer* timer;
    QList<AbstractHandler*> handlers_list;
};



class App : public QObject {
Q_OBJECT

public:
    explicit App(QObject* parent = Q_NULLPTR);
    ~App();

public slots:
    int main();

private:
    Manager* manager;
    SomeHandler1* some_handler1;
    BlokerHandler2* bloker_handler2;
    QThread* bloker_thread;
};

#endif //QT_SIGNALS_EXAMPLE_HPP
