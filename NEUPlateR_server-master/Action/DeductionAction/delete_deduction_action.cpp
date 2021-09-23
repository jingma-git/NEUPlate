#include "delete_deduction_action.h"

IMPLEMENT_ACTION(delete_deduction_record, CDeleteDeductionAction)

/**
* @name          run
* @brief         receive delete deduction request from front-end, parse request parameters
*                and call bussiness layer handle function.
*                finally packaging result and send to front-end.
* @author        luxijia
* @date          2018-07-31
*/
void CDeleteDeductionAction::run()
{
    QJsonArray delete_deduction_array;

    try
    {
        delete_deduction_array = req->get_array("delete_deduction");
    }
    catch (NullException e)
    {
        Log::error(QString("%1%2").arg("Receive delete deduction request parameter error. Error:").arg(e.what()));
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("invailed parameters");
        return;
    }

    QString deduction_id  = "";

    if (!delete_deduction_array.isEmpty())
    {
        int size = delete_deduction_array.size();

        for (int i = 0; i < size; i++)
        {
            QJsonValue value = delete_deduction_array.at(i);

            if (value.isString())
                deduction_id += value.toString() + ",";
        }

        deduction_id.chop(1);
    }

    if (deduction_id.isEmpty())
        return;

    int code = DeductionController::delete_deduction(deduction_id);

    if (StatusCode::SUCCESS == code)
    {
        this->resp->set_status_code(StatusCode::SUCCESS);
        this->resp->set_desc("delete deduction success.");
        return;
    }
    else if (StatusCode::DELETE_ERROR == code)
    {
        this->resp->set_status_code(StatusCode::DELETE_ERROR);
        this->resp->set_desc("delete deduction failed.");
        return;
    }
}
