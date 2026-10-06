#include "slang-math-int-ops.h"

#include <bit>

namespace Slang
{

namespace
{

bool isNegative(const MathIntValue& v)
{
    return v.type.isSigned && (v.getSignedValue() < 0);
}

unsigned getMinimumBitWidth(const MathIntValue& v)
{
    if (isNegative(v))
    {
        return std::bit_width(~v.getUnsignedValue()) + 1U;
    }
    else
    {
        return std::bit_width(v.getUnsignedValue());
    }
}

}

void MathIntValue::normalize()
{
    if (type.width == 0)
    {
        rawValue = 0;
    }
    else if (type.width <= 63)
    {
        const unsigned shift = 64 - type.width;
        rawValue <<= shift;

        if (type.isSigned)
            rawValue = static_cast<uint64_t>(static_cast<int64_t>(rawValue) >> shift);
        else
            rawValue >>= shift;
    }
    else if (type.width == 255)
    {
        rawValue = 0;
    }
}

String MathIntValue::toString() const
{
    if (!isValid())
        return "(unset)";

    if (isUndetermined())
        return "(expr)";

    if (isBitwise)
    {
        MathIntValue val{*this};

        // drop extended sign bits for printing
        val.type.isSigned = false;
        val.normalize();

        StringBuilder sb;
        sb.append("0x");
        sb.append(val.getUnsignedValue(), 16);
        if (!type.isSigned)
            sb.append('U');

        return sb.toString();
    }
    else
    {
        if (type.isSigned)
            return String(getSignedValue());
        else
            return String(getUnsignedValue());
    }
}

MathIntResult MathIntOps::cast(const MathIntType& resultType, const MathIntValue& a)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.rawValue = a.rawValue;
    res.value.normalize();

    if (compNeq(res.value, a))
    {
        if (!a.isBitwise)
        {
            res.flags |= MathIntResult::Overflow;
        }
        else
        {
            MathIntValue tmp = a;
            tmp.type.isSigned = true;
            tmp.normalize();

            if (getMinimumBitWidth(tmp) > res.value.type.width)
                res.flags |= MathIntResult::Overflow;
        }
    }

    return res;
}

MathIntResult MathIntOps::castToBool(const MathIntValue& a)
{
    MathIntResult res{};
    res.value.type = MathIntType{.width = 1, .isSigned = false};
    res.value.rawValue = a.rawValue != 0 ? 1 : 0;
    res.value.normalize();
    return res;
}

bool MathIntOps::compNeq(const MathIntValue& a, const MathIntValue& b)
{
    // both values negative?
    if (isNegative(a) && isNegative(b))
        return a.getSignedValue() != b.getSignedValue();

    // both values non-negative?
    if (!isNegative(a) && !isNegative(b))
        return a.getUnsignedValue() != b.getUnsignedValue();

    // one is negative, one is non-negative
    return true;
}

MathIntResult MathIntOps::bitwiseAnd(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.rawValue = a.rawValue & b.rawValue;
    res.value.normalize();
    res.value.isBitwise = true;

    return res;
}

MathIntResult MathIntOps::bitwiseOr(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.rawValue = a.rawValue | b.rawValue;
    res.value.normalize();
    res.value.isBitwise = true;

    return res;
}

MathIntResult MathIntOps::bitwiseXor(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.rawValue = a.rawValue ^ b.rawValue;
    res.value.normalize();
    res.value.isBitwise = true;

    return res;
}

MathIntResult MathIntOps::bitwiseNot(const MathIntType& resultType, const MathIntValue& a)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.rawValue = ~a.rawValue;
    res.value.normalize();
    res.value.isBitwise = true;

    return res;
}

MathIntResult MathIntOps::lsh(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.isBitwise = true;

    const uint64_t shiftAmount = b.getUnsignedValue();

    if (shiftAmount < resultType.width)
    {
        res.value.rawValue = a.getUnsignedValue() << shiftAmount;
    }
    else
    {
        res.value.rawValue = 0U;
        res.flags |= MathIntResult::TargetDefined;
    }

    res.value.normalize();

    return res;
}

MathIntResult MathIntOps::rsh(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b)
{
    MathIntResult res{};
    res.value.type = resultType;
    res.value.isBitwise = true;

    const uint64_t shiftAmount = b.getUnsignedValue();

    if (shiftAmount < resultType.width)
    {
        if (resultType.isSigned)
            res.value.rawValue = static_cast<uint64_t>(a.getSignedValue() >> shiftAmount);
        else
            res.value.rawValue = a.getUnsignedValue() >> shiftAmount;
    }
    else
    {
        if (isNegative(a) && resultType.isSigned)
            res.value.rawValue = static_cast<uint64_t>(int64_t{-1});
        else
            res.value.rawValue = 0U;

        res.flags |= MathIntResult::TargetDefined;
    }

    res.value.normalize();

    return res;
}

} // namespace Slang
