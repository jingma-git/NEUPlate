#include "pay_by_cash_action.h"

IMPLEMENT_ACTION(pay_by_cash,CPayByCashAction)

void CPayByCashAction::run(){
    QString travel_id = req->get_string("travel_id");
    QString toll_station = req->get_string("toll_station");
    double  fee = req->get_double("fee");

    if(StatusCode::SUCCESS==DeductionController::add_deduction_record(travel_id, toll_station, fee)){
        resp->set_status_code(StatusCode::SUCCESS);
    }else{
        resp->set_status_code(StatusCode::DEDUCT_FEE_FAIL);
    }
}
