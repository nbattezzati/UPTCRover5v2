// (C) 2001-2014 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// (C) 2001-2013 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/13.1up/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#1 $
// $Revision: #1 $
// $Date: 2013/11/05 $
// $Author: swbranch $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module NIOS_mm_interconnect_0_addr_router_001_default_decode
  #(
     parameter DEFAULT_CHANNEL = 1,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 25 
   )
  (output [93 - 89 : 0] default_destination_id,
   output [31-1 : 0] default_wr_channel,
   output [31-1 : 0] default_rd_channel,
   output [31-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[93 - 89 : 0];

  generate begin : default_decode
    if (DEFAULT_CHANNEL == -1) begin
      assign default_src_channel = '0;
    end
    else begin
      assign default_src_channel = 31'b1 << DEFAULT_CHANNEL;
    end
  end
  endgenerate

  generate begin : default_decode_rw
    if (DEFAULT_RD_CHANNEL == -1) begin
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin
      assign default_wr_channel = 31'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 31'b1 << DEFAULT_RD_CHANNEL;
    end
  end
  endgenerate

endmodule


module NIOS_mm_interconnect_0_addr_router_001
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [107-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [107-1    : 0] src_data,
    output reg [31-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 62;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 93;
    localparam PKT_DEST_ID_L = 89;
    localparam PKT_PROTECTION_H = 97;
    localparam PKT_PROTECTION_L = 95;
    localparam ST_DATA_W = 107;
    localparam ST_CHANNEL_W = 31;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 65;
    localparam PKT_TRANS_READ  = 66;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h4000000 - 64'h2000000); 
    localparam PAD1 = log2ceil(64'h4001800 - 64'h4001000); 
    localparam PAD2 = log2ceil(64'h4002000 - 64'h4001800); 
    localparam PAD3 = log2ceil(64'h4002020 - 64'h4002000); 
    localparam PAD4 = log2ceil(64'h4002040 - 64'h4002020); 
    localparam PAD5 = log2ceil(64'h4002060 - 64'h4002040); 
    localparam PAD6 = log2ceil(64'h4002080 - 64'h4002060); 
    localparam PAD7 = log2ceil(64'h40020a0 - 64'h4002080); 
    localparam PAD8 = log2ceil(64'h40020c0 - 64'h40020a0); 
    localparam PAD9 = log2ceil(64'h40020e0 - 64'h40020c0); 
    localparam PAD10 = log2ceil(64'h4002100 - 64'h40020e0); 
    localparam PAD11 = log2ceil(64'h4002120 - 64'h4002100); 
    localparam PAD12 = log2ceil(64'h4002140 - 64'h4002120); 
    localparam PAD13 = log2ceil(64'h4002160 - 64'h4002140); 
    localparam PAD14 = log2ceil(64'h4002180 - 64'h4002160); 
    localparam PAD15 = log2ceil(64'h4002190 - 64'h4002180); 
    localparam PAD16 = log2ceil(64'h40021a0 - 64'h4002190); 
    localparam PAD17 = log2ceil(64'h40021b0 - 64'h40021a0); 
    localparam PAD18 = log2ceil(64'h40021c0 - 64'h40021b0); 
    localparam PAD19 = log2ceil(64'h40021d0 - 64'h40021c0); 
    localparam PAD20 = log2ceil(64'h40021e0 - 64'h40021d0); 
    localparam PAD21 = log2ceil(64'h40021f0 - 64'h40021e0); 
    localparam PAD22 = log2ceil(64'h4002200 - 64'h40021f0); 
    localparam PAD23 = log2ceil(64'h4002210 - 64'h4002200); 
    localparam PAD24 = log2ceil(64'h4002220 - 64'h4002210); 
    localparam PAD25 = log2ceil(64'h4002230 - 64'h4002220); 
    localparam PAD26 = log2ceil(64'h4002240 - 64'h4002230); 
    localparam PAD27 = log2ceil(64'h4002250 - 64'h4002240); 
    localparam PAD28 = log2ceil(64'h4002258 - 64'h4002250); 
    localparam PAD29 = log2ceil(64'h4002260 - 64'h4002258); 
    localparam PAD30 = log2ceil(64'h4002262 - 64'h4002260); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h4002262;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;

    localparam RG = RANGE_ADDR_WIDTH-1;
    localparam REAL_ADDRESS_RANGE = OPTIMIZED_ADDR_H - PKT_ADDR_L;

      reg [PKT_ADDR_W-1 : 0] address;
      always @* begin
        address = {PKT_ADDR_W{1'b0}};
        address [REAL_ADDRESS_RANGE:0] = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];
      end   

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;
    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [31-1 : 0] default_src_channel;




    // -------------------------------------------------------
    // Write and read transaction signals
    // -------------------------------------------------------
    wire read_transaction;
    assign read_transaction  = sink_data[PKT_TRANS_READ];


    NIOS_mm_interconnect_0_addr_router_001_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_wr_channel   (),
      .default_rd_channel   (),
      .default_src_channel  (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;
        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;

        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

    // ( 0x2000000 .. 0x4000000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 27'h2000000   ) begin
            src_channel = 31'b0000000000000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 25;
    end

    // ( 0x4001000 .. 0x4001800 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 27'h4001000   ) begin
            src_channel = 31'b0000000000000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
    end

    // ( 0x4001800 .. 0x4002000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 27'h4001800   ) begin
            src_channel = 31'b0000000000000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0x4002000 .. 0x4002020 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 27'h4002000   ) begin
            src_channel = 31'b1000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
    end

    // ( 0x4002020 .. 0x4002040 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 27'h4002020   ) begin
            src_channel = 31'b0100000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x4002040 .. 0x4002060 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 27'h4002040   ) begin
            src_channel = 31'b0010000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0x4002060 .. 0x4002080 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 27'h4002060   ) begin
            src_channel = 31'b0001000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0x4002080 .. 0x40020a0 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 27'h4002080   ) begin
            src_channel = 31'b0000100000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0x40020a0 .. 0x40020c0 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 27'h40020a0   ) begin
            src_channel = 31'b0000010000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0x40020c0 .. 0x40020e0 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 27'h40020c0   ) begin
            src_channel = 31'b0000001000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0x40020e0 .. 0x4002100 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 27'h40020e0   ) begin
            src_channel = 31'b0000000100000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0x4002100 .. 0x4002120 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 27'h4002100   ) begin
            src_channel = 31'b0000000010000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 27;
    end

    // ( 0x4002120 .. 0x4002140 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 27'h4002120   ) begin
            src_channel = 31'b0000000001000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0x4002140 .. 0x4002160 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 27'h4002140   ) begin
            src_channel = 31'b0000000000000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0x4002160 .. 0x4002180 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 27'h4002160   ) begin
            src_channel = 31'b0000000000000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 30;
    end

    // ( 0x4002180 .. 0x4002190 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 27'h4002180  && read_transaction  ) begin
            src_channel = 31'b0000000000100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 17;
    end

    // ( 0x4002190 .. 0x40021a0 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 27'h4002190  && read_transaction  ) begin
            src_channel = 31'b0000000000010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0x40021a0 .. 0x40021b0 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 27'h40021a0   ) begin
            src_channel = 31'b0000000000001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0x40021b0 .. 0x40021c0 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 27'h40021b0   ) begin
            src_channel = 31'b0000000000000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0x40021c0 .. 0x40021d0 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 27'h40021c0   ) begin
            src_channel = 31'b0000000000000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 24;
    end

    // ( 0x40021d0 .. 0x40021e0 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 27'h40021d0   ) begin
            src_channel = 31'b0000000000000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 23;
    end

    // ( 0x40021e0 .. 0x40021f0 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 27'h40021e0   ) begin
            src_channel = 31'b0000000000000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

    // ( 0x40021f0 .. 0x4002200 )
    if ( {address[RG:PAD22],{PAD22{1'b0}}} == 27'h40021f0  && read_transaction  ) begin
            src_channel = 31'b0000000000000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x4002200 .. 0x4002210 )
    if ( {address[RG:PAD23],{PAD23{1'b0}}} == 27'h4002200   ) begin
            src_channel = 31'b0000000000000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0x4002210 .. 0x4002220 )
    if ( {address[RG:PAD24],{PAD24{1'b0}}} == 27'h4002210   ) begin
            src_channel = 31'b0000000000000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 29;
    end

    // ( 0x4002220 .. 0x4002230 )
    if ( {address[RG:PAD25],{PAD25{1'b0}}} == 27'h4002220   ) begin
            src_channel = 31'b0000000000000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 20;
    end

    // ( 0x4002230 .. 0x4002240 )
    if ( {address[RG:PAD26],{PAD26{1'b0}}} == 27'h4002230  && read_transaction  ) begin
            src_channel = 31'b0000000000000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 26;
    end

    // ( 0x4002240 .. 0x4002250 )
    if ( {address[RG:PAD27],{PAD27{1'b0}}} == 27'h4002240   ) begin
            src_channel = 31'b0000000000000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0x4002250 .. 0x4002258 )
    if ( {address[RG:PAD28],{PAD28{1'b0}}} == 27'h4002250   ) begin
            src_channel = 31'b0000000000000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0x4002258 .. 0x4002260 )
    if ( {address[RG:PAD29],{PAD29{1'b0}}} == 27'h4002258  && read_transaction  ) begin
            src_channel = 31'b0000000000000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 28;
    end

    // ( 0x4002260 .. 0x4002262 )
    if ( {address[RG:PAD30],{PAD30{1'b0}}} == 27'h4002260   ) begin
            src_channel = 31'b0000000000000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule


