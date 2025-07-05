onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /t0/t1/sys_clk
add wave -noupdate /t0/t1/sys_rst_n
add wave -noupdate /t0/t1/en_spi
add wave -noupdate /t0/t1/cs
add wave -noupdate /t0/t1/sclk
add wave -noupdate /t0/t1/mosi
add wave -noupdate /t0/t1/miso
add wave -noupdate /t0/t1/wr_ack
add wave -noupdate /t0/t1/re_ack
add wave -noupdate /t0/t1/ins_spi_master/data_tx
add wave -noupdate /t0/t1/slave_rx
add wave -noupdate /t0/t1/ins_spi_slave/data_tx
add wave -noupdate /t0/t1/master_rx
add wave -noupdate /t0/t1/cs_ctrl
add wave -noupdate /t0/t1/ins_spi_master/CPOL
add wave -noupdate /t0/t1/ins_spi_master/CPHA
add wave -noupdate /t0/t1/ins_spi_master/data_rx
add wave -noupdate /t0/t1/ins_spi_master/mosi
add wave -noupdate /t0/t1/ins_spi_master/mosi_shift
add wave -noupdate /t0/t1/ins_spi_master/miso
add wave -noupdate /t0/t1/ins_spi_master/miso_shift
add wave -noupdate /t0/t1/ins_spi_master/state
add wave -noupdate -radix unsigned /t0/t1/ins_spi_master/cnt_clk
add wave -noupdate /t0/t1/ins_spi_master/sclk
add wave -noupdate -radix unsigned /t0/t1/ins_spi_master/cnt_sclk_edge
add wave -noupdate /t0/t1/ins_spi_slave/CPOL
add wave -noupdate /t0/t1/ins_spi_slave/CPHA
add wave -noupdate /t0/t1/ins_spi_slave/data_rx
add wave -noupdate /t0/t1/ins_spi_slave/mosi
add wave -noupdate /t0/t1/ins_spi_slave/mosi_shift
add wave -noupdate /t0/t1/ins_spi_slave/miso
add wave -noupdate /t0/t1/ins_spi_slave/miso_shift
add wave -noupdate /t0/t1/ins_spi_slave/state
add wave -noupdate /t0/t1/ins_spi_slave/sclk
add wave -noupdate -radix unsigned /t0/t1/ins_spi_slave/cnt_sclk_edge
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {7098 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 236
configure wave -valuecolwidth 60
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {32837 ps}
