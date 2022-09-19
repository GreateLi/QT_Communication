#ifndef TEST_H
#define TEST_H
#include <QObject>

class Test: public QObject{
    Q_OBJECT
public:
signals:
    void test1(const QString& str);
    void test2(const QString& str);
};

#endif // TEST_H
