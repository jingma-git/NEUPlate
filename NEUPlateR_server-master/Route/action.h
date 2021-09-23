#ifndef ACTION_H
#define ACTION_H

#include <QString>
#include <QObject>
#include <map>
#include <memory>
#include "Network/request.h"
#include "Network/response.h"

// base class
class CAction
{
public:
    static CAction* create(const QString &action_name, std::shared_ptr<CRequest> req);
//    virtual void do_action(Request &req, Response &resp) = 0;
    CAction(std::shared_ptr<CRequest> req);
    virtual ~CAction();
    virtual void run() = 0;
    std::shared_ptr<CResponse> response();
    void set_socket_id(const QString &socket_id);

protected:
    typedef CAction *(*p_create_func)(std::shared_ptr<CRequest> req); // create function pointer
    static void register_action(const QString &action_name, p_create_func func); // bind string with create function
protected:
    std::shared_ptr<CRequest> req;
    std::shared_ptr<CResponse> resp;
    QString m_socket_id;
private:
    static std::map<QString, p_create_func> &create_table();
signals:
    void notify_send(CResponse);
};

// sub class define
#define DECLEAR_ACTION(class_name)\
public:\
    class_name(std::shared_ptr<CRequest> req);\
    static CAction* create(std::shared_ptr<CRequest> req);\
private:\
    struct isRegister\
    {\
        isRegister();\
    };\
private:\
    static struct isRegister registered;

#define IMPLEMENT_ACTION(action_name, class_name)\
class_name::isRegister class_name::registered;\
class_name::class_name(std::shared_ptr<CRequest> req):CAction(req){}\
CAction* class_name::create(std::shared_ptr<CRequest> req)\
{\
    return new class_name(req);\
}\
class_name::isRegister::isRegister()\
{\
    static bool is_registered = false;\
    if(!is_registered){\
        CAction::register_action(#action_name, class_name::create);\
        is_registered = true;\
    }\
}

#endif // ACTION_H
