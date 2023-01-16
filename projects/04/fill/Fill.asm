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
@R0 // loop counter (*(i++))
M=0

@SCREEN
D=A
@R1 // loop begin (16384)
M=D

@24576
D=A
@R2 // loop end (begin (16384) + screen width*height = (128*256/16 or 8192))
M=D
@R3 // bit that checks if screen has already been cleared; if TRUE, don't clear until after the next draw. if FALSE, the screen has not been cleared; don't keep redrawing the screen
M=0
(LOOP)
  
  @KBD
  D=M;
  @CLEAR
  D;JEQ
  
  @DRAW
  0;JEQ
  
(DRAW)
  // go to LOOP if clear bit is FALSE
  @R3
  D=M
  @LOOP
  D;JEQ
  @DRAWLOOP
  0;JEQ
(DRAWLOOP)
  @R1 // go to LOOP if R1 == R2
  D=M
  @R0
  D=D+M
  @R2
  D=M-D
  @DRAWEND
  D;JEQ
  // set RAM[16384]..RAM[24575] to -1
  @R1
  D=M
  @R0
  A=D+M
  M=-1
  @R0
  M=M+1
  @DRAWLOOP
  0;JEQ
(DRAWEND)
  // set R0 back to 16384
  @R0
  M=0
  // set clear bit to FALSE
  @R3
  M=0
  @LOOP
  0;JEQ

(CLEAR)
  // go to LOOP if clear bit is TRUE
  @R3
  D=M
  @LOOP
  D;JGT
  @CLEARLOOP
  0;JEQ
(CLEARLOOP)
  @R1 // go to LOOP if R1 == R2
  D=M
  @R0
  D=D+M
  @R2
  D=M-D
  @CLEAREND
  D;JEQ
  // set RAM[16384]..RAM[24575] to 0
  @R1
  D=M
  @R0
  A=D+M
  M=0
  @R0
  M=M+1
  @CLEARLOOP
  0;JEQ
(CLEAREND)
  // set R0 back to 16384
  @R0
  M=0
  // set clear bit to TRUE
  @R3
  M=1
  @LOOP
  0;JEQ

(STOP)

(END)