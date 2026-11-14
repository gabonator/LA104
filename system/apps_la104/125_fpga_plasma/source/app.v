// Plasma animation for AG1KLPQ48
// v = sin(x+a) + sin(y+b) + sin(x+y+c) mapped through rainbow palette
// Uses quarter-wave sine LUT (128x7 bits, 512-step), triangle-wave RGB palette
//
// SPI commands (5-byte, big-endian):
//   0x01 [xx] ...               : Read version (byte 1)
//   0x03 [xx] [xx] [xx] [xx]   : Read status (byte 1)
//   0x05 [00] [00] [00] [val]  : Set palette offset (8-bit, rotates hue)
//   0x06 [00] [00] [hi] [lo]   : Set width (9-bit, default 320)
//   0x07 [00] [00] [00] [val]  : Set height (8-bit, default 240)
//   0x08 [00] [00] [00] [val]  : Set clock divider (0=full speed, 3=slowest)
//   0x09 [00] [00] [00] [val]  : Set speed A (signed 8-bit, added to phase_a each frame)
//   0x0A [00] [00] [00] [val]  : Set speed B (signed 8-bit, added to phase_b each frame)
//   0x0B [00] [00] [00] [val]  : Set speed C (signed 8-bit, added to phase_c each frame)
//   0x10 [xx] ...               : Start animation (auto-repeating)
//   0x11 [xx] ...               : Release (tri-state all)

module app(clk, SCK, MOSI, MISO, SSEL, LCD_WR, LCD_DATA);

input clk;
input SCK, SSEL, MOSI;
output MISO;
inout LCD_WR;
inout [15:0] LCD_DATA;

// ---------------------------------------------------------------
// Quarter-wave sine LUT: 128 entries x 7 bits (0..127)
// Full 512-step sine reconstructed via quadrant mirroring + offset
// ---------------------------------------------------------------
function [6:0] sine_quarter;
    input [6:0] addr;
    begin
        case (addr)
            7'd0:   sine_quarter = 7'd0;
            7'd1:   sine_quarter = 7'd2;
            7'd2:   sine_quarter = 7'd3;
            7'd3:   sine_quarter = 7'd5;
            7'd4:   sine_quarter = 7'd6;
            7'd5:   sine_quarter = 7'd8;
            7'd6:   sine_quarter = 7'd9;
            7'd7:   sine_quarter = 7'd11;
            7'd8:   sine_quarter = 7'd12;
            7'd9:   sine_quarter = 7'd14;
            7'd10:  sine_quarter = 7'd16;
            7'd11:  sine_quarter = 7'd17;
            7'd12:  sine_quarter = 7'd19;
            7'd13:  sine_quarter = 7'd20;
            7'd14:  sine_quarter = 7'd22;
            7'd15:  sine_quarter = 7'd23;
            7'd16:  sine_quarter = 7'd25;
            7'd17:  sine_quarter = 7'd26;
            7'd18:  sine_quarter = 7'd28;
            7'd19:  sine_quarter = 7'd29;
            7'd20:  sine_quarter = 7'd31;
            7'd21:  sine_quarter = 7'd32;
            7'd22:  sine_quarter = 7'd34;
            7'd23:  sine_quarter = 7'd35;
            7'd24:  sine_quarter = 7'd37;
            7'd25:  sine_quarter = 7'd38;
            7'd26:  sine_quarter = 7'd40;
            7'd27:  sine_quarter = 7'd41;
            7'd28:  sine_quarter = 7'd43;
            7'd29:  sine_quarter = 7'd44;
            7'd30:  sine_quarter = 7'd46;
            7'd31:  sine_quarter = 7'd47;
            7'd32:  sine_quarter = 7'd49;
            7'd33:  sine_quarter = 7'd50;
            7'd34:  sine_quarter = 7'd51;
            7'd35:  sine_quarter = 7'd53;
            7'd36:  sine_quarter = 7'd54;
            7'd37:  sine_quarter = 7'd56;
            7'd38:  sine_quarter = 7'd57;
            7'd39:  sine_quarter = 7'd58;
            7'd40:  sine_quarter = 7'd60;
            7'd41:  sine_quarter = 7'd61;
            7'd42:  sine_quarter = 7'd63;
            7'd43:  sine_quarter = 7'd64;
            7'd44:  sine_quarter = 7'd65;
            7'd45:  sine_quarter = 7'd67;
            7'd46:  sine_quarter = 7'd68;
            7'd47:  sine_quarter = 7'd69;
            7'd48:  sine_quarter = 7'd71;
            7'd49:  sine_quarter = 7'd72;
            7'd50:  sine_quarter = 7'd73;
            7'd51:  sine_quarter = 7'd74;
            7'd52:  sine_quarter = 7'd76;
            7'd53:  sine_quarter = 7'd77;
            7'd54:  sine_quarter = 7'd78;
            7'd55:  sine_quarter = 7'd79;
            7'd56:  sine_quarter = 7'd81;
            7'd57:  sine_quarter = 7'd82;
            7'd58:  sine_quarter = 7'd83;
            7'd59:  sine_quarter = 7'd84;
            7'd60:  sine_quarter = 7'd85;
            7'd61:  sine_quarter = 7'd86;
            7'd62:  sine_quarter = 7'd88;
            7'd63:  sine_quarter = 7'd89;
            7'd64:  sine_quarter = 7'd90;
            7'd65:  sine_quarter = 7'd91;
            7'd66:  sine_quarter = 7'd92;
            7'd67:  sine_quarter = 7'd93;
            7'd68:  sine_quarter = 7'd94;
            7'd69:  sine_quarter = 7'd95;
            7'd70:  sine_quarter = 7'd96;
            7'd71:  sine_quarter = 7'd97;
            7'd72:  sine_quarter = 7'd98;
            7'd73:  sine_quarter = 7'd99;
            7'd74:  sine_quarter = 7'd100;
            7'd75:  sine_quarter = 7'd101;
            7'd76:  sine_quarter = 7'd102;
            7'd77:  sine_quarter = 7'd103;
            7'd78:  sine_quarter = 7'd104;
            7'd79:  sine_quarter = 7'd105;
            7'd80:  sine_quarter = 7'd106;
            7'd81:  sine_quarter = 7'd106;
            7'd82:  sine_quarter = 7'd107;
            7'd83:  sine_quarter = 7'd108;
            7'd84:  sine_quarter = 7'd109;
            7'd85:  sine_quarter = 7'd110;
            7'd86:  sine_quarter = 7'd111;
            7'd87:  sine_quarter = 7'd111;
            7'd88:  sine_quarter = 7'd112;
            7'd89:  sine_quarter = 7'd113;
            7'd90:  sine_quarter = 7'd113;
            7'd91:  sine_quarter = 7'd114;
            7'd92:  sine_quarter = 7'd115;
            7'd93:  sine_quarter = 7'd115;
            7'd94:  sine_quarter = 7'd116;
            7'd95:  sine_quarter = 7'd117;
            7'd96:  sine_quarter = 7'd117;
            7'd97:  sine_quarter = 7'd118;
            7'd98:  sine_quarter = 7'd118;
            7'd99:  sine_quarter = 7'd119;
            7'd100: sine_quarter = 7'd120;
            7'd101: sine_quarter = 7'd120;
            7'd102: sine_quarter = 7'd121;
            7'd103: sine_quarter = 7'd121;
            7'd104: sine_quarter = 7'd122;
            7'd105: sine_quarter = 7'd122;
            7'd106: sine_quarter = 7'd122;
            7'd107: sine_quarter = 7'd123;
            7'd108: sine_quarter = 7'd123;
            7'd109: sine_quarter = 7'd124;
            7'd110: sine_quarter = 7'd124;
            7'd111: sine_quarter = 7'd124;
            7'd112: sine_quarter = 7'd125;
            7'd113: sine_quarter = 7'd125;
            7'd114: sine_quarter = 7'd125;
            7'd115: sine_quarter = 7'd125;
            7'd116: sine_quarter = 7'd126;
            7'd117: sine_quarter = 7'd126;
            7'd118: sine_quarter = 7'd126;
            7'd119: sine_quarter = 7'd126;
            7'd120: sine_quarter = 7'd126;
            7'd121: sine_quarter = 7'd127;
            7'd122: sine_quarter = 7'd127;
            7'd123: sine_quarter = 7'd127;
            7'd124: sine_quarter = 7'd127;
            7'd125: sine_quarter = 7'd127;
            7'd126: sine_quarter = 7'd127;
            7'd127: sine_quarter = 7'd127;
            default: sine_quarter = 7'd0;
        endcase
    end
endfunction

// Full 9-bit sine: input 0..511 -> output 0..255 (128 = zero crossing)
// Uses bit manipulation to avoid lpm_add_sub (not available on AG1KLPQ48)
// Positive half: {1, qv} = 128..255, Negative half: {0, ~qv} = 127..0
function [7:0] sine;
    input [8:0] angle;
    reg [6:0] qa;
    reg [6:0] qv;
    begin
        qa = angle[7] ? ~angle[6:0] : angle[6:0];
        qv = sine_quarter(qa);
        sine = angle[8] ? {1'b0, ~qv} : {1'b1, qv};
    end
endfunction

// Triangle wave: input 0..255 -> output 0..127..0
function [6:0] triangle;
    input [7:0] x;
    begin
        triangle = x[7] ? ~x[6:0] : x[6:0];
    end
endfunction

// ---------------------------------------------------------------
// Clock domain crossing
// ---------------------------------------------------------------
reg [2:0] SCKr;  always @(posedge clk) SCKr <= {SCKr[1:0], SCK};
wire SCK_risingedge  = (SCKr[2:1] == 2'b01);
wire SCK_fallingedge = (SCKr[2:1] == 2'b10);

reg [2:0] SSELr; always @(posedge clk) SSELr <= {SSELr[1:0], SSEL};
wire SSEL_active       = ~SSELr[1];
wire SSEL_startmessage = (SSELr[2:1] == 2'b10);

reg [1:0] MOSIr; always @(posedge clk) MOSIr <= {MOSIr[0], MOSI};
wire MOSI_data = MOSIr[1];

// ---------------------------------------------------------------
// Tri-state outputs
// ---------------------------------------------------------------
reg data_active;
reg wr_active;
reg wr_out;
reg [15:0] data_out;

assign LCD_WR   = wr_active  ? wr_out   : 1'bz;
assign LCD_DATA = data_active ? data_out : 16'bz;

// ---------------------------------------------------------------
// Fill state machine
// ---------------------------------------------------------------
reg        filling;
reg        fill_done;
reg [8:0]  x_counter;
reg [7:0]  y_counter;
reg [9:0]  frame_cnt;
reg [1:0]  clk_div;
reg [1:0]  clk_div_max;

reg [8:0]  width_reg;
reg [7:0]  height_reg;

wire [7:0] status = filling ? 8'h01 : (fill_done ? 8'h02 : 8'h00);

// ---------------------------------------------------------------
// Plasma computation
// ---------------------------------------------------------------
wire [8:0] pix_x = x_counter[8:0];
wire [8:0] pix_y = {1'b0, y_counter};

// Phase accumulators — 17-bit (upper 9 = integer, lower 8 = fractional)
// Incremented by speed registers each frame
reg [16:0] phase_a;
reg [16:0] phase_b;
reg [16:0] phase_c;

// Speed registers (signed 8-bit, added to 16-bit phase each frame → 8.8 fixed point)
// Set via SPI 0x09/0x0A/0x0B. E.g. speed=1 → phase advances 1/256 per frame
reg signed [7:0] speed_a;
reg signed [7:0] speed_b;
reg signed [7:0] speed_c;

// Three sine components — use upper 9 bits of phase (integer part)
wire [7:0] s1 = sine(pix_x + phase_a[16:8]);
wire [7:0] s2 = sine(pix_y + phase_b[16:8]);
wire [7:0] s3 = sine(pix_x + pix_y + phase_c[16:8]);

// Sum wraps naturally to 8 bits
reg [7:0] palette_offset;
wire [7:0] plasma_idx = s1 + s2 + s3 + palette_offset;

// Rainbow palette: triangle waves offset by 120 degrees
wire [6:0] r_tri = triangle(plasma_idx);
wire [6:0] g_tri = triangle(plasma_idx + 8'd85);
wire [6:0] b_tri = triangle(plasma_idx + 8'd170);

wire [15:0] pixel_color = {r_tri[6:2], g_tri[6:1], b_tri[6:2]};

// ---------------------------------------------------------------
// SPI state
// ---------------------------------------------------------------
reg [2:0] bitcnt;
reg [7:0] byte_data_received;
reg [7:0] command;
reg [1:0] byte_offset;
reg [7:0] data_send;
reg cmd_start;
reg cmd_release;

// ---------------------------------------------------------------
// Main logic
// ---------------------------------------------------------------
always @(posedge clk)
begin
    // --- Fill state machine (2-phase per pixel, auto-repeat) ---
    if (cmd_start && !filling)
    begin
        data_active <= 1'b1;
        wr_active   <= 1'b1;
        wr_out      <= 1'b1;
        filling     <= 1'b1;
        fill_done   <= 1'b0;
        x_counter   <= 9'd0;
        y_counter   <= 8'd0;
        frame_cnt   <= 10'd0;
        clk_div     <= 2'd0;
        phase_a     <= 17'd0;
        phase_b     <= 17'd0;
        phase_c     <= 17'd0;
        cmd_start   <= 1'b0;
        if (width_reg == 9'd0)  width_reg  <= 9'd320;
        if (height_reg == 8'd0) height_reg <= 8'd240;
        if (speed_a == 8'd0 && speed_b == 8'd0 && speed_c == 8'd0)
        begin
            speed_a <= 8'd80;
            speed_b <= -8'sd50;
            speed_c <= 8'd120;
        end
    end
    else if (cmd_release)
    begin
        data_active <= 1'b0;
        wr_active   <= 1'b0;
        filling     <= 1'b0;
        fill_done   <= 1'b0;
        cmd_release <= 1'b0;
    end
    else if (filling)
    begin
        if (clk_div == clk_div_max)
        begin
            clk_div <= 2'd0;
        end
        else
        begin
            clk_div <= clk_div + 2'd1;
        end
        if (clk_div == clk_div_max)
        begin
            if (wr_out == 1'b1)
            begin
                data_out <= pixel_color;
                wr_out   <= 1'b0;
            end
            else
            begin
                wr_out <= 1'b1;
                if (y_counter == height_reg - 8'd1 && x_counter == width_reg - 9'd1)
                begin
                    y_counter <= 8'd0;
                    x_counter <= 9'd0;
                    frame_cnt <= frame_cnt + 10'd1;
                    phase_a <= phase_a + {{7{speed_a[7]}}, speed_a, 2'b00};
                    phase_b <= phase_b + {{7{speed_b[7]}}, speed_b, 2'b00};
                    phase_c <= phase_c + {{7{speed_c[7]}}, speed_c, 2'b00};
                end
                else if (y_counter == height_reg - 8'd1)
                begin
                    y_counter <= 8'd0;
                    x_counter <= x_counter + 9'd1;
                end
                else
                    y_counter <= y_counter + 8'd1;
            end
        end
    end

    // --- SPI receive logic ---
    if (~SSEL_active)
    begin
        bitcnt      <= 3'b000;
        command     <= 8'hff;
        data_send   <= 8'h00;
        byte_offset <= 2'd0;
    end
    else if (SCK_risingedge)
    begin
        bitcnt <= bitcnt + 3'b001;
        byte_data_received <= {byte_data_received[6:0], MOSI_data};

        if (bitcnt == 3'b111)
        begin
            case (command)
                8'hff: // Command byte
                begin
                    command     <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= 2'd0;
                    case ({byte_data_received[6:0], MOSI_data})
                        8'h01:   data_send <= 8'h06;  // Version 6 (plasma)
                        8'h03:   data_send <= status;
                        8'h10:   begin cmd_start   <= 1'b1; data_send <= 8'hAA; end
                        8'h11:   begin cmd_release  <= 1'b1; data_send <= 8'hAA; end
                        default: data_send <= 8'hAA;
                    endcase
                end

                8'h05: // Set palette offset (8-bit)
                begin
                    if (byte_offset == 2'd3)
                        palette_offset <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h06: // Set width
                begin
                    if (byte_offset == 2'd3)
                        width_reg <= {byte_data_received[0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h07: // Set height
                begin
                    if (byte_offset == 2'd3)
                        height_reg <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h08: // Set clock divider
                begin
                    if (byte_offset == 2'd3)
                        clk_div_max <= {byte_data_received[0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h09: // Set speed A (signed 8-bit)
                begin
                    if (byte_offset == 2'd3)
                        speed_a <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h0A: // Set speed B (signed 8-bit)
                begin
                    if (byte_offset == 2'd3)
                        speed_b <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h0B: // Set speed C (signed 8-bit)
                begin
                    if (byte_offset == 2'd3)
                        speed_c <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h03: // Status read
                begin
                    data_send   <= 8'h00;
                    byte_offset <= byte_offset + 2'd1;
                end

                default:
                    data_send <= 8'hFF;
            endcase
        end
    end
end

// ---------------------------------------------------------------
// SPI transmit
// ---------------------------------------------------------------
reg [7:0] byte_data_sent;

always @(posedge clk)
if (SSEL_active)
begin
    if (SSEL_startmessage)
        byte_data_sent <= 8'h00;
    else if (SCK_fallingedge)
    begin
        if (bitcnt == 3'b000)
            byte_data_sent <= data_send;
        else
            byte_data_sent <= {byte_data_sent[6:0], 1'b0};
    end
end

assign MISO = byte_data_sent[7];

endmodule
