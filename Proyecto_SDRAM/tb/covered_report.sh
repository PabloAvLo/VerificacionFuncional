echo 'Running coverage analysis'
covered score -I . -t tb -v tb.v -v ../rtl/top/sdrc_top.v -v ../rtl/wb2sdrc/wb2sdrc.v -v ../rtl/core/sdrc_bank_ctl.v -v ../rtl/core/sdrc_bank_fsm.v -v ../rtl/core/sdrc_req_gen.v -v ../rtl/core/sdrc_xfr_ctl.v -v ../rtl/core/sdrc_bs_convert.v -v ../rtl/lib/async_fifo.v -v ../rtl/core/sdrc_core.v -e mt48lc8m8a2 -vcd verilog.vcd -o cov5.cdd
echo 'Generating coverage report'
covered report cov5.cdd