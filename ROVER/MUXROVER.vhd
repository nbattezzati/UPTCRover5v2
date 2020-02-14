-- Universidad Pedagógica y Tecnológica de Colombia UPTC
-- Escuela de Ingeniería Electrónica
-- Autor : Sergio Fernando Barrera Molano
-- Fecha : 07/Nov/2012
-- Descripción: MUX 2-1 N bits.

-- Este multiplexor está orientado a evitar interferencia de señales en los sensores ultrasonicos


--Librerias de uso general

library ieee;					
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity MUXROVER is
		PORT 	(
					D1,D2,D3,D4,XBEE_R,GPS_R		: IN STD_LOGIC;					
					GPS_T,XBEE_T						: OUT STD_LOGIC;
					
					GPS_N_R, XBEE_N_R					: IN STD_LOGIC;					
					GPS_N_T, XBEE_N_T					: OUT STD_LOGIC;
					
					SELECTOR								: IN STD_LOGIC_VECTOR (2 DOWNTO 0)
				);
end entity;
architecture Comportamental of MUXROVER is		-- Arquitectura del MUX, se describira de forma Comportamental
begin	
	process (D1,D2,D3,D4,XBEE_R,GPS_R,GPS_N_R, XBEE_N_R,SELECTOR)			-- Process de Control del MUX
	begin
		CASE SELECTOR is			-- Dependiendo del estado actual y las entradas Se elige El estado FUTURO.
			When "000" =>
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= XBEE_N_R;
				XBEE_N_T	 	<= XBEE_R;
				
			When "001" =>					
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= D1;
				XBEE_N_T	 	<= XBEE_R;
	
	      When "010" =>
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= D2;
				XBEE_N_T	 	<= XBEE_R;	
			
			When "011" =>	
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= D3;
				XBEE_N_T	 	<= XBEE_R;	
			
			When "100" =>
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= D4;
				XBEE_N_T	 	<= XBEE_R;	
			
			When "101" =>				
				GPS_N_T		<= '1';
				XBEE_N_T		<= '1';
				
				XBEE_T 		<= GPS_R;
				GPS_T	 		<= XBEE_R;						
			
			When Others => 			-- Condicion de Seguridad
				GPS_T 		<= GPS_N_R;				
				GPS_N_T 		<= GPS_R;
				
				XBEE_T 		<= XBEE_N_R;
				XBEE_N_T	 	<= XBEE_R;
				
		End CASE;
	end process;
end architecture;