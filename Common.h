#ifndef COMMON_H
#define COMMON_H
#include <QMetaEnum>
#include <QDebug>

template<typename QtEnum>
std::string QtEnumToString(QtEnum value)
{
    return QVariant::fromValue(value).toString().toStdString();
}

class ChannelEnum {
    Q_GADGET
public:
    virtual std::string getParamStr(int i) { return "";}
    virtual int getSize() { return -1; }
};
Q_DECLARE_METATYPE(ChannelEnum)


class ParamsPOX : public ChannelEnum
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
    std::string getParamStr(int i)
    {
        params param = (params)i;
        return QtEnumToString(param);
    }
    int getSize() { return sizeOfPOX; }

};
Q_DECLARE_METATYPE(ParamsPOX)




#endif // COMMON_H
