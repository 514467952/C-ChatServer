#include"../../include/server/chatserver.hpp"
#include"../../thirdparty/json.hpp"
#include"../../include/server/chatservice.hpp"

#include<functional>
#include<string>
using namespace std;
using namespace placeholders;
using json =  nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
    const InetAddress& listenAddr,
    const string& nameArg)
    :_server(loop,listenAddr,nameArg)
     ,_loop(loop)
    
{
  //注册链接回调
  _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));

  //注册消息回调
  _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

  //设置线程数量
  _server.setThreadNum(4);
}


void ChatServer::start()
{
  _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
  //客户端断开链接
  if(!conn->connected())
  {
    ChatService::instance()->clientCloseException(conn);
    conn->shutdown();
  }

}

void ChatServer::onMessage(const TcpConnectionPtr&conn,
                            Buffer *buffer,
                            Timestamp time)
{
  string buf = buffer->retrieveAllAsString();//把缓冲区的数据拿到

  //数据的反序列化
  json js = json::parse(buf);

  //达到的目的:完全解耦网络模块的代码和业务模块的代码
  //通过js["msgid"]获取业务处理器handler=>conn js time 
  //get转换成整型
  //获取对应的msgid对应的处理器
  auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
  
  //回调消息绑定号的事件处理器，来执行相应的业务处理
  msgHandler(conn,js,time);
}



