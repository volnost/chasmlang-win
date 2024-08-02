# Chasm Language Windows c-based interpreter.
A sort of OOASM (object-oriented assembly) intented for a future OS kernel.
## Command list
### int 'name' 'value'
Creates and integer, sets it to 0 automatically if the value isn't given.
### inc 'name' 'value'
Adds a static value to an integer.
### add 'name' 'name2'
Adds an integer to the first integer.
### intout 'name' 'format'
Prints the integer, if 'format' is a '!' prints a new line after it.
### intcmp 'value' 'value2'
Compares two values. Result is stored in program. Takes in two values so use ! for integers.
### intptr 'name' 'name2'
Puts the in-language pointer to the second integer into the first one.
### str 'name'
Creates an empty string.
### strin 'name'
Puts the input given by the user into the string.
### strout 'name' 'format'
Prints the string, if the format is a '!' prints a new line after.
### label 'name'
Creates a label to jump to, as in assembly, c or batch...
### do 'name'
Jumps to a label and returns when it detects the ret command.
### ret
Returns to where do was called.
### jump 'name'
Jumps to a previously created label.
### je 'name'
Jumps if the previous comparison resulted in 0.
### jee
Exits if the previous comparison resulted in 0.
### 'out' 
Prints the text after it, useful with !.
### 'lnout'
Prints a new line.
### 'command' !'argument'
This is useful for functions like inc or out, it converts the name of the function into its value -> **inc a !b** will work the same as **add a b**.
### end
Quits the program
