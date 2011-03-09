----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:52:49 03/05/2011 
-- Design Name: 
-- Module Name:    FullAdder - Behavioral 
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

--component FullAdder is
--    Port ( a, b, c : in  STD_LOGIC;
--           sum, carry : out  STD_LOGIC);
--end component;


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity FullAdder is
    Port ( a, b, c : in  STD_LOGIC;
           sum, carry : out  STD_LOGIC);
end FullAdder;

architecture Behavioral of FullAdder is

component HalfAdder is
   Port ( a, b : in  STD_LOGIC;
          sum, carry : out  STD_LOGIC);
end component;

	signal c1, c2 : std_logic;
	signal tmpSum : std_logic;

begin

	h1 : HalfAdder port map (
		a => a,
		b => b,
		sum => tmpSum,
		carry => c1
	);
	
	h2 : HalfAdder port map (
		a => c,
		b => tmpSum,
		sum => sum,
		carry => c2
	);
	
	carry <= c1 or c2;

end Behavioral;

