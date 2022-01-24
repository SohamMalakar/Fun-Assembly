<h1 align="center">
  <br>
    <img src="resources/logo.png" alt="logo" width="250">
  <br><br>
  Fun Assembly Programming Language
  <br>
  <br>
</h1>

## About

It's a simple token-based programming language written in C++. <br>
Authors- [Soham Malakar](https://www.linkedin.com/in/soham-malakar-108533207) and [Sanket Tarafder](https://www.linkedin.com/in/sanket-tarafder-bbb33117b).

---

## Know About The Authors

[**Soham Malakar**](https://www.linkedin.com/in/soham-malakar-108533207) is a student pursuing Bachelor of Science (Hons.) in the Computer Science Department of Scottish Church College, Kolkata. He is very passionate about computers and innovation.

[**Sanket Tarafder**](https://www.linkedin.com/in/sanket-tarafder-bbb33117b) is a student pursuing Bachelor of Technology in the Information Technology Department of Kalyani Government Engineering College, Nadia. He is a computer and data enthusiast.

---

## Overview

It is a Procedure Oriented Programming Language which uses a simple expression evaluator. It stores the variables and elements in a hash table. As the instructions set for this language is very small and simple, it does not come with any loop structures or functions. These functionalities can be obtained using jumps.

---

## Disadvantages

- Same variable name can't be used anywhere.
- It doesn't support structures and file handling.

---

## Installation

There are few pre-compiled binaries available in the [release](https://github.com/SohamMalakar/Fun-Assembly/releases/tag/v1.1.0) section.
If you don't find any suitable version for your system, please compile it manually.

> **_NOTE:_** Make sure you have the latest g++ compiler installed in your system.

### Windows

A batch script is provided for compiling the program in the repository.

1. Compile the file by runnning the batch script `build.bat`

2. Now run the code using `fasml.exe`

```
> fasml.exe [FILENAME]
```

Here [FILENAME] refers to the `.fasml` file you want to compile.

### Linux/Unix/MacOS/Windows WSL

The Makefile for this compiler is provided in the repository.

1. Compile the file using `make`

```
$ make
```

2. Now run the code using `fasml`

```
$ ./fasml [FILENAME]
```

Here [FILENAME] refers to the `.fasml` file you want to compile.

---

## Extension

We have also provided the syntax highlighter for this programming language for Visual Studio Code.

| Andromeda                                  | Monokai-Pro                                |
| ------------------------------------------ | ------------------------------------------ |
| ![andromeda](resources/code-andromeda.png) | ![monokai](resources/code-monokai-pro.png) |

You can install the extension from [Marketplace](https://marketplace.visualstudio.com/items?itemName=skt09.fun-assembly-syntax).

---

## Keywords

There are total 20 reserved Keywords and 8 Logical Operators present in this language which get recognized by this compiler.

The 20 Keywords with their functions are provided here:

| Keywords | Description                                       |
| -------- | ------------------------------------------------- |
| `PRT`    | Prints the tokens                                 |
| `SCN`    | Takes inputs from the keyboard                    |
| `SCNL`   | Takes inputs upto the next newline character      |
| `MOV`    | Assigns a value to a variable                     |
| `INT`    | Converts a variable to an integer                 |
| `ADD`    | Adds two variables                                |
| `SUB`    | Subtracts two variables                           |
| `MUL`    | Multiplies two variables                          |
| `DIV`    | Divides two variables                             |
| `MOD`    | Modulo of two variables                           |
| `POW`    | Raises a variable to the given power              |
| `IF`     | If the condition is true, executes the block      |
| `>`      | Creates a label                                   |
| `JMP`    | Jumps to a label                                  |
| `ARR`    | Assigns a value to an array                       |
| `ARRV`   | Assigns the element's value into another variable |
| `BYE`    | Exits the program                                 |
| `!`      | Comments out any text                             |
| `STR`    | Stores strings as character arrays                |
| `CAT`    | Concatenates strings                              |

The 8 Logical Operators with their meanings are provided here:

| Logical Operators | Description                                       |
| ----------------- | ------------------------------------------------- |
| `EQL`             | Check if two variables are equal                  |
| `NEQ`             | Check if two variables are not equal              |
| `LSS`             | Check if one variable is less than another        |
| `LEQ`             | Check if one variable is less than or equal to    |
| `GTR`             | Check if one variable is greater than another     |
| `GEQ`             | Check if one variable is greater than or equal to |
| `SEQL`            | Check if two strings are equal                    |
| `SNEQ`            | Check if two strings are not equal                |

---

## Syntax

The syntax is very simplistic and easy to understand.

### Hello World

Here is the Hello World program:

```
PRT Hello World
```

Here, the compiler prints the tokens 'Hello' and 'World'.
Tokens are separated by spaces. The compiler will ignore the spaces.
But it'll print the spaces explicitly.

---

### Assignment Operator

To store a variable, we use the keyword `MOV`.

```
MOV $var1 Hello
```

Here, we assign the value 'Hello' to the variable `$var1`.
There are no data types in this language. So, you can assign any type of data to a variable.

---

### Printing Variables

Now, to print the variable, we use the keyword `PRT`.

```
PRT $var1
```

And yes. You can print multiple variables at once.

---

### Taking Inputs

Now, to take input from the keyboard, we use the keyword `SCN`.

```
SCN $var1
```

Here, the compiler takes input from the keyboard and assign it to the variable `$var1`.
And multiple inputs can be taken at once.

Now, to take input upto the next newline character, we use the keyword `SCNL`.

```
SCNL $var1
```

Here, the compiler takes input upto the next newline character from the keyboard and assign it to the variable `$var1`.

---

### Math Operations

To add, subtract, multiply, divide, modulo, and power two variables, we use the keywords `ADD`, `SUB`, `MUL`, `DIV`, `MOD`, and `POW` respectively.

```
ADD $var3 $var1 $var2
```

Here, the compiler adds the values of the variables `$var1` and `$var2` and assign the result to the variable `$var3`.

Now, just replace `ADD` with `SUB`, `MUL`, `DIV`, `MOD`, and `POW` to get the result of subtraction, multiplication, division, modulo, and power respectively.

But it'll convert the variables to floats before performing the operation.

---

### Integer Casting

To convert a variable to integer, we use the keyword `INT`.

```
INT $var1 $var2
```

Here the compiler converts the variable `$var2` to integer and assign it to the variable `$var1`.

---

### Labels

Instead of separate `loop structures` or `if-else structures` we incorporate only `IF` with `JMP` to perform even the complex tasks. To work with `JMP` easily, we have introduced `labels` in this language.

The `label` is a special type of variable which will hold it's own line number into the given label.

```
> $START
```

It will create a `label` named `$START` which will contain the line number where it is introduced.

---

### If Statements

To implement if-else statements, we use the keywords `IF` and `JMP`.

```
IF EQL $var1 $var2
[
    PRT SUCCESS
    JMP $TERMINATE
]
IF NEQ $var1 $var2
[
    PRT FAILURE
    JMP $TERMINATE
]

> $TERMINATE
```

Here, the compiler checks if the values of the variables `$var1` and `$var2` are equal.
If they are equal, the compiler prints 'SUCCESS' and jump to the label `$TERMINATE`.
If they are not equal (else), the compiler prints 'FAILURE' and jump to the label `$TERMINATE`.

There is no ELSE keyword. So, this is the only way to implement if-else statements.

Now, replace the second token `EQL` with the other logical operators to get the desired condition.

---

### Loops

To implement loops, we use the keyword `JMP` with `label`s.

```
> $LOOP
IF EQL $var1 $var2
[
    JMP $LOOP
]
```

Here, the compiler checks if the values of the variables `$var1` and `$var2` are equal.
If they are equal, the compiler jumps back to the label `$LOOP`.
If they are not equal, the compiler exits the loop.

---

### Printing Special Characters

Now, to print a new line, tab, space, exclamation mark, dollar sign, and ampersand, we print (`PRT`) these predefined variables: `$ENDL`, `$TAB`, `$SPC`, `$EXL`, `$DOL`, `$AMP` respectively.

```
PRT Hello World
PRT $EXL
PRT $ENDL
```

It prints 'Hello World!\n'.

> **_NOTE:_** Other special characters can be printed by using `PRT` with the character itself. For example, `PRT %` prints a percent sign.

---

### Arrays

To store an array, we use the keyword `ARR`.

```
ARR $array $index $value
```

Here, the compiler assigns the value `$value` to the array `$array` at the index `$index`.

To get the value of an array, we use the keyword `ARRV`.

```
ARRV $var1 $array $index
```

Here, the compiler assigns the value of the array `$array` at the index `$index` to the variable `$var1`.

Now, to print the value of the element at the index `$index`, we use the keyword `PRT`.

_Method 1:_

```
MOV $index 0
ARRV $var1 $array $index
PRT $var1
```

Here, the compiler assigns the value of the array `$array` at the index `$index` to the variable `$var1`.
Then the compiler prints the value of the variable `$var1`.

_Method 2:_

```
PRT $array(0)
```

Here, the compiler prints the value of the element at the index `$index` of the array `$array`.

This way of printing an array is not recommended because it's hard coded.
So, it can't be iterated.

---

### Strings

We can perform various string operations in this programming language. The language contains two keywords and two logical operators and a special `NULL` character to deal with strings.

`STR` keyword is used for storing the given string as a character array and add a `NULL` string at the end.

```
PRT Enter your name with underscore separated words:
SPC
SCN $name

STR $name
```

The above code snippet takes a string as input and stores it as a string in the memory.

> **_NOTE:_** As every space separated words are tokenized separately, it will only store one word as string. To avoid this problem we took the input separated by `_` or `underscore` character.

---

### String Concatenation

To concatenate two or more strings together, we use the keyword `CAT`.

```
CAT 2 $str3 $str1 $str2
```

Here, we first specify the number of the strings to be concatenated immediately after the `CAT` keyword. After that we have three string variables `$str1`, `$str2` and `$str3`. The string `$str2` is concatenated with `$str1` and then it is stored into `$str3`.

---

### Comments

To comment a line, we use the exclamation mark `!`.

```
! This is a comment
```

It only supports single line comments.

---

### Bye

To terminate the program, we use the keyword `BYE`.

```
BYE 0
```

Here, the compiler terminates the program with the exit code 0.

---

### Command Line Arguments

To run the interpreter with command line arguments, do the following:

```
$ ./fasml samples/command_line_args.fasml C C++ Python Java
```

Here, we run the file `command_line_args.fasml` with the command line arguments `C`, `C++`, `Python`, and `Java`.

The `command_line_args.fasml` file contains the following code:

```
MOV $I 0

> $LOOP
IF LSS $I $ARGC
[
    INT $I $I
    PRT &I
    ENDL

    ADD $I $I 1

    JMP $LOOP
]
```

> **_NOTE:_** `$ARGC` is predefined. It contains the number of command line arguments.

Here, we assign the value of the variable `$I` to 0.

Then, we check if the value of the variable `$I` is less than the value of the variable `$ARGC`.
If it is true, we print the `$I`th argument from the command line.

Then, we increment the value of the variable `$I` by 1.

Finally, we jump back to the label `$LOOP`.

If we look closely,we find it's `PRT` followed by `&I`. It means that we grab the value of the variable `$I` and put a extra `$` before it.
So, the interpreter will print the value of `$0`, `$1`, `$2`, and so on.

You can also print them in the following way:

```
PRT $0 $1 $2
```

> **_NOTE:_** Here `$0` is the file name. And `$1`, `$2`, and so on are the command line arguments. These are predefined.

---

## Samples

There are some examples in the [samples](https://github.com/SohamMalakar/Fun-Assembly/tree/master/samples) folder. You can analyze them to get a better understanding.

> **_NOTE:_** Make sure these files are in LF format and not in CRLF format.

---

## Contribution

If you want to contribute to this language, feel free to open an [issue](https://github.com/SohamMalakar/Fun-Assembly/issues) or create a [pull request](https://github.com/SohamMalakar/Fun-Assembly/pulls).
