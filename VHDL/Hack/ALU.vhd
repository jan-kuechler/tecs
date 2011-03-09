----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:06:05 03/05/2011 
-- Design Name: 
-- Module Name:    ALU - Behavioral 
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

entity ALU is
    Port ( x, y : in  STD_LOGIC_VECTOR(15 downto 0);
           zx, nx : in  STD_LOGIC;
           zy, ny : in  STD_LOGIC;
           f, no : in  STD_LOGIC;
           z : out  STD_LOGIC_VECTOR(15 downto 0);
           zr, ng : out  STD_LOGIC);
end ALU;

architecture Behavioral of ALU is

	component Add16 is
		port ( a, b : in std_logic_vector(15 downto 0);
				sum  : out std_logic_vector(15 downto 0)
				);
	end component;

	signal xOrZero, yOrZero : std_logic_vector(15 downto 0);
	signal theX, theY : std_logic_vector(15 downto 0);
	
	signal xAndY, xPlusY : std_logic_vector(15 downto 0);
	
	signal preRes, result : std_logic_vector(15 downto 0);
	
	signal tmp : std_logic_vector(13 downto 0);
begin

	xOrZero <= (others => '0') when zx = '1' else x;
	yOrZero <= (others => '0') when zy = '1' else y;
				  
	theX <= not xOrZero when nx = '1' else xOrZero;
	theY <= not yOrZero when ny = '1' else yOrZero;

	xAndY <= theX and theY;
	
	add : Add16 port map (
		a => theX,
		b => theY,
		sum => xPlusY
	);

	preRes <= xAndY when f = '0' else xPlusY;
	result <= preRes when no = '0' else not preRes;
	
	z <= result;
	ng <= result(15);
	
	tmp(0) <= result(0) nor result(1);
	nor16: for i in 2 to 13 generate
		tmp(i-1) <= result(i) nor tmp(i-2);
	end generate;
	zr <= result(15) nor tmp(13);
	
end Behavioral;

