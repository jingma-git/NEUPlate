#ifndef CQueryCarByPayerAction_H
#define CQueryCarByPayerAction_H
#include "Route/action.h"
#include "Business/Car/car_controller.h"
#include <QJsonArray>
class CQueryCarsByPayerAction : public CAction
{
public:
    DECLEAR_ACTION(CQueryCarsByPayerAction)
    void run();
};

#endif // CQueryCarByPayerAction_H
