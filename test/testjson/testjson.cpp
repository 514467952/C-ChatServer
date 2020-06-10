
#include"json.hpp"
using  nlohmann::json;

#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;

//json序列化示例1
string func()
{
  json js;
  js["msg_type"] = 2;
  js["from"] = "zhang san";
  js["to"] = "li si";
  string sendBuf = js.dump(); //序列化后的字符串
  //cout<<sendBuf.c_str()<<endl;
  return sendBuf;
}

void func2()
{
  json js;
  js["msg"]["zhang san"] = "helloworld";
  js["msg"]["liu shuo"] = "helo china";
  cout<<js<<endl;
}

//json序列化容器
void fun3()
{
  json js;
  // 直接序列化一个vector容器
   vector<int> vec;
   vec.push_back(1);
   vec.push_back(2);
   vec.push_back(5);
   js["list"] = vec;
   // 直接序列化一个map容器
   map<int, string> m;
   m.insert({1, "黄山"});
   m.insert({2, "华山"});
   m.insert({3, "泰山"});
   js["path"] = m;
  
    string sendBuf = js.dump();
   cout<<sendBuf<<endl;
}
int main()
{
  string recvBuf = func();
  //数据的反序列化
  json jsbuf= json::parse(recvBuf);
  cout<<jsbuf["msg_type"]<<endl;
  cout<<jsbuf["from"]<<endl;
  cout<<jsbuf["to"]<<endl;
  return 0;
}




