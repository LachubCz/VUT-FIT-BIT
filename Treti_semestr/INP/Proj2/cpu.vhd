-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

   signal incpc: STD_LOGIC := '0';
   signal decpc: STD_LOGIC := '0';
   signal datapc: STD_LOGIC_VECTOR(11 downto 0) := (others => '0');

   signal incptr: STD_LOGIC := '0';
   signal decptr: STD_LOGIC := '0';
   signal dataptr: STD_LOGIC_VECTOR(9 downto 0) := (others => '0');
	
   signal datatmp: STD_LOGIC_VECTOR(7 downto 0) := (others => '0');

   signal mux: STD_LOGIC_VECTOR(1 downto 0) := (others => '0');
   
   type instenum is (inccell, deccell, incdata, decdata, print, entry, lbracket, rbracket, endnull, other, exmark, dollar);
   signal instr : instenum;

   type fsmState is (init, load,inccell2, deccell2, incdata2, decdata2, print2, entry2, lbracket2, rbracket2, endnull2, skip, decloop, skipmid, decloopmid, other2);
   signal pstate : fsmState;
   signal nstate : fsmState;

begin

-- pc registr - pohybuje se v kodu
   CODE_ADDR <= datapc;
   pc: process(RESET, CLK, incpc, datapc, decpc)
   begin
      if (RESET='1') then
         datapc <= (others => '0');
      elsif (CLK'event) and (CLK='1') then
         if (incpc = '1') then
            datapc <= datapc + 1;
         end if;
         if (decpc = '1') then
            datapc <= datapc - 1;
         end if;
      end if;
   end process;


-- ptr regitr - data programu
   DATA_ADDR <= dataptr;
   ptr: process(RESET, CLK, dataptr, decptr, incptr)
   begin
      if (RESET='1') then
         dataptr <= (others => '0');
      elsif (CLK'event) and (CLK='1') then
         if (incptr = '1') then
            dataptr <= dataptr + 1;
         end if;
         if (decptr = '1') then
            dataptr <= dataptr - 1;
         end if ;
      end if;
   end process;

-- multiplexor - dodelat pouze vzor
   process(CLK,IN_DATA,DATA_RDATA,mux)
   begin
      if (mux="00") then
         DATA_WDATA <= IN_DATA;
      elsif (mux="01") then
         DATA_WDATA <= DATA_RDATA + 1;
      elsif (mux="10") then
         DATA_WDATA <= DATA_RDATA - 1;
		elsif (mux="11") then
			DATA_WDATA <= datatmp;
      end if;
   end process;


   instruction: process(CODE_DATA, instr)
   begin
      case CODE_DATA is  
         when x"3E" => instr <= inccell;
         when x"3C" => instr <= deccell;
         when x"2B" => instr <= incdata;
         when x"2D" => instr <= decdata;
         when x"5B" => instr <= lbracket;
         when x"5D" => instr <= rbracket;
         when x"2E" => instr <= print;
         when x"2C" => instr <= entry;
			when x"24" => instr <= dollar;
			when x"21" => instr <= exmark;
         when x"00" => instr <= endnull;
         when others => instr <= other; --komentare
      end case;
   end process;
	
--registr tmp

-- fsm pro soucasny stav
   pfsm: process(CLK, RESET, EN)
   begin
      if (RESET='1') then
         pstate <= init;
      elsif (CLK'event) and (CLK='1') then
         if (EN = '1') then
            pstate <= nstate;
         end if ;
      end if;
   end process;

-- fsm pro dalsi stav
   nfsm: process(OUT_BUSY,CODE_DATA,DATA_RDATA, pstate, instr)--dodelat argumenty
   begin
      incpc <= '0';
      decpc <= '0';
      incptr <= '0';
      decptr <= '0';
      DATA_EN <= '0';
      CODE_EN <= '0';
      OUT_WE <= '0';
      mux <= "00";
      nstate <= init;      --nulovani signalu

      
      case pstate is

         when init =>
            CODE_EN <= '1';
            nstate <= load; 

         when load =>
            case instr is
               when inccell =>
                  --incpc <= '1';
                  incptr <= '1';
                  nstate <= inccell2;

               when deccell =>
                  --incpc <= '1';
                  decptr <= '1';
                  nstate <= deccell2;

               when incdata =>
                  DATA_EN <= '1'; --mozna neni nutne
                  DATA_RDWR <= '0';
                  mux <= "01";
                  nstate <= incdata2;

               when decdata =>
                  
                  DATA_EN <= '1';
                  DATA_RDWR <= '0';
                  mux <= "10";
                  nstate <= decdata2;

               when print =>
                  incpc <= '1';
                  DATA_EN <= '1';
                  DATA_RDWR <= '1';
                  nstate <= print2;

               when entry =>
                  incpc <= '1';
                  nstate <= entry2;

               when lbracket =>
                  DATA_EN <= '1';
                  DATA_RDWR <= '1';
                  nstate <= lbracket2;

               when rbracket =>
                  DATA_EN <= '1';
                  DATA_RDWR <= '1';
                  nstate <= rbracket2;  

               when endnull =>
                  nstate <= endnull2; 
						
					when other =>
						nstate <= other2;
						
					when dollar =>
						nstate <= other2;
						
					when exmark =>
						nstate <= other2;

            end case;
				
         when inccell2 =>
			   incpc <= '1';
            --incptr <= '1';
				DATA_RDWR <= '1';
            DATA_EN <= '1';
            nstate <= init;
				
         when deccell2 =>
            DATA_RDWR <= '1';
				incpc <= '1';
				--decptr <= '1';
            DATA_EN <= '1';
            nstate <= init;
         
         when incdata2 =>
            DATA_RDWR <= '1';
            incpc <= '1';
            DATA_EN <= '1';
            nstate <= init;

         when decdata2 =>
            DATA_RDWR <= '1';
            incpc <= '1';
            DATA_EN <= '1';
            nstate <= init;

         when print2 =>
            if (OUT_BUSY = '1') then
               nstate <= print2;

            else
               --DATA_RDATA <= '0';
               OUT_DATA <= DATA_RDATA;
               OUT_WE <= '1';
               nstate <= init;
            end if;

         when lbracket2 =>
            incpc <= '1';
            if (DATA_RDATA = "00000000") then
               nstate <= skipmid;
            else
               nstate <= init;
            end if ;
                               
         when rbracket2 =>
            if (DATA_RDATA = "00000000") then
               incpc <= '1';
               nstate <= init;
            else
               decpc <= '1';
               nstate <= decloopmid;
            end if ;
			
			when  decloopmid =>
				CODE_EN <= '1';
				nstate <= decloop;
			
			when  skipmid =>
				CODE_EN <= '1';
				nstate <= skipmid;			
			
         when endnull2 =>
            nstate <= endnull2;

         when decloop =>
            if (instr = lbracket) then
               --CODE_EN <= '1';
               incpc <= '1';
               nstate <= init;
            else
               --CODE_EN <= '1';
               decpc <= '1';
               nstate <= decloopmid;
            end if;

         when skip =>
            if (instr = rbracket) then
               --CODE_EN <= '1';
               incpc <= '1';
               nstate <= init;
            else
               --CODE_EN <= '1';
               incpc <= '1';
               nstate <= skipmid;
         end if;

         when entry2 =>
				IN_REQ <= '1';	
				if(IN_VLD = '0') then
					nstate <= entry2;
				else
					mux <= "00";
					DATA_RDWR <= '1';
					DATA_EN <= '1';
					nstate <= init;
				end if;
				
			when other2 =>
				incpc <= '1';
				nstate <= init;

      end case;
   end process;


end behavioral;