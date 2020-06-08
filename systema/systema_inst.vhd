	component systema is
		port (
			button_mode_export : in std_logic_vector(1 downto 0) := (others => 'X'); -- export
			clk_clk            : in std_logic                    := 'X';             -- clk
			reset_reset_n      : in std_logic                    := 'X';             -- reset_n
			btn_op_export      : in std_logic_vector(1 downto 0) := (others => 'X')  -- export
		);
	end component systema;

	u0 : component systema
		port map (
			button_mode_export => CONNECTED_TO_button_mode_export, -- button_mode.export
			clk_clk            => CONNECTED_TO_clk_clk,            --         clk.clk
			reset_reset_n      => CONNECTED_TO_reset_reset_n,      --       reset.reset_n
			btn_op_export      => CONNECTED_TO_btn_op_export       --      btn_op.export
		);

