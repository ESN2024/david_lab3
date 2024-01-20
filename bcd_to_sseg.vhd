library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bcd_to_sseg is port(
	BCD : in  std_logic_vector(3 downto 0);
	sseg : out std_logic_vector(7 downto 0));
end entity;

architecture arch_bcd of bcd_to_sseg is
begin
process(BCD)
	begin
	case BCD is
		when "0000" =>
--			sseg <= "00111111";
			sseg <= "11000000";
		when "0001" =>
--			sseg <= "00000110";
			sseg <= "11111001";
		when "0010" =>
--			sseg <= "01011011";
			sseg <= "10100100";
		when "0011" =>
--			sseg <= "01001111";
			sseg <= "10110000";
		when "0100" =>
--			sseg <= "01100110";
			sseg <= "10011001";
		when "0101" =>
--			sseg <= "01101101";
			sseg <= "10010010";
		when "0110" =>
--			sseg <= "01111101";
			sseg <= "10000010";
		when "0111" =>
--			sseg <= "00000111";
			sseg <= "11111000";
		when "1000" =>
--			sseg <= "01111111";
			sseg <= "10000000";
		when "1001" =>
--			sseg <= "01101111";
			sseg <= "10010000";
		when others =>
--			sseg <= "11111111";
			sseg <= "00000000";
	end case;
end process;
end architecture;