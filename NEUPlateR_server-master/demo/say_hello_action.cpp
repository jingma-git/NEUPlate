#include "say_hello_action.h"
#include <QDebug>

IMPLEMENT_ACTION(sayHello, SayHelloAction)

void SayHelloAction::run()
{
    QString name(req->get_string("name"));
    qDebug() << "Hello, " << name;
    resp->put("say", "Hello");
}
