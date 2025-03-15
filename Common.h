#ifndef COMMON_H
#define COMMON_H
#include <QMetaEnum>
#include <QDebug>

class ChannelParams {
    Q_GADGET
public:
    virtual int getSize() { return -1; }
};
Q_DECLARE_METATYPE(ChannelParams)


class ParamsPOX : public ChannelParams
{
    Q_GADGET
public:
    enum params {
        SpO2,
        PR,
        sizeOfPOX
    };
    Q_ENUM(params)
public:
    explicit ParamsPOX() {}
    ParamsPOX(const ParamsPOX& value) {}
    ~ParamsPOX() {}

    int getSize() { return params::sizeOfPOX; }

};
Q_DECLARE_METATYPE(ParamsPOX)

template<typename QtEnum>
std::string QtEnumToString(QtEnum value)
{
    return QVariant::fromValue(value).toString().toStdString();
}


#endif // COMMON_H
