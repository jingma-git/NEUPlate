#include "reversal_deduction_by_plate_action.h"

IMPLEMENT_ACTION(reversal_deduction_record_by_plate, CReversalDeductionByPlateAction)

/**
* @name          run
* @brief         receive reversal deduction request from front-end, parse request parameters
*                and call bussiness layer handle function.
*                finally packaging result and send to front-end.
* @author        luxijia
* @date          2018-07-31
*/
void CReversalDeductionByPlateAction::run()
{
    QString plate = "";
    QString reversal_deduction = "";

    try
    {
       plate  = this->req->get_string("plate");
       reversal_deduction = this->req->get_string("reversal_deduction");
    }
    catch (NullException e)
    {
        Log::error(QString("%1%2").arg("Receive reversal deduction request parameter error. Error:").arg(e.what()));
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("invailed parameters");
        return;
    }

    if (plate.isEmpty() || reversal_deduction.isEmpty())
    {
        Log::error("Receive reversal deduction request parameter is empty.");
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("parameter is empty");
        return;
    }

    int code;

    code = DeductionController::reversal_deduction_by_plate(reversal_deduction, plate);

    if (StatusCode::SUCCESS == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction record success.");
        return;
    }
    else if ((StatusCode::INSERT_ERROR == code) || (StatusCode::SQL_EXEC_ERROR == code))
    {
        this->resp->set_status_code(StatusCode::INSERT_ERROR);
        this->resp->set_desc("reversal deduction record failed.");
        return;
    }
    else if (StatusCode::TIME_OUT_ERROR == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction record time out.");
        return;
    }
    else if ((StatusCode::REVERSAL_ERROR == code) || (StatusCode::HAS_REVERSAL == code))
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction failed.");
        return;
    }
}
