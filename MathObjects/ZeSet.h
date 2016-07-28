#ifndef ZESET_H
#define ZESET_H

#include <QObject>

class ZeFunction;
class ZeConstant;
class ZeSequence;



class ZeSet : public QObject
{
    Q_OBJECT
public:
    explicit ZeSet(QObject *parent = 0);

    ZeFunction* addFunction(QString name);
    ZeSequence* addSequence(QString name);

protected:
    QHash<QString, ZeFunction*> standardFunctions, userFunctions;
    QHash<QString, ZeSequence*> sequences;
    QHash<QString, ZeConstant*> constants;
};

#endif // ZESET_H
