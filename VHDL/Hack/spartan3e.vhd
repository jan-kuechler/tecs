library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity spartan3e is
	port (
		clk : in std_logic;
		
		btn_south, btn_west, btn_north: in std_logic;
		
		led : out std_logic_vector(7 downto 0)
	);
end;

architecture behavioral of spartan3e is
	signal reset : std_logic;
begin
	reset <= btn_south;
	
	led(7 downto 2) <= "000000";
	
	test : process(clk, reset)
	begin
		if reset = '1' then
		
		elsif clk'event and clk = '1' then
			led(0) <= btn_west;
			led(1) <= btn_north;
		end if;
	end process;
end;
