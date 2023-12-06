# CFGProject 
Interpreter and REPL for a language specified as a CFG.
# Building The Project
Uses CMake as a build generator. Includes two targets (executables):
- Repl.exe: The main executable; An interactive shell where you can run files and language instructions.
- Tests.exe: A series of simple tests, similar to those specified in the project instructions.
Executables are placed in the 'bin' folder.
# CFG Specification
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
