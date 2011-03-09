// Pseudo Code
// R2 = R0 * R1
//
// for i=0 to *R1 do *R2 = *R2 + *R0 end
// 
// i = 0
// loop:
//   if (i - R1) >= 0 then
//     goto endloop
//   end
//   R2 = R2 + R1
//   i = i + 1
//   goto loop
//

// set i to 0
@i
M=0

// set R2 to 0
@R2
M=0

(LOOP)
	@i
	D=M
	
	@R1
	D=D-M
	
	@ENDLOOP
	D;JGE

	@R0
	D=M
	
	@R2
	M=M+D
		
	@i
	M=M+1
	
	@LOOP
	0;JMP
	
(ENDLOOP)
