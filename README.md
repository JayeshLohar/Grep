# DSA Project : GREP
To run program run the command `make`
Usage: `grep [OPTION]... PATTERNS [FILE]...`
Try `'grep --help'` for more information.
### Flags :
  - `-i`  ignore case distinctions in patterns and data
  - `-w`  match only whole words
  - `-x`  match only whole lines
  - `-v`  select non-matching lines
  - `-m`  stop after NUM selected lines
  - `-b`  print the byte offset with output lines
  - `-n`  print line number with output lines
  - `-H`  print file name with output lines
  - `-h`  suppress the file name prefix on output
  - `-o`  show only nonempty parts of lines that match
  - `-r`  to handle directories
  - `-L`  print only names of FILEs with no selected lines
  - `-l`  print only names of FILEs with selected lines
  - `-c`  print only a count of selected lines per FILE
  - `-E`  patterns are extended regular expressions
  - `-q`  quit
###  Regular Expression Implemented :
  -  `^`         Start anchor, matches beginning of string
  -  `$`         End anchor, matches end of string
  -  `*`         Asterisk, match zero or more (greedy)
  -  `+`         Plus, match one or more (greedy)
  -  `?`         Question, match zero or one (non-greedy)
  -  `[abc]`     Character class, match if one of {'a', 'b', 'c'}
  -  `[^abc]`   Inverted class, match if NOT one of {'a', 'b', 'c'}
  -  `[a-zA-Z]` Character ranges, the character set of the ranges { a-z | A-Z }
  -  `\s`       Whitespace, \t \f \r \n \v and spaces
  -  `\S`       Non-whitespace
  -  `\w`       Alphanumeric, [a-zA-Z0-9_]
  -  `\W`       Non-alphanumeric
  -  `\d`       Digits, [0-9]
  -  `\D`       Non-digits
