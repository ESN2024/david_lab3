library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bcd_to_dot_sseg is port(
	BCD : in  std_logic_vector(3 downto 0);
	sseg : out std_logic_vector(7 downto 0));
end entity;

architecture arch_bcd of bcd_to_dot_sseg is
begin
process(BCD)
	begin
	case BCD is
		when "0000" =>
			sseg <= "01000000";
		when "0001" =>
			sseg <= "01111001";
		when "0010" =>
			sseg <= "00100100";
		when "0011" =>
			sseg <= "00110000";
		when "0100" =>
			sseg <= "00011001";
		when "0101" =>
			sseg <= "00010010";
		when "0110" =>
			sseg <= "00000010";
		when "0111" =>
			sseg <= "01111000";
		when "1000" =>
			sseg <= "00000000";
		when "1001" =>
			sseg <= "00010000";
		when others =>
			sseg <= "00000000";
	end case;
end process;
end architecture;