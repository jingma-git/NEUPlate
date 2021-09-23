include(CarTypeDeduction/CarTypeDeduction.pri)
include(ProvinceDeduction/ProvinceDeduction.pri)

HEADERS += \
    $$PWD/deduction_controller.h \
    $$PWD/deduction_dao.h \
    $$PWD/deduction_factory.h \
    $$PWD/deduction_record.h

SOURCES += \
    $$PWD/deduction_controller.cpp \
    $$PWD/deduction_dao.cpp \
    $$PWD/deduction_factory.cpp \
    $$PWD/dudction_record.cpp
