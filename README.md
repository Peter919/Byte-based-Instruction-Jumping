# Thanks for opening this page :)
I'm pretty sure this program only works for windows, because it uses system commands and notepad (I've never really used Linux or iOS)

To learn how BIJ programs work, open this link: https://esolangs.org/wiki/Byte-based_Instruction_Jumping

Now that you know how BIJ works, you should probably know how to use this little piece of software. At the beginning of the program, you'll be prompted to enter a file name. This is the name from a relative path from the project directory. Do NOT surround the name with quotation marks or double quotation marks, as this is done for you. If you want to access a file inside of a folder, use slash or backslash. Let's assume that you want to access testing.bij inside of the bij files folder. In that case, you simply write bij files/testing.bij or bij files\testing.bij when you're asked to enter a BIJ file.

Three file windows will now be opened in notepad: A bij file, a hex file and an "ins" file (instruction file). You can edit any of those files, but to update the others, you'll need a separate command. Write "bij" to translate the bij file to hex and ins. Write "hex" to translate the hex file to bij and ins. Write "ins" to translate the ins file to bij and hex.

To run the BIJ program you've written, type "run". To exit, type "exit" before closing the program window (no damage will be done if you don't, but it cleans things up a little).

Do NOT edit hex or ins files without running main.c. These changes will be discarded the next time you run main.c.

This is not a very sophisticated piece of software, and there are certainly things that could've been better:

- When you've started running a program, you can't exit manually. If the program is an infinite loop, you simply have to close the window.
- You cannot open multiple BIJ files without running this program multiple times.
- main.c isn't as clean as it probably should be
- running.c has a million line run_instr function

#a
##b
###c
####d
