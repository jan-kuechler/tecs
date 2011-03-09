----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:27:10 03/05/2011 
-- Design Name: 
-- Module Name:    Bit - Behavioral 
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

--component Bit is
--    Port ( clk : in  STD_LOGIC;
--           x, load : in  STD_LOGIC;
--           z : out  STD_LOGIC);
--end component;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity Bit is
    Port ( clk : in  STD_LOGIC;
           x, load : in  STD_LOGIC;
           z : out  STD_LOGIC);
end Bit;

architecture Behavioral of Bit is

	component DFF is
		Port ( clk : in  STD_LOGIC;
				x : in  STD_LOGIC;
				z : out  STD_LOGIC);
	end component;

	signal dffIn, dffOut : std_logic;
begin
	
	dffIn <= x when load = '1' else dffOut;
	
	flipflop : DFF port map (
		clk => clk,
		x => dffIn,
		z => dffOut
	);
	
	z <= dffOut;

end Behavioral;

