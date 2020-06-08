// systema_tb.v

// Generated using ACDS version 18.1 625

`timescale 1 ps / 1 ps
module systema_tb (
	);

	wire        systema_inst_clk_bfm_clk_clk;                // systema_inst_clk_bfm:clk -> [systema_inst:clk_clk, systema_inst_reset_bfm:clk]
	wire  [1:0] systema_inst_btn_op_bfm_conduit_export;      // systema_inst_btn_op_bfm:sig_export -> systema_inst:btn_op_export
	wire  [1:0] systema_inst_button_mode_bfm_conduit_export; // systema_inst_button_mode_bfm:sig_export -> systema_inst:button_mode_export
	wire        systema_inst_reset_bfm_reset_reset;          // systema_inst_reset_bfm:reset -> systema_inst:reset_reset_n

	systema systema_inst (
		.btn_op_export      (systema_inst_btn_op_bfm_conduit_export),      //      btn_op.export
		.button_mode_export (systema_inst_button_mode_bfm_conduit_export), // button_mode.export
		.clk_clk            (systema_inst_clk_bfm_clk_clk),                //         clk.clk
		.reset_reset_n      (systema_inst_reset_bfm_reset_reset)           //       reset.reset_n
	);

	altera_conduit_bfm systema_inst_btn_op_bfm (
		.sig_export (systema_inst_btn_op_bfm_conduit_export)  // conduit.export
	);

	altera_conduit_bfm systema_inst_button_mode_bfm (
		.sig_export (systema_inst_button_mode_bfm_conduit_export)  // conduit.export
	);

	altera_avalon_clock_source #(
		.CLOCK_RATE (50000000),
		.CLOCK_UNIT (1)
	) systema_inst_clk_bfm (
		.clk (systema_inst_clk_bfm_clk_clk)  // clk.clk
	);

	altera_avalon_reset_source #(
		.ASSERT_HIGH_RESET    (0),
		.INITIAL_RESET_CYCLES (50)
	) systema_inst_reset_bfm (
		.reset (systema_inst_reset_bfm_reset_reset), // reset.reset_n
		.clk   (systema_inst_clk_bfm_clk_clk)        //   clk.clk
	);

endmodule
