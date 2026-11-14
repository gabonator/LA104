// Parallax checkerboard animation for AG1KLPQ48
// Ported from vga-playground.com VGA design by Renaldas Zioma
// 5 layers of scrolling checkerboards with dithered transparency
//
// SPI commands (5-byte, big-endian):
//   0x01 [xx] ...               : Read version (byte 1)
//   0x03 [xx] [xx] [xx] [xx]   : Read status (byte 1)
//   0x05 [00] [00] [00] [val]  : Set color palette (low 6 bits)
//   0x06 [00] [00] [hi] [lo]   : Set width (9-bit, default 320)
//   0x07 [00] [00] [00] [val]  : Set height (8-bit, default 240)
//   0x08 [00] [00] [00] [val]  : Set clock divider (0=full speed, 3=slowest)
//   0x10 [xx] ...               : Start animation (auto-repeating)
//   0x11 [xx] ...               : Release (tri-state all)

module app(clk, SCK, MOSI, MISO, SSEL, LCD_WR, LCD_DATA);

input clk;
input SCK, SSEL, MOSI;
output MISO;
inout LCD_WR;
inout [15:0] LCD_DATA;

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
// LCD column-first scan: y_counter fast (0..height-1), x_counter slow (0..width-1)
reg        filling;
reg        fill_done;
reg [8:0]  x_counter;  // page 0..width-1 (horizontal)
reg [7:0]  y_counter;  // row 0..height-1 (vertical)
reg [9:0]  frame_cnt;  // animation frame counter
reg [1:0]  clk_div;    // clock divider counter
reg [1:0]  clk_div_max; // divider limit (0=full speed, 3=slowest). Set via SPI cmd 0x08

// Configurable dimensions (set via SPI cmd 0x06/0x07)
reg [8:0]  width_reg;   // default 320 (9 bits: 0..511)
reg [7:0]  height_reg;  // default 240 (8 bits: 0..255)

wire [7:0] status = filling ? 8'h01 : (fill_done ? 8'h02 : 8'h00);

// ---------------------------------------------------------------
// Parallax animation (ported from Renaldas Zioma's VGA design)
// 5 layers of scrolling checkerboards with dithered transparency
// ---------------------------------------------------------------
wire [9:0] pix_x = {1'b0, x_counter};
wire [9:0] pix_y = {2'b0, y_counter};

// Animate layers — each scrolls at different speed
// Division replaced with shifts: /2 = >>1, /4 = >>2, /6 ≈ >>2 + >>3 (close enough)
wire [9:0] layer_a_x = pix_x + {frame_cnt[5:0], 4'b0};          // * 16
wire [9:0] layer_a_y = pix_y + {frame_cnt[8:0], 1'b0};          // * 2
wire [9:0] layer_b_x = pix_x + {frame_cnt[6:0], 3'b0} - frame_cnt; // * 7
wire [9:0] layer_b_y = pix_y + frame_cnt + frame_cnt[9:1];      // + cnt + cnt/2
wire [9:0] layer_c_x = pix_x + {frame_cnt[7:0], 2'b0};          // * 4
wire [9:0] layer_c_y = pix_y + frame_cnt[9:1];                   // / 2
wire [9:0] layer_d_x = pix_x + {frame_cnt[8:0], 1'b0};          // * 2
wire [9:0] layer_d_y = pix_y + frame_cnt[9:2];                   // / 4
wire [9:0] layer_e_x = pix_x + frame_cnt[9:1];                   // / 2
wire [9:0] layer_e_y = pix_y + frame_cnt[9:2] + frame_cnt[9:3]; // /4+/8 ≈ /2.67 (approx /6)

// Checker shape * transparency via pixel dithering
wire layer_a = (layer_a_x[8] ^ layer_a_y[8]) & ( pix_y[1] ^ pix_x[0]);
wire layer_b = (layer_b_x[7] ^ layer_b_y[7]) & (~pix_y[0] ^ pix_x[1]);
wire layer_c =  layer_c_x[6] ^ layer_c_y[6];
wire layer_d =  layer_d_x[5] ^ layer_d_y[5];
wire layer_e = (layer_e_x[4] ^ layer_e_y[4]) & ( pix_y[1] ^ pix_x[0]);

// Color palette (6-bit: RR_GG_BB, set via SPI cmd 0x05)
reg [5:0] color_input;
wire [5:0] color_a  = ~color_input;
wire [5:0] color_b  = color_a ^ 6'b00_10_10;
wire [5:0] color_c  = color_b & 6'b10_10_10;
wire [5:0] color_de = color_c >> 1;

// Layer priority mux (front to back)
wire [5:0] rgb6 = layer_a ? color_a :
                  (layer_b ? color_b :
                  (layer_c ? color_c :
                  (layer_d ? color_de :
                  (layer_e ? color_de : 6'b00_00_00))));

// Convert 6-bit {RR,GG,BB} to RGB565 {RRRRR,GGGGGG,BBBBB}
wire [4:0] r5 = {rgb6[5:4], rgb6[5:4], rgb6[5]};
wire [5:0] g6 = {rgb6[3:2], rgb6[3:2], rgb6[3:2]};
wire [4:0] b5 = {rgb6[1:0], rgb6[1:0], rgb6[1]};
wire [15:0] pixel_color = {r5, g6, b5};

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
        cmd_start   <= 1'b0;
        if (width_reg == 9'd0)  width_reg  <= 9'd320;
        if (height_reg == 8'd0) height_reg <= 8'd240;
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
                // Phase A: put pixel color on bus, drive WR low
                data_out <= pixel_color;
                wr_out   <= 1'b0;
            end
            else
            begin
                // Phase B: WR high (LCD latches), advance position
                wr_out <= 1'b1;
                if (y_counter == height_reg - 8'd1 && x_counter == width_reg - 9'd1)
                begin
                    // Frame complete — auto-repeat
                    y_counter <= 8'd0;
                    x_counter <= 9'd0;
                    frame_cnt <= frame_cnt + 10'd1;
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
                        8'h01:   data_send <= 8'h05;  // Version 5 (+ clk divider reg)
                        8'h03:   data_send <= status;
                        8'h10:   begin cmd_start   <= 1'b1; data_send <= 8'hAA; end
                        8'h11:   begin cmd_release  <= 1'b1; data_send <= 8'hAA; end
                        default: data_send <= 8'hAA;
                    endcase
                end

                8'h05: // Set color palette
                begin
                    if (byte_offset == 2'd3)
                        color_input <= {byte_data_received[4:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h06: // Set width (low 9 bits of last 2 bytes)
                begin
                    if (byte_offset == 2'd3)
                        width_reg <= {byte_data_received[0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h07: // Set height (low 8 bits of last byte)
                begin
                    if (byte_offset == 2'd3)
                        height_reg <= {byte_data_received[6:0], MOSI_data};
                    byte_offset <= byte_offset + 2'd1;
                    data_send   <= 8'h00;
                end

                8'h08: // Set clock divider (0=full speed, 1=div2, 2=div3, 3=div4)
                begin
                    if (byte_offset == 2'd3)
                        clk_div_max <= {byte_data_received[0], MOSI_data};
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
