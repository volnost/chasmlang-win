# Chasm Language windows c-based interpreter.
A sort of OOASM (object-oriented assembly) intented for a future OS kernel.
## Command list
### int 'name'
Creates and integer, sets it to 0 automatically.
### add 'name' 'value'
Adds a static value to an integer.
### !add 'name' 'name2'
Adds an integer to the first integer.
### intout 'name' 'format'
Prints the integer, if 'format' is a '!' prints a new line after it.
### intcmp 'name' 'value'
Compares and integer to a static value, results in 0 if equal, 1 if greater or -1 if smaller (than given value).
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
### .'text'
Prints the text.
### !'text'
Adds the text after the exclemation mark to the next defined string. (Currently broken!)
### 'lnout'
Prints a new line.
### end
Quits the program
