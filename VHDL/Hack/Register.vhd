----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:34:32 03/05/2011 
-- Design Name: 
-- Module Name:    Register - Behavioral 
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
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Register16 is
    Port ( clk : in  STD_LOGIC;
           x : in std_logic_vector(15 downto 0);
			  load : in  STD_LOGIC;
           z : out  STD_LOGIC_VECTOR(15 downto 0)
			 );
end Register;

architecture Behavioral of Register16 is

	component Bit is
		 Port ( clk : in  STD_LOGIC;
				  x, load : in  STD_LOGIC;
				  z : out  STD_LOGIC);
	end component;

begin

	gen: for i in 0 to 15 generate
		b : Bit port map (
			clk => clk,
			load => load,
			x => x(i),
			z => z(i)
		);
	end generate;

end Behavioral;

