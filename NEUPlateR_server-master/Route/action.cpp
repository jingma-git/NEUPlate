#include "action.h"

/**
* @functionName  create
* @Description   create subAction by action_name, get the create function from create_table
* @author        chenhanlin
* @date          2018-07-19
* @parameter     const QString &action_name
* @return        Action*
*/
CAction* CAction::create(const QString &action_name, std::shared_ptr<CRequest> req)
{
    auto &s_create_table = create_table();
    auto iter = s_create_table.find(action_name);
    if(s_create_table.end() != iter){
        return (iter->second)(req);
    }else{
        return nullptr;
    }
}

CAction::CAction(std::shared_ptr<CRequest> req): req(req), resp(new CResponse)
{

}

CAction::~CAction()
{
}

std::shared_ptr<CResponse> CAction::response()
{
    return resp;
}

/**
* @functionName  register_action
* @Description   add key and value into create_table
* @author        chenhanlin
* @date          2018-07-19
* @parameter     const QString &action_name, Action::p_create_func func
* @return        void
*/
void CAction::register_action(const QString &action_name, CAction::p_create_func func)
{
    create_table().insert(std::map<QString, p_create_func>::value_type(action_name, func));
}

/**
* @functionName  create_table
* @Description   get the create function table
* @author        chenhanlin
* @date          2018-07-19
* @parameter     void
* @return        void
*/
std::map<QString, CAction::p_create_func> &CAction::create_table()
{
    static std::map<QString, p_create_func> s_create_table;
    return s_create_table;
}

void CAction::set_socket_id(const QString &socket_id)
{
    m_socket_id = socket_id;
}
