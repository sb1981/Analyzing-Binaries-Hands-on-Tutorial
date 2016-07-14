Reverse Engineering
===================

Working with only the assembler-code can be very hard and frustrating, especially when analyzing complex programs. So it is advised, that you figure out how the program will look in a higher level language than assembler. This may help to find out special behaviors (potential security leaks) or to see what data the program will need to work.

Since developers are aware of reverse engineering, there are some counter-measures. One of them is code-obfuscation, which makes the binary much more complex than the original source code would be.

_Note: the serialnumber is numerical_

Exercise 1
----------

Try to figure out what how the c-program of the provided binary would look like. With this you can write a small program to figure out the right serial number.

The usage of this program is `./ex1 username serialnumber`

[download binary (zip)](ex1.zip)

Exercise 2
----------

Similar to Exercise 1, you have to figure out what how the c-program of the provided binary would look like. But this time code is more complex.

The usage of this program is `./ex2 username serialnumber`


[download binary (zip)](ex2.zip)
