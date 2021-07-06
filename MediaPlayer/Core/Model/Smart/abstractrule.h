#pragma once

#include <Core/expression.h>
#include <designpattern.h>

#include <Model/media.h>

class AbstractRule
{
    Q_GADGET
public:
    enum class Op {Undefined, Superior, Inferior, Equal, Not, InferiorEqual, SuperiorEqual, Limit, And, Or, List, RegExp, Contain, Start, End};
    Q_ENUM(Op)

    virtual void set(MediaPointer) = 0;
    virtual QSharedPointer<Expression<bool>> create() = 0;
    virtual QSharedPointer<AbstractRule> clone() const = 0;
    virtual QPartialOrdering compare(QSharedPointer<AbstractRule>) const = 0;
};

