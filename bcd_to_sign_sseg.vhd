library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bcd_to_sign_sseg is port(
	BCD : in  std_logic_vector(3 downto 0);
	sseg : out std_logic_vector(7 downto 0));
end entity;

architecture arch_bcd of bcd_to_sign_sseg is
begin
process(BCD)
	begin
	case BCD is
		when "0000" => --positive
			sseg <= "11111111";
		when "0001" => --negative
			sseg <= "10111111";
		when others =>
			sseg <= "00000000";
	end case;
end process;
end architecture;