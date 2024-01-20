library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lab3vhd2 is port(
	clk 				: in  	std_logic := '0';
	reset_n 			: in  	std_logic := '0';
	SW  				: in  	std_logic_vector(9 downto 0);
	sseg0 			: out 	std_logic_vector(7 downto 0);
	sseg1 			: out 	std_logic_vector(7 downto 0);
	sseg2 			: out 	std_logic_vector(7 downto 0);
	BP					: in  	std_logic_vector(1 downto 0);
	i2c_sda			: inout	std_logic;
	i2c_scl			: inout	std_logic;
	i2c_gsensor_en : out 	std_logic := '1';
	i2c_gsensor_sdo: out		std_logic := '1');
end entity;


architecture arch_lab3 of lab3vhd2 is
	component lab3v1 is port(
		clk_clk										: in  std_logic                     := 'X';
		pio_0_external_connection_export 	: in  std_logic_vector(9 downto 0)  := (others => 'X');
		pio_1_external_connection_export 	: out std_logic_vector(11 downto 0) := (others => 'X');
		pio_2_external_connection_export 	: in  std_logic_vector(1 downto 0)	:= (others => 'X');
		opencores_i2c_0_export_0_scl_pad_io : inout std_logic                   := 'X';
      opencores_i2c_0_export_0_sda_pad_io : inout std_logic                   := 'X';
      reset_reset_n  							: in  std_logic                     := 'X');
	end component;
	 
	component bcd_to_sseg is port(
		BCD  : in  std_logic_vector(3 downto 0);
		sseg : out std_logic_vector(7 downto 0));
	end component;
	
	component bcd_to_dot_sseg is port(
		BCD  : in  std_logic_vector(3 downto 0);
		sseg : out std_logic_vector(7 downto 0));
	end component;
	
	component bcd_to_sign_sseg is port(
		BCD  : in  std_logic_vector(3 downto 0);
		sseg : out std_logic_vector(7 downto 0));
	end component;
	
	signal s_pio_1 : std_logic_vector(11 downto 0);

begin
	u0 : component lab3v1 port map(
		clk_clk 										=> clk,
		pio_0_external_connection_export 	=> SW,
		pio_1_external_connection_export 	=> s_pio_1,
		pio_2_external_connection_export 	=> BP,
		opencores_i2c_0_export_0_scl_pad_io => i2c_scl,
      opencores_i2c_0_export_0_sda_pad_io => i2c_sda,
		reset_reset_n								=> reset_n);
		
	u1 : component bcd_to_sseg port map(
		BCD  => s_pio_1(3 downto 0),
		sseg => sseg0);
		
	u2 : component bcd_to_dot_sseg port map(
		BCD  => s_pio_1(7 downto 4),
		sseg => sseg1);
		
	u3 : component bcd_to_sign_sseg port map(
		BCD  => s_pio_1(11 downto 8),
		sseg => sseg2);
end architecture;