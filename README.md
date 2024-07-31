# Chasm Language windows c-based interpreter.
A sort of OOASM (object-oriented assembly) intented for a future OS kernel.
## Command list
### int 'name'
Creates and integer, sets it to 0 automatically.
### add 'name' 'value'
Adds a static value to an integer.
### !add 'name' 'name2'
Adds an integer to the first integer.
### intcmp 'name' 'value'
Compares and integer to a static value, results in 0 if equal, 1 if greater or -1 if smaller (than given value).
### label 'name'
Creates a label to jump to, as in assembly, c or batch...
### jump 'name'
Jumps to a previously created label.
### je 'name'
Jumps if the previous comparison resulted in 0.
### jee
Exits if the previous comparison resulted in 0.
### end
Quits the program
