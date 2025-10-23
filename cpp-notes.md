# cpp-notes

C++ is a complex language. The C++26 standard (currently draft), which
you can consult by compiling the draft from
[here](https://github.com/cplusplus/draft), is more than 2500 pages
long (although, if we do not count the standard library part and we
focus only on the core language, we have "only" about 500
pages). Additionally, C++ has a culture of "Best Practices" and
"Performance Optimizations" which you should also (or are expected to)
know and follow, including countless books and talks to study. Don't
let me get started on all tools you need to be comfortable using:
compilers, debuggers, build systems, profilers, and so on.

This document attempts to summarize the knowledge gained through years
of practice and study, in order to provide a reference for myself and
for other programmers. It contains useful notes about the C++
programming language: tips and tricks, odd and peculiar things about
the standard, and best practices.


# Understanding C++

This section contains notes on how C++ works, explaining common
terminology and ideas.


## Value categories

To help determine how expressions should evaluate and where they can
be used, all expressions in C++ have two properties: a _type_ and a
_value category_.

The type of an expression is equivalent to the type of the value,
object, or function that results from the evaluated expression. The
value category of an expression (or subexpression) indicates whether
an expression resolves to a value, a function, or an object of some
kind.

Prior to C++11, there were only two possible value categories: lvalue
and rvalue. In C++11, three additional value categories (glvalue,
prvalue, and xvalue) were added to support a new feature called move
semantics.

```
          ┌───────────────────────────────┐
          │         value category        │
          └───────────────────────────────┘
                         │
           ┌─────────────┴───────────────┐
           │                             │
 ┌────────────────────┐         ┌────────────────────┐
 │      glvalue       │         │       rvalue       │
 │   (has identity)   │         │  (temporary value) │
 └────────────────────┘         └────────────────────┘
           │                               │
     ┌─────┴─────┐                   ┌─────┴─────┐
     │           │                   │           │
 ┌───────┐  ┌────────┐           ┌────────┐  ┌────────┐
 │lvalue │  │xvalue  │           │ xvalue │  │prvalue │
 └───────┘  └────────┘           └────────┘  └────────┘
```

- a `glvalue` ("generalized" lvalue) is an expression whose evaluation
  determines the identity of an object or function;
- a `prvalue` ("pure" rvalue) is an expression whose evaluation:

  - computes the value of an operand of a built-in operator (such
    prvalue has no result object), or
  - initializes an object (such prvalue is said to have a result
    object).

    The result object may be a variable, an object created by
    new-expression, a temporary created by temporary materialization,
    or a member thereof. Note that non-void discarded expressions have
    a result object (the materialized temporary). Also, every class
    and array prvalue has a result object except when it is the
    operand of decltype;
    
- an `xvalue` (an "eXpiring" value) is a glvalue that denotes an object
  whose resources can be reused;
- an `lvalue` is a glvalue that is not an xvalue;
- an `rvalue` is a prvalue or an xvalue; 

Here is an example:

```cpp
int makeValue() { return 42; }

int main() {
    int x = 10;              // lvalue (has a name, address)
    int y = makeValue();     // makeValue() -> prvalue (temporary)
    int&& r1 = makeValue();  // binds rvalue reference to prvalue
    int&& r2 = std::move(x); // std::move(x) -> xvalue (expiring lvalue)

    const int& ref = makeValue(); // lvalue reference bound to prvalue (lifetime extended)

    cout << x << " " << y << " " << r1 << " " << r2 << " " << ref << endl;
}
```

When a reference is initialized with an object (or function), we say
it is _bound_ to that object (or function). The process by which such a
reference is bound is called reference binding. The object (or
function) being referenced is sometimes called the referent.

```cpp
int x { 5 };
int& ref { x }; // okay: reference to int is bound to int variable
```

Non-const lvalue references can only be bound to a modifiable
lvalue. For this reason, is always best to take `const T&` as argument
in functions, unless mutability is required.


## Class declaration

Classes are user-defined types, defined by class-specifier which is
one of `class`, `struct` and `union`. The keywords class and struct
are identical except for the default member access and the default
base class access (public for structs, and private for classes). If it
is union, the declaration introduces a union type.


# Actionable advice

This section contains various things to remember when programming or
reviewing code.


## Copy elision

Copy elision is allowed to elide the copy constructor even if the copy
constructor has side effects (such as printing text to the console)!
This is why copy constructors should not have side effects other than
copying -- if the compiler elides the call to the copy constructor,
the side effects won’t execute, and the observable behavior of the
program will change.

- Reference: https://www.learncpp.com/cpp-tutorial/class-initialization-and-copy-elision/


## List initialization vs constructor

The following two statements may appear do to exactly the same thing,
but not actually:

```
Test test{1};
Test test = Test(1);
```

- The first one is named list initialization and calls a matching
  constructor. It will call the constructor with initializer list if
  provided.
- The second one creates a temporary object, then copies or moves it
  to the lvalue. Since C++17, copy elision is guaranteed.


## C++ and inheritance

Whenever you are dealing with inheritance, you should make any
explicit destructors virtual.

- Reference: https://www.learncpp.com/cpp-tutorial/virtual-destructors-virtual-assignment-and-overriding-virtualization/

If a function is virtual, all matching overrides in derived classes
are implicitly virtual.

Never call virtual functions from constructors or destructors.

- Reference: https://www.learncpp.com/cpp-tutorial/virtual-functions/


## Constructors

Best practice: Make any constructor that accepts a single argument
explicit by default. If an implicit conversion between types is both
semantically equivalent and performant, you can consider making the
constructor non-explicit.  Do not make copy or move constructors
explicit, as these do not perform conversions.

- Reference: https://www.learncpp.com/cpp-tutorial/converting-constructors-and-the-explicit-keyword/


## T* specializations

Specializations for `T*` are problematic because the lifetime of the
original object is different from the one of the class, potentially
shorter and leaving the class with a dangling pointer. You can do this
trick to disallow `T*`:

```

template <typename T>
class Storage
{
    // Make sure T isn't a pointer or a std::nullptr_t
    static_assert(!std::is_pointer_v<T> && !std::is_null_pointer_v<T>, "Storage<T*> and Storage<nullptr> disallowed");

   // ...
}
```

Alternatively, you can make a specialization for `T*` that copies the
pointer to heap memory managed by the class.
