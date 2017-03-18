-- fsm.vhd: Finite State Machine
-- Author(s): Petr Buchal (xbucha02)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (HESLOAB1, HESLOAB2, HESLOAB11, RIGHT, WRONG, CHYBA, HESLOA3, HESLOA4, HESLOA5, HESLOA6, HESLOA7, HESLOA8, HESLOA9, HESLOA10, HESLOB3, HESLOB4, HESLOB5, HESLOB6, HESLOB7, HESLOB8, HESLOB9, HESLOB10, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= HESLOAB1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOAB1 =>
      next_state <= HESLOAB1;
      if (KEY(1) = '1') then
         next_state <= HESLOAB2;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOAB2 =>
      next_state <= HESLOAB2;
      if (KEY(5) = '1') then
         next_state <= HESLOA3;
      elsif (KEY(6) = '1') then
      	 next_state <= HESLOB3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA3 =>
      next_state <= HESLOA3;
      if (KEY(9) = '1') then
         next_state <= HESLOA4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA4 =>
      next_state <= HESLOA4;
      if (KEY(2) = '1') then
         next_state <= HESLOA5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA5 =>
      next_state <= HESLOA5;
      if (KEY(8) = '1') then
         next_state <= HESLOA6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA6 =>
      next_state <= HESLOA6;
      if (KEY(9) = '1') then
         next_state <= HESLOA7;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA7 =>
      next_state <= HESLOA7;
      if (KEY(3) = '1') then
         next_state <= HESLOA8;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA8 =>
      next_state <= HESLOA8;
      if (KEY(7) = '1') then
         next_state <= HESLOA9;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA9 =>
      next_state <= HESLOA9;
      if (KEY(5) = '1') then
         next_state <= HESLOA10;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOA10 =>
      next_state <= HESLOA10;
      if (KEY(6) = '1') then
         next_state <= HESLOAB11;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOAB11 =>
      next_state <= HESLOAB11;
      if (KEY(15) = '1') then
         next_state <= RIGHT;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB3 =>
      next_state <= HESLOB3;
      if (KEY(0) = '1') then
         next_state <= HESLOB4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB4 =>
      next_state <= HESLOB4;
      if (KEY(5) = '1') then
         next_state <= HESLOB5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB5 =>
      next_state <= HESLOB5;
      if (KEY(9) = '1') then
         next_state <= HESLOB6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB6 =>
      next_state <= HESLOB6;
      if (KEY(2) = '1') then
         next_state <= HESLOB7;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB7 =>
      next_state <= HESLOB7;
      if (KEY(7) = '1') then
         next_state <= HESLOB8;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB8 =>
      next_state <= HESLOB8;
      if (KEY(4) = '1') then
         next_state <= HESLOB9;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB9 =>
      next_state <= HESLOB9;
      if (KEY(7) = '1') then
         next_state <= HESLOB10;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when HESLOB10 =>
      next_state <= HESLOB10;
      if (KEY(9) = '1') then
         next_state <= HESLOAB11;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= CHYBA;
      elsif (KEY(15) = '1') then
         next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CHYBA =>
      next_state <= CHYBA;
      if (KEY(15) = '1') then
        next_state <= WRONG;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG =>
      next_state <= WRONG;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when RIGHT =>
      next_state <= RIGHT;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= HESLOAB1;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= HESLOAB1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------PROGRESS
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when RIGHT =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   end case;
end process output_logic;

end architecture behavioral;

