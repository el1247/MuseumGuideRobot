#include "test.h"

Test::~Test(){

}


Test::Test(){
    clickCount = 0;
}


void Test::onclick(){
    clickCount++;
}


int Test::getclick(){
    return clickCount;
}


void Test::resetclick(){
    clickCount = 0;
}

//char* Test::distext(){
//    return mystring;
//}

QString Test::speak(){
    QString wordy;
    wordy = mystring;
    return wordy;
}
