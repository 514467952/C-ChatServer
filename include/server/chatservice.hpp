#ifndef  CHATSERVICE_H
#define  CHATSERVICE_H 

#include<muduo/net/TcpClient.h>
#include<unordered_map>
#include<functional>
#include<mutex>

#include"./redis/redis.hpp"
#include"./model/usermodel.hpp"
#include"./model/offlinemessagemodel.hpp"
#include"./model/groupmodel.hpp"
#include"./model/friendmodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

#include"../../thirdparty/json.hpp"
using json = nlohmann::json;

//c++11新语法using 代替#define
//表示MsgHandler 对应了一个回调函数
//表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr&conn,json& js,Timestamp)>;

//聊天服务器业务类
class ChatService 
{
  public:
    //获取单例对象的接口函数
    static ChatService* instance();
  public:
    //处理登录业务
    void login(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //处理注册业务
    void reg(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //处理注销业务
    void loginout(const TcpConnectionPtr&conn,json&js,Timestamp time);

  public:
    //一对一聊天业务
    void oneChat(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //添加好友业务
    void addFriend(const TcpConnectionPtr&conn,json&js,Timestamp time);

  public:
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //congredis消息队列中获取订阅消息
    void handleRedisSubscribeMessage(int,string);

  public:
    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);
    //服务器异常，业务重置方法
    void reset();

  public:
    //群组业务
    
    //创建群组
    void createGroup(const TcpConnectionPtr& conn,json&js,Timestamp time);
    //加入群组
    void addGroup(const TcpConnectionPtr&conn,json&js,Timestamp time);
    //群聊天
    void groupChat(const TcpConnectionPtr&conn,json&js,Timestamp time);
  private:

    ChatService();

    //存储消息id和其对应的业务处理方法
    unordered_map<int,MsgHandler> _msgHandlerMap;

    //存储在线用户的通信链接
    unordered_map<int,TcpConnectionPtr> _userConnMap;
    
    //定义互斥锁,保证_userConnMap的线程安全
    mutex _connMutex;

    //数据操作类对象
    //用户
    UserModel _userModel;
    //离线消息
    OfflineMsgModel _offlineMsgModel;
    //群组 
    GroupModel _groupModel;
    //好友
    FriendModel _friendModel;

    //Redis对象
    Redis _redis;
};



#endif 









