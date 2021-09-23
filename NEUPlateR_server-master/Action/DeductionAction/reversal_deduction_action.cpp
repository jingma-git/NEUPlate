#include "reversal_deduction_action.h"

IMPLEMENT_ACTION(reversal_deduction_record, CReversalDeductionAction)

/**
* @name          run
* @brief         receive reversal deduction request from front-end, parse request parameters
*                and call bussiness layer handle function.
*                finally packaging result and send to front-end.
* @author        luxijia
* @date          2018-07-31
*/
void CReversalDeductionAction::run()
{
    QString plate = "";
    QString start_id = "";
    QString end_id = "";
    QString reversal_deduction;

    try
    {
       plate  = this->req->get_string("plate");
       start_id = this->req->get_string("start_id");
       end_id = this->req->get_string("end_id");
       reversal_deduction = this->req->get_string("reversal_deduction");
    }
    catch (NullException e)
    {
        Log::error(QString("%1%2").arg("Receive reversal deduction request parameter error. Error:").arg(e.what()));
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("invailed parameters");
        return;
    }

    if (plate.isEmpty() || start_id.isEmpty() || end_id.isEmpty() || reversal_deduction.isEmpty())
    {
        Log::error("Receive reversal deduction request parameter is empty.");
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("parameter is empty");
        return;
    }

    QString new_deduction_id;
    int code;

    new_deduction_id = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + reversal_deduction.mid(14);
    code = DeductionController::correct_error_palte(start_id, plate);

    if (StatusCode::SUCCESS != code)
    {
        this->resp->set_status_code(StatusCode::CORRECT_PLATE_ERROR);
        this->resp->set_desc("correct error plate failed.");
        return;
    }

    code = DeductionController::correct_error_palte(end_id, plate);

    if (StatusCode::SUCCESS != code)
    {
        this->resp->set_status_code(StatusCode::CORRECT_PLATE_ERROR);
        this->resp->set_desc("correct error plate failed.");
        return;
    }

    code = DeductionController::reversal_deduction(reversal_deduction, new_deduction_id);

    if (StatusCode::SUCCESS == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction record success.");
        return;
    }
    else if (StatusCode::TIME_OUT_ERROR == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction record time out.");
        return;
    }
    else if (StatusCode::REVERSAL_ERROR == code || StatusCode::HAS_REVERSAL == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction failed.");
        return;
    }
}
