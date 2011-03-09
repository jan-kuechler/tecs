----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:44:53 03/05/2011 
-- Design Name: 
-- Module Name:    HalfAdder - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------

--component HalfAdder is
--   Port ( a, b : in  STD_LOGIC;
--           sum, carry : out  STD_LOGIC);
--end component;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity HalfAdder is
    Port ( a, b : in  STD_LOGIC;
           sum, carry : out  STD_LOGIC);
end HalfAdder;

architecture Behavioral of HalfAdder is

begin

	sum <= a xor b;
	carry <= a and b;

end Behavioral;

