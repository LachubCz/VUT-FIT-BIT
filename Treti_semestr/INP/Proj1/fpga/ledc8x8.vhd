library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 
	SMCLK:	in std_logic;
	RESET:	in std_logic;
	ROW:	out std_logic_vector (7 downto 0);
	LED:	out std_logic_vector (7 downto 0)
);
end ledc8x8;

architecture main of ledc8x8 is

    signal ce: STD_LOGIC := '0';
    signal ce_cnt: std_logic_vector (7 downto 0) := (others => '0') ;
    signal ce_cnt2: std_logic_vector (22 downto 0) := (others => '0');
    signal sel: STD_LOGIC := '0';
    signal row_cnt: std_logic_vector (7 downto 0) := (others => '0');
    signal led_cnt: std_logic_vector (7 downto 0) := (others => '0');

begin
	LED <= led_cnt;
	ROW <= row_cnt;

	ce_gen: process(SMCLK, RESET)
	begin
		if SMCLK'event and SMCLK = '1' then
			if RESET = '1' then
				ce_cnt <= "00000000";
			end if;

			ce_cnt <= ce_cnt + 1;

			if (ce_cnt = "11111111") then
				ce <= '1';
			else
				ce <= '0';
			end if;

		end if;
	end process ce_gen;
			

	process(RESET, SMCLK, ce)
	begin
	   if (RESET='1') then
	      row_cnt <= "10000000";
	   elsif (SMCLK'event) and (SMCLK='1') then
	      if (ce='1') then
	         row_cnt <= row_cnt(0) & row_cnt(7 downto 1);
	      end if;
	   end if;
	end process;


	ce_gen2: process(SMCLK, RESET)
	begin
		if SMCLK'event and SMCLK = '1' then
			if RESET = '1' then
				ce_cnt2 <= (others => '0');
			end if;

			ce_cnt2 <= ce_cnt2 + 1;

			if (ce_cnt2 = "1110000100000000000000") then
				if sel = '1' then
				 	sel <= '0';
				 	ce_cnt2 <= (others => '0');
				else
					sel <= '1';
					ce_cnt2 <= (others => '0');
				end if; 
			end if;
		end if;
	end process ce_gen2;


	display: process (row_cnt, sel)
	begin
		if sel = '0' then
			case row_cnt is --pro pismeno p
				when "10000000" => led_cnt <= "11111111";
				when "01000000" => led_cnt <= "11111110";
				when "00100000" => led_cnt <= "11111110";
				when "00010000" => led_cnt <= "11111110";
				when "00001000" => led_cnt <= "11111000";
				when "00000100" => led_cnt <= "11110110";
				when "00000010" => led_cnt <= "11110110";
				when "00000001" => led_cnt <= "11111000";
				when others 	=> led_cnt <= "11111111";
			end case;
		else
			case row_cnt is --pro pismeno b
				when "10000000" => led_cnt <= "11111111";
				when "01000000" => led_cnt <= "11110000";
				when "00100000" => led_cnt <= "11101110";
				when "00010000" => led_cnt <= "11101110";
				when "00001000" => led_cnt <= "11110000";
				when "00000100" => led_cnt <= "11110110";
				when "00000010" => led_cnt <= "11110110";
				when "00000001" => led_cnt <= "11111000";
				when others 	=> led_cnt <= "11111111";
			end case;
		end if;
	end process;
end main;
