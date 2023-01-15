// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

@SCREEN
D=A
@R0 // loop counter (*(i++))
M=D

@SCREEN
D=A
@R1 // loop begin (16384)
M=D

@24576
D=A
@R2 // loop end (begin (16384) + screen width*height = (128*256/16 or 8192))
M=D
@R3 // bit that checks if screen has already been cleared; if true, don't clear until after the next draw
M=0
(LOOP)
  
  @KBD
  D=M;
  @CLEAR
  D;JEQ
  
  @DRAW
  0;JEQ
  
(DRAW)
  // jump if clear bit is false
  @R3
  D=M
  @LOOP
  D;JEQ
  
  @R0
  D=M
  @R2
  D=D-M
  @LOOP
  D;JEQ
  
  @R0
  M=-1
  A=A+1
  
  // set clear bit to false
  @R3
  M=0
  @LOOP
  0;JEQ
(CLEAR)
  // jump if clear bit is true
  @R3
  D=M
  @LOOP
  D;JGT
  
  @SCREEN
  M=0
  
  // set clear bit to true
  @R3
  M=1
  @LOOP
  D;JEQ
(STOP)

(END)