#include "valid_reversal_deduction_action.h"

IMPLEMENT_ACTION(vailed_reversal_deduction_record, CVailedReversalDeductionAction)

/**
* @name          run
* @brief         receive reversal deduction whether vailed request from front-end, parse request parameters
*                and call bussiness layer handle function.
*                finally packaging result and send to front-end.
* @author        luxijia
* @date          2018-07-31
*/
void CVailedReversalDeductionAction::run()
{
    QString reversal_deduction = "";

    try
    {
        reversal_deduction = this->req->get_string("reversal_deduction");
    }
    catch (NullException e)
    {
        Log::error(QString("%1%2").arg("Receive reversal deduction request parameter error. Error:").arg(e.what()));
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("invailed parameters");
        return;
    }

    if (reversal_deduction.isEmpty())
    {
        Log::error("Receive reversal deduction request parameter is empty.");
        this->resp->set_status_code(StatusCode::ERROR_PARAMS);
        this->resp->set_desc("parameter is empty");
        return;
    }

    QString drive_id;
    int code = DeductionController::is_valid_reversal_deduction(drive_id, reversal_deduction);

    if (StatusCode::INVAILD_REVERSAL == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("invalid reversal deduction record.");
        return;
    }
    else if (StatusCode::TIME_OUT_ERROR == code)
    {
        this->resp->set_status_code(code);
        this->resp->set_desc("reversal deduction time out.");
        return;
    }

    QString start_id = TravelRecordController::get_start_toll(drive_id);
    QString end_id = TravelRecordController::get_end_toll(drive_id);

    if (start_id.isEmpty() || end_id.isEmpty())
    {
        this->resp->set_status_code(StatusCode::INVAILD_REVERSAL);
        this->resp->set_desc("Get drive record toll station information failed.");
        return;
    }

    QString start_photo = RecogRecordController::queryPhotoPath(start_id);//调接口
    QString end_photo = RecogRecordController::queryPhotoPath(end_id);

    if (start_photo.isEmpty() || end_photo.isEmpty())
    {
        this->resp->set_status_code(StatusCode::INVAILD_REVERSAL);
        this->resp->set_desc("Get recognition photo failed.");
        return;
    }

    this->resp->set_status_code(StatusCode::SUCCESS);
    this->resp->set_desc("reversal deduction record is valid.");
    this->resp->put("start_id", start_id);
    this->resp->put("end_id", end_id);
    this->resp->put("start_photo", PhotoTool::to_base64(PhotoTool::load_photo(start_photo)));
    this->resp->put("end_photo", PhotoTool::to_base64(PhotoTool::load_photo(end_photo)));
    this->resp->put("deduction_id", reversal_deduction);
    return;
}
