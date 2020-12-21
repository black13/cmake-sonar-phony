
#include <QObject>
#include <QByteArray>
class Message : public QObject
{
    Q_OBJECT

public:
    Message();

    int value() ;

public slots:
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};