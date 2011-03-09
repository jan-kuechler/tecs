// Variables:
// @out : contains the address of the screenspace to be written to
// @clr : contains the current color (full black or white)

// Load the screen address to @out
(PRELOOP)
	@SCREEN
	D=A
	@out
	M=D

(LOOP)
	// Check key state
	@KBD
	D=M
	
	@KEYDOWN
	D;JNE
	
		// No key is down, load white (0)
		@0
		D=A
		@clr
		M=D

		@CONTLOOP
		0;JMP
	
	(KEYDOWN)
		// A key is pressed, load black (0xFFFF)
		@32767  // 0x7FFF is the largest constant to be loaded directly
		D=A     // So we have to construct 0xFFFF manually
		D=D+A
		D=D+1

		@clr
		M=D
	
(CONTLOOP)
	
	// Load the color
	@clr
	D=M

	// and write it to the screen
	@out
	A=M
	M=D
	
	// Move to next 8 pixels
	@out
	M=M+1
	D=M
	
	// Jump to PRELOAD when @out is past the end of the screen
	// 24576 = SCREEN+8K is the end of the screen
	@24576
	D=D-A
	
	@PRELOOP
	D;JGE
	
	@LOOP
	0;JMP
