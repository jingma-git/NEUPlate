#ifndef STATUS_CODE_H
#define STATUS_CODE_H

namespace StatusCode {
static const int SUCCESS = 0;
static const int QUERY_ERROR = 1;
static const int UPDATE_ERROR = 2;
static const int INSERT_ERROR = 3;
static const int DELETE_ERROR = 4;
static const int SQL_EXEC_ERROR = 5;
static const int EMPTY_QUERY = 6;
static const int NO_ACTION = 7;
static const int ERROR_PARAMS = 8;
static const int SYSTEM_ERROR = 9;
static const int INVAILD_ACCESS = 100;
static const int SYSTEM_BUSY = 101;

static const int ADD_ERROR = 8;
//payer_info
static const int BALANCE_NEGATIVE = 91;
static const int BALANCE_OVERFLOW = 93;
static const int LESS_THEN_ZERO = 92;
static const int PAYER_NOT_FOUND = 94;

//deduction
static const int FEE_NO_ENOUGH = 30;
static const int REVERSAL_ERROR = 31;
static const int TIME_OUT_ERROR = 32;
static const int HAS_REVERSAL = 33;
static const int INVAILD_REVERSAL = 34;
static const int CORRECT_PLATE_ERROR = 35;

// employee
static const int RIGHT_ANSWER = 50;
static const int WRONG_ANSWER = 51;
static const int STATE_ERROR = 52;
static const int NO_SUCH_USER = 53;
static const int NEW_EMPLOYEE = 54;
static const int FROZEN = 55;
static const int RESIGNATION = 56;
static const int INVAILD_PASSWD = 57;
static const int NO_NEW_EMPLOYEE = 58;
static const int LOGINED = 59;

//EnterExitStation
static const int PASS = 70; //放行
static const int NOT_PASS = 71; //不放行


static const int IN_BLACKLIST = 61;
static const int IN_WANTED = 62;
static const int DUPLICATE_PLATE = 63;
static const int RECOG_ERROR= 64;
static const int NOT_ENTER_HIGHWAY = 65;
static const int DEDUCT_FEE_FAIL = 66;
}
#endif // STATUS_CODE_H
