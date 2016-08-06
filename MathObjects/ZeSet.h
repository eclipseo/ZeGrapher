#ifndef ZESET_H
#define ZESET_H

#include <QObject>

class ZeFunction;
class ZeConstant;
class ZeSequence;

enum struct AddResult
{
    OK, INVALID_NAME, NAME_ALREADY_TAKEN
};

class ZeSet : public QObject
{
    Q_OBJECT
public:
    explicit ZeSet(QObject *parent = 0);
    ZeSet(const ZeSet &set);

    AddResult addFunction(QString name, QStringList vars = QString("x"));
    AddResult addSequence(QString name, QString var = "n");

    QStringList antiderivativeNames();
    QStringList derivativeNames();
    QStringList functionNames();
    QStringList sequenceNames();
    QStringList constantNames();

protected:
    QHash<QString, ZeFunction*> functions;
    QHash<QString, ZeSequence*> sequences;
    QHash<QString, ZeConstant*> constants;

    QStringList antiderivatives, derivatives;
};

#endif // ZESET_H
