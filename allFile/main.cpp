#include "User.hpp"
#include "Meeting.hpp"
#include "Date.hpp"
#include "Storage.hpp"
#include "AgendaService.hpp"
#include "User.cpp"
#include "Meeting.cpp"
#include "Date.cpp"
#include "Storage.cpp"
#include "AgendaService.cpp"
#include <iostream>
using namespace std;
int main(){
    AgendaService a;
    a.userRegister("abc", "123456", "abc@qq.com", "12345678901");
    a.userLogIn("abc", "123456");
    a.userRegister("def", "123456", "def@qq.com", "23456789012");
    a.userLogIn("def", "123456");
    a.userRegister("ghi", "123456", "ghi@qq.com", "34567890123");
    a.userLogIn("ghi", "123456");
    a.userRegister("jkl", "123456", "jkl@qq.com", "45678901234");
    a.userLogIn("jkl", "123456");
    a.userRegister("mno", "123456", "mno@qq.com", "5678901234");
    a.userLogIn("mno", "123456");
    vector<string> par;
    par.push_back("def");
    par.push_back("ghi");
    par.push_back("jkl");
    bool success = a.createMeeting("abc", "meeting", "2018-10-10/10:10", "2019-10-10/10:10", par);
    cout << success << endl;
    return 0;
}
