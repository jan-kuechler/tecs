--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   13:31:00 03/05/2011
-- Design Name:   
-- Module Name:   F:/Programmieren/TECS/VHDL/Hack/Bit_tb.vhd
-- Project Name:  Hack
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Bit
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
ENTITY Bit_tb IS
END Bit_tb;
 
ARCHITECTURE behavior OF Bit_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Bit
    PORT(
         clk : IN  std_logic;
         x : IN  std_logic;
         load : IN  std_logic;
         z : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal x : std_logic := '0';
   signal load : std_logic := '0';

 	--Outputs
   signal z : std_logic;

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Bit PORT MAP (
          clk => clk,
          x => x,
          load => load,
          z => z
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clk_period*10;
		
		x <= '0';
		load <= '1';
		wait for clk_period;	
		assert(z = '0');
		
		x <= '1';
		load <= '0';
		assert(z = '0');
		wait for clk_period;
		assert(z = '0');
		
		x <= '1';
		load <= '1';
		assert(z = '0');
		wait for clk_period;
		assert(z = '1');
		
		x <= '0';
		load <= '1';
		assert(z = '1');
		wait for clk_period;
		assert(z = '0');
	
      wait;
   end process;

END;
