#include "query_payers_action.h"


IMPLEMENT_ACTION(query_payers,CQueryPayersAction)

void CQueryPayersAction::run()
{

    std::vector<CPayer> payers;
    QJsonArray payers_json;
    QString keyword;
    try{
    keyword=req->get_string("keyword");
    int page=req->get_int("page");
    int page_size=req->get_int("page_size");
    int all_pages;

    int status_code=Payer::query_payers(payers,all_pages,keyword,page,page_size);
    for(unsigned int i=0;i<payers.size();i++){
        QJsonValue qjv=QJsonValue(payers.at(i).toJSON());
        payers_json.append(qjv);
    }
    resp->set_status_code(status_code);
    resp->put("payers",payers_json);
    resp->put("all_pages",all_pages);
    return;
    }catch(NullException e){
        resp->set_status_code(StatusCode::ERROR_PARAMS);
        return ;
    }

}


