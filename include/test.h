#include <QObject>
#ifndef TEST_H
#define TEST_H



class Test : public QObject{
    Q_OBJECT
 private:
    int clickCount = 0;

 public:
    char* mystring = strdup("abc");
    Test();
    ~Test();
    Q_INVOKABLE void onclick();
    Q_INVOKABLE int getclick();
    Q_INVOKABLE void resetclick();
    Q_INVOKABLE QString speak();
};

#endif // TEST_H
