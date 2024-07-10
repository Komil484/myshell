# Table of Contents
- [About](#about)
- [Features](#features)
  - [Shell specific commands](#shell-specific-commands)
  - [Environment variables](#environment-variables)
    - [Example](#example)
  - [Quotation marks](#quotation-marks)
    - ["Double quotes"](#-double-quotes-)
    - ['Single quotes'](#-single-quotes-)
    - [\`Backticks\`](#--backticks--)
  - [Argument parsing](#argument-parsing)
    - [Example](#example-1)

# About
A command shell for Linux with very limited functionality, made in the C programming language.

This is a student project, made for the purpose of gaining more experience with the C language, as well as studying how command shells work. More specifically: how commands are processed by the shell, what kinds of decisions have had to be made and to disambiguate what is handled by the shell compared to what is handled by other programs or commands. 

# Features
Let me start out by saying some of the things the shell does **not** have, which are usually taken for granted:
- "Up arrow" cannot be used to go to the last executed command
- "Tab" cannot be used to autofill arguments
- Has no ability to pipe multiple commands
- The shell itself does not color code anything
- Has no scripting language, and cannot run from files

Now onto what the shell provides.

## Shell specific commands
- `<command> [args..]`: Calls the program `command` with the arguments `args`.
  - Searches for `command` only in `PATH`, unless prefixed by `./`.

- `help`: Prints out a list of commands provided by the shell. ***(WIP)***

- `cd [path]`: Goes to the directory `path`.
  - If no path is given, goes to the home directory given by the `HOME` environment variable.

- `var <variable> [value] [no-overwrite]`: Exports the environment variable with name `variable`.
  - If no value is given, the variable is unset.
  - If no-overwrite is set, and the variable exists, the value will not be changed, and an error will be printed out.

- `exit`: Exits the shell.

## Environment variables
Environment variables can be edited by using the `var` keyword as mentioned above.

Passing their values can be done by prefixing `$` to the variable name.

When accessing variables in this way, the characters of the variable names can contain only alphanumeric or underscores, except the first character, which cannot be a digit.

### Example
```
var my_var Hello
```
will set the variable `my_var` to `Hello`.

```
echo $my_var
```
will be treated as if it were `echo Hello`.

Hence it will output `Hello`.

## Quotation marks
There are 3 quotation marks that are recognized by the shell:
- `"` double quotes
- `'` single quotes
- `` ` `` backticks

And they all have a few differences.

Any quotation mark that has not been closed, will be automatically closed at the end of the line.

So `echo "asd` will be treated as `echo "asd"`

### "Double quotes"
Most common escape sequences can be escaped in double quotes: `\n`, `\t`, `\r`, `\\`, `\'`, `` \` ``.

### 'Single quotes'
Single quotes accept all characters as raw characters, nothing can be escaped.
This means that they cannot contain single quotes inside of them.

### \`Backticks\`
Only 2 characters can be escaped inside of backticks: `\\` and `` \` ``.

## Argument parsing
The arguments are separated by any whitespace character not in any quotation marks, some common ones are ` ` and `\t`.

As long as there are no whitespaces between any characters, they will be treated as one argument, this allows for mixing between different quotations and environment variables.

### Example
```
var my_var Hello
echo `in backticks`"\nin double quotes\n"'in single quotes'$my_var
```
In the line above, everything after `echo` will be passed as one argument, as there are no spaces between the quotation marks.

And the output will be
```
in backticks
in double quotes
in single quotesHello
```
