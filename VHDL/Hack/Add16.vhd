----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:57:50 03/05/2011 
-- Design Name: 
-- Module Name:    Add16 - Behavioral 
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

--component Add16 is
--	port ( a, b : in std_logic_vector(15 downto 0);
--	       sum  : out std_logic_vector(15 downto 0)
--			 );
--end component;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Add16 is
	port ( a, b : in std_logic_vector(15 downto 0);
	       sum  : out std_logic_vector(15 downto 0)
			 );
end Add16;

architecture Behavioral of Add16 is

	component HalfAdder is
		Port ( a, b : in  STD_LOGIC;
				sum, carry : out  STD_LOGIC);
	end component;

	component FullAdder is
		 Port ( a, b, c : in  STD_LOGIC;
				  sum, carry : out  STD_LOGIC);
	end component;

	signal c : std_logic_vector(15 downto 0);

begin

	add0 : HalfAdder port map(
		a => a(0),
		b => b(0),
		sum => sum(0),
		carry => c(0)
	);

	gen_add: for i in 1 to 15 generate
		add : FullAdder port map (
			a => a(i),
			b => b(i),
			c => c(i-1),
			sum => sum(i),
			carry => c(i)
		);
	end generate;

end Behavioral;

