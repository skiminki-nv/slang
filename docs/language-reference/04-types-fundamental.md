Fundamental types
=================

The following types are collectively called the _fundamental types_:
- The `void` type
- The scalar integer types
- The scalar floating point types


Void Type
---------

The type `void` contains no data and has a single unnamed value.

A function with return type `void` does not return a value.

Variables, arrays, or references of type `void` cannot be declared. Structures may not inherit from parent type `void`.


Scalar Types
------------

### Boolean Type

Type `bool` is used to represent Boolean truth values: `true` and `false`.

The size of `bool` is target-defined. Similarly, the underlying bit patterns for `true` and `false` are target-defined. The use of `bool` should be avoided when a specific in-memory layout of a data structure is required. This includes data shared between different targets.


### Integer Types

The following integer types are defined:

| Name                 | Description             |
|----------------------|-------------------------|
| `int8_t`             | 8-bit signed integer    |
| `int16_t`            | 16-bit signed integer   |
| `int` or `int32_t`   | 32-bit signed integer   |
| `int64_t`            | 64-bit signed integer   |
| `uint8_t`            | 8-bit unsigned integer  |
| `uint16_t`           | 16-bit unsigned integer |
| `uint` or `uint32_t` | 32-bit unsigned integer |
| `uint64_t`           | 64-bit unsigned integer |

All arithmetic operations on signed and unsigned integers wrap on overflow.

All target platforms support the `int`/`int32_t` and `uint`/`uint32_t` types. The support for other types depends on the target and target capabilities. See [target platforms](../target-compatibility.md) for details.

All integer types are stored in memory with their natural size and alignment on all targets that support them.

### Floating-Point Types

The following floating-point type are defined:

| Name                    | Description                             | Precision (sign/exponent/significand bits) |
|-------------------------|-----------------------------------------|--------------------------------------------|
| `half` or `float16_t`   | 16-bit floating-point number (IEEE 754) | 1/5/10                                     |
| `float` or `float32_t`  | 32-bit floating-point number (IEEE 754) | 1/8/23                                     |
| `double` or `float64_t` | 64-bit floating-point number (IEEE 754) | 1/11/52                                    |

Rules for rounding, denormals, infinite values, and not-a-number (NaN) values are target-defined.

All targets support the `float` type. Support for other types is target-defined. See [targets platforms](../target-compatibility.md) for details.

All floating-point types are stored in memory with their natural size and alignment on all targets that support them.


Alignment and data layout
-------------------------
All fundamental types are _naturally aligned_. That is, their alignment is the same as their size.

All signed integers use two's complement little-endian representation.

