#ifndef SLANG_CORE_MATH_INT_OPS_H
#define SLANG_CORE_MATH_INT_OPS_H

// Integer operations for compile-time evaluation

#include <cstdint>
#include <limits>
#include <type_traits>

#include "slang-string.h"

namespace Slang
{

struct MathIntType
{
    // Bit width of the integer
    //
    // Note that there are two special values:
    // - 0 -- uninitialized type
    // - 255 -- undetermined value (i.e., variable)
    uint8_t width = 0U;

    // Whether the integer type is signed
    bool isSigned = false;

    bool isValid() const
    {
        return width > 0U;
    }

    bool isValue() const
    {
        return isValid() && !isUndetermined();
    }

    bool isUndetermined() const
    {
        return width == 255U;
    }

    String toString() const;
};

struct MathIntValue
{
    // The raw bit pattern
    uint64_t rawValue;

    // The integer type parameters
    MathIntType type;

    // Whether the integer value is a bitwise value (literal or result of
    // bitwise op)
    bool isBitwise;

    template <typename T>
    void setValue(const MathIntType& _type, T _value, bool _bitwise = false)
    {
        static_assert(std::is_integral_v<T>);

        if constexpr (std::is_unsigned_v<T>)
            rawValue = _value;
        else
            rawValue = static_cast<uint64_t>(int64_t{_value});

        type = _type;

        isBitwise = _bitwise;

        normalize();
    }

    // An undetermined value means that the value is not known, typically
    // representing that the source is a variable or a non-constant expression.
    //
    // This is used for diagnosing platform-specific and undefined behavior in
    // math operations, even if not all operands values are known. (e.g.,
    // x << 65 is always platform-specific and x / 0 is always undefined.)
    void setUnderermined()
    {
        *this = MathIntValue{};
        type.width = 255U;
    }

    template <typename T>
    T getTypedValue() const
    {
        return static_cast<T>(rawValue);
    }

    uint64_t getUnsignedValue() const
    {
        return getTypedValue<uint64_t>();
    }

    int64_t getSignedValue() const
    {
        return getTypedValue<int64_t>();
    }

    // Normalize value by discarding all extra high bits and extending the sign
    // for signed types.
    void normalize();

    String toString() const;

    bool isValid() const
    {
        return type.isValid();
    }

    bool isValue() const
    {
        return type.isValue();
    }

    bool isUndetermined() const
    {
        return type.isUndetermined();
    }
};

struct MathIntResult
{
    MathIntValue value;

    enum ResultFlag : unsigned
    {
        // Result is undefined and it should not be used
        //
        // Example: 1/0  (division by zero)
        Undefined = 1U << 0,

        // Result has an overflow
        //
        // Note that shifts do not trigger an overflow when shifted bits get out
        // of range.
        Overflow = 1U << 1,

        // Set if the value is target-defined
        //
        // Example: uint32_t(1) << 32 (out-of-range shift)
        TargetDefined = 1U << 2,
    };

    unsigned flags;
};

/// @brief Integer math ops
///
/// The integer math ops are checked for undefined behavior, target-defined
/// behavior, and overflows. The intent is that anything that might result in
/// differences between compile-time and runtime behavior gets flagged, allowing
/// the compiler to diagnose things accordingly.
struct MathIntOps
{
    static MathIntResult cast(const MathIntType& resultType, const MathIntValue& a);
    static MathIntResult castToBool(const MathIntValue& a);

    static MathIntResult add(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult sub(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult mul(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult div(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult rem(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult neg(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);

    static bool compEql(const MathIntValue& a, const MathIntValue& b);
    static bool compNeq(const MathIntValue& a, const MathIntValue& b);
    static bool compGeq(const MathIntValue& a, const MathIntValue& b);
    static bool compLeq(const MathIntValue& a, const MathIntValue& b);
    static bool compGreater(const MathIntValue& a, const MathIntValue& b);
    static bool compLess(const MathIntValue& a, const MathIntValue& b);

    static MathIntResult bitwiseAnd(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult bitwiseOr(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult bitwiseXor(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult bitwiseNot(const MathIntType& resultType, const MathIntValue& a);

    static MathIntResult lsh(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
    static MathIntResult rsh(const MathIntType& resultType, const MathIntValue& a, const MathIntValue& b);
};

} // namespace Slang

#endif
