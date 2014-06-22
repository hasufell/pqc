# Hacking

## Coding style

This is more or less kernel coding style, but the light version, see
https://www.kernel.org/doc/Documentation/CodingStyle for a general idea.

Usually you are good to go if you just match the surroundings, but here
are some more details...

### Random rules:
* english only
* no CamelCase
* opening braces for functions are placed on the next line, opening braces for non-functions (such as loops) are placed on the same line
* if all conditions of an if, else if, else clause only have one line of instructions, then we omit the scope-operators {...}
* spaces around stuff like =, +, - etc, as well as keywords (like while, for etc), but not around function names
* no C99-style comments like // foo
* 80 char limit
* no trailing white spaces
* no implicit function declarations, even for static functions

### Differences to kernel coding style:
* fine to use tabs with 4 characters instead of 8 (but never whitespaces)
* newline after return type

## Comments

* comments are in doxygen format
* all functions, all data types, all macros, all typedefs... basically everything
* comments usually at declaration, not definition (structs are an exception for example)

## Good practices

* use const modifiers whenever possible, especially on function parameters
* if unsure whether to make a function static or not, make it static first
* use unsigned ints instead of signed ints whenever possible

## How to contribute

* pull request on github
* email with pull request
* email with patch
