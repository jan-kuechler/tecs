----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:23:52 03/05/2011 
-- Design Name: 
-- Module Name:    DFF - Behavioral 
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

--component DFF is
--    Port ( clk : in  STD_LOGIC;
--           x : in  STD_LOGIC;
--           z : out  STD_LOGIC);
--end component;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity DFF is
    Port ( clk : in  STD_LOGIC;
           x : in  STD_LOGIC;
           z : out  STD_LOGIC);
end DFF;

architecture Behavioral of DFF is

begin

	tick : process (clk)
	begin
		if rising_edge(clk) then
			z <= x;
		end if;
	end process;

end Behavioral;

