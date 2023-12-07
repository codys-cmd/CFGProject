# CFGProject 
Interpreter and REPL for a language specified as a CFG.
# Building The Project
Uses CMake as a build generator. Includes two targets (executables):
- Repl.exe: The main executable; An interactive shell where you can run files and language instructions.
- Tests.exe: A series of simple tests, similar to those specified in the project instructions.
Executables are placed in the 'bin' folder.
# Language Description & Examples
The language supports if statements, if-else statements, and while loops. 

Variables are dynamically typed, and may have types of integer, floating-point,
boolean, and undefined. 

Variables that are just 'defined' instead of assigned to recieve a value of undefined.

There are also basic print statements- variables cannot be printed.

There is no support for comments. The comments in the example are for purely demonstrative purposes.

Incorrect syntax will result in undefined behavior (the language will not throw any errors).

## Example of language code:
```
a;           //Variable definition.
a = 5;       //Variable assignment.

//If Block
if (a < 10) { 
    b = true;
}

//If-Else Block
if (a < 5) { 
    print("A is less than 5"); //Print Statement
} else {
    print("A is not less than 5");
}

//While loop
i = 0;
while (i < 10) {
    print("loop!");
    i = i + 1;
}
```
## CFG Specification
- Statement -> Assignment | Definition | Control Block | Print Statement | ∅
- Assignment -> Symbol '=' Expression ';' | Assignment Statement

- Symbol -> Character | Character Symbol
- Character -> 'a' | 'b' | 'c' | 'd' | 'e' | ... | 'z'

- Expression -> '(' Expression ')' Operator Expression |
                 Expression Operator '(' Expression ')' |
                 Expression Operator Value | Expression Operator Symbol |
                 Value | Symbol
- Operator -> '+' | '-' | '*' | '/' | '^' | '==' | '&&' | '||' | '<' | '>'
- Value -> Integer | Boolean | Floating-Point
- Integer -> Digit | Digit Integer
- Digit -> '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '0'
- Boolean -> 'true' | 'false'
- Floating-Point -> Integer '.' Integer

- Definition -> Symbol ';'

- Control Block -> If Block | While Block
- If Block -> If Block Else Block | 'if (' Expression ') {' Statement '}'
- Else Block -> 'else {' Statement '}'
- While Block -> 'while (' Expression ') {' Statement '}'
