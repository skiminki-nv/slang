# Enumerations

An enumeration is a type that has any number of named constants. An enumeration has an underlying type, which
specifies the storage type and the type for its named constants.

The underlying type of an enumeration is a [Boolean](types-fundamental.md) or an
[integer](types-fundamental.md). If not specified, the default type is `int`.

If the enumeration is *scoped*, the named constants are defined within the enumeration name space scope.
Hence, the constants are referred by using the `EnumType.ENUM_CONST` form. If the enumeration is *unscoped*,
the named constants are defined in the same name space as the enumeration type.

## Syntax {#syntax}

Enumeration declaration:
> *`enum-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[*`modifier-list`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'enum'`** [**`'class'`**] [*`enum-identifier`*] <br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`tag-type`*]<br>
> **`'{'`** [*`enum-case-decl`* (**`','`** *`enum-case-decl`*)* ] **`'}'`**

Enumeration case declaration:
> *`enum-case-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`enum-const-identifier`* [**`'='`** *`expr`*]

> 📝 **Remark:** The parser currently accepts generic parameters for an enumeration declaration,
> but the parameters are not usable. See GitHub issue
> [#10078](https://github.com/shader-slang/slang/issues/10078) for details.

### Parameters {#parameters}

- *`modifier-list`* is an optional list of modifiers. (TODO: link)
- **`'class'`** specifies that the type is a scoped enumeration.
- *`enum-identifier`* is the identifier for the declared enumeration type.
- *`tag-type`* specifies the underlying type for the enumeration.
- *`enum-const-identifier`* is an identifier for an enumerated constant.
- *`expr`* is a [link-time constant](expressions-evaluation-classes.md), specifying the
  concrete value for the enumeration.

### Description {#description}



## Examples

TODO
