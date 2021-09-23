#include "query_deduction_action.h"

IMPLEMENT_ACTION(query_deduction_record, CQueryDeductionAction)

/**
* @name          run
* @brief         receive query deduction request from front-end, parse request parameters
*                and call bussiness layer handle function.
*                finally packaging result and send to front-end.
* @author        luxijia
* @date          2018-07-31
*/
void CQueryDeductionAction::run()
{
    QString keyword;
    QString start_time;
    QString end_time;
    int state;
    int page_number;
    int page_item;

    try
    {
        keyword = this->req->get_string("keyword");
        start_time = this->req->get_string("start_time");
        end_time = this->req->get_string("end_time");
        state = this->req->get_int("state");
        page_number = this->req->get_int("page_number");
        page_item = this->req->get_int("page_item");
    }
    catch (NullException e)
    {
        Log::error(QString("%1%2").arg("Receive query deduction request parameter error. Error:").arg(e.what()));
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("invailed parameters");
        return;
    }

    int all_row_number;
    std::vector<CDedutionRecord> records;

    int code = DeductionController::get_deduction(records, all_row_number, keyword, start_time, end_time,
                                                  state, page_number, page_item);

    if (StatusCode::SUCCESS == code)
    {
        int size = records.size();
        QJsonArray deduction_array;

        for (int i = 0; i < size; i++)
        {
            QJsonObject deduction;

            deduction.insert("deduction_id", records[i].deduction_id());
            deduction.insert("plate", records[i].plate());
            deduction.insert("start_name", records[i].start_name());
            deduction.insert("end_name", records[i].end_name());
            deduction.insert("distance", records[i].distance());
            deduction.insert("fee", records[i].fee());
            deduction.insert("pay_method", records[i].pay_menthod());
            deduction.insert("deduction_time", records[i].deduction_time());
            deduction_array.append(deduction);
        }

        this->resp->put("all_row_number", all_row_number);
        this->resp->put("deduction_record", deduction_array);
        this->resp->set_status_code(code);
        this->resp->set_desc("query deduction success.");
        return;
    }
    else if (StatusCode::EMPTY_QUERY == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("query deduction no result.");
        return;
    }
    else if (StatusCode::QUERY_ERROR == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("query deduction failed.");
        return;
    }
}
