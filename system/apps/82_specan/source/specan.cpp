/*
 * Copyright 2010 Michael Ossmann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "specan.h"
#include "CC1101.h"

/* globals */
channel_info chan_table[NUM_CHANNELS];
uint16_t center_freq;
uint16_t user_freq;
uint8_t band;
uint8_t width;
char max_hold;
char height;
char sleepy;
uint8_t vscroll;
uint8_t min_chan;
uint8_t max_chan;
uint8_t sweep;
uint8_t persistence;


void radio_setup() {

	static const uint8_t specan_settings [] = {
	/* IF of 457.031 kHz */
	CC1101_FSCTRL1,0x12,
	CC1101_FSCTRL0,0x00,

	/* disable 3 highest DVGA settings */
	CC1101_AGCCTRL2, 0xC0,

	/* frequency synthesizer calibration */
	CC1101_FSCAL3, 0xEA,
	CC1101_FSCAL2, 0x2A,
	CC1101_FSCAL1, 0x00,
	CC1101_FSCAL0, 0x1F,
	CC1101_TEST2 ,0x88,
	CC1101_TEST1 ,0x31,
	CC1101_TEST0 ,0x09,
	CC1101_MCSM0 ,0,
    0,0
	};

	cc1101.RadioConfigure(specan_settings);
}

/* set the channel bandwidth */
void set_filter() {
	/* channel spacing should fit within 80% of channel filter bandwidth */
	switch (width) {
	case NARROW:
		cc1101.SpiWriteReg(CC1101_MDMCFG4,0xEC); /* 67.708333 kHz */
		break;
	case ULTRAWIDE:
		cc1101.SpiWriteReg(CC1101_MDMCFG4,0x0C); /* 812.5 kHz */
		break;
	default:
		cc1101.SpiWriteReg(CC1101_MDMCFG4,0x6C); /* 270.833333 kHz */
		break;
	}
}

/* set the radio frequency in Hz */
void set_radio_freq(uint32_t freq) {
	/* the frequency setting is in units of 396.728515625 Hz */

	uint32_t setting = (uint32_t) (freq * .0025206154);
	cc1101.SpiWriteReg(CC1101_FREQ2,(setting >> 16) & 0xff);
	cc1101.SpiWriteReg(CC1101_FREQ1,(setting >> 8) & 0xff);
	cc1101.SpiWriteReg(CC1101_FREQ0,setting & 0xff);

if ((band == BAND_300 && freq < MID_300) ||
			(band == BAND_400 && freq < MID_400) ||
			(band == BAND_900 && freq < MID_900)){
		/* select low VCO */
		cc1101.SpiWriteReg(CC1101_FSCAL2,0x0A);
	}else{
		/* select high VCO */
		cc1101.SpiWriteReg(CC1101_FSCAL2,0x2A);
	}

}

/* freq in Hz */
void calibrate_freq(uint32_t freq, uint8_t ch) {
	set_radio_freq(freq);

	cc1101.SpiStrobe(CC1101_SCAL);
	cc1101.SpiStrobe(CC1101_SRX);

	/* wait for calibration */
	delay(2);

	/* store frequency/calibration settings */
	chan_table[ch].freq2 = cc1101.SpiReadReg(CC1101_FREQ2);
	chan_table[ch].freq1 = cc1101.SpiReadReg(CC1101_FREQ1);
	chan_table[ch].freq0 = cc1101.SpiReadReg(CC1101_FREQ0);
	chan_table[ch].fscal3 = cc1101.SpiReadReg(CC1101_FSCAL3);
	chan_table[ch].fscal2 = cc1101.SpiReadReg(CC1101_FSCAL2);
	chan_table[ch].fscal1 = cc1101.SpiReadReg(CC1101_FSCAL1);

	/* get initial RSSI measurement */
	chan_table[ch].ss[sweep] = (cc1101.SpiReadReg(CC1101_RSSI) ^ 0x80);
	chan_table[ch].max = 0;
	chan_table[ch].last_drawn =  0;

//	RFST = RFST_SIDLE;
	cc1101.SpiStrobe(CC1101_SIDLE);

}

#define UPPER(a, b, c)  ((((a) - (b) + ((c) / 2)) / (c)) * (c))
#define LOWER(a, b, c)  ((((a) + (b)) / (c)) * (c))

/* set the center frequency in MHz */
uint16_t set_center_freq(uint16_t freq) {
	uint8_t new_band;
	uint32_t spacing;
	uint32_t hz;
	uint32_t min_hz;
	uint32_t max_hz;
	uint8_t margin;
	uint8_t step;
	uint16_t upper_limit;
	uint16_t lower_limit;
	uint16_t next_up;
	uint16_t next_down;
	uint8_t next_band_up;
	uint8_t next_band_down;
	uint8_t i;

	switch (width) {
	case NARROW:
		margin = NARROW_MARGIN;
		step = NARROW_STEP;
		spacing = NARROW_SPACING;
		break;
	case ULTRAWIDE:
		margin = ULTRAWIDE_MARGIN;
		step = ULTRAWIDE_STEP;
		spacing = ULTRAWIDE_SPACING;

		/* nearest 20 MHz step */
		freq = ((freq + 10) / 20) * 20;
		break;
	default:
		margin = WIDE_MARGIN;
		step = WIDE_STEP;
		spacing = WIDE_SPACING;

		/* nearest 5 MHz step */
		freq = ((freq + 2) / 5) * 5;
		break;
	}

	/* handle cases near edges of bands */
	if (freq > EDGE_900) {
		new_band = BAND_900;
		upper_limit = UPPER(MAX_900, margin, step);
		lower_limit = LOWER(MIN_900, margin, step);
		next_up = LOWER(MIN_300, margin, step);
		next_down = UPPER(MAX_400, margin, step);
		next_band_up = BAND_300;
		next_band_down = BAND_400;
	} else if (freq > EDGE_400) {
		new_band = BAND_400;
		upper_limit = UPPER(MAX_400, margin, step);
		lower_limit = LOWER(MIN_400, margin, step);
		next_up = LOWER(MIN_900, margin, step);
		next_down = UPPER(MAX_300, margin, step);
		next_band_up = BAND_900;
		next_band_down = BAND_300;
	} else {
		new_band = BAND_300;
		upper_limit = UPPER(MAX_300, margin, step);
		lower_limit = LOWER(MIN_300, margin, step);
		next_up = LOWER(MIN_400, margin, step);
		next_down = UPPER(MAX_900, margin, step);
		next_band_up = BAND_400;
		next_band_down = BAND_900;
	}

	if (freq > upper_limit) {
		freq = upper_limit;
		if (new_band == band) {
			new_band = next_band_up;
			freq = next_up;
		}
	} else if (freq < lower_limit) {
		freq = lower_limit;
		if (new_band == band) {
			new_band = next_band_down;
			freq = next_down;
		}
	}

	band = new_band;

	/* doing everything in Hz from here on */
	switch (band) {
	case BAND_400:
		min_hz = MIN_400 * 1000000;
		max_hz = MAX_400 * 1000000;
		break;
	case BAND_300:
		min_hz = MIN_300 * 1000000;
		max_hz = MAX_300 * 1000000;
		break;
	default:
		min_hz = MIN_900 * 1000000;
		max_hz = MAX_900 * 1000000;
		break;
	}

	/* calibrate upper channels */
	hz = freq * 1000000;
	max_chan = NUM_CHANNELS / 2;
	while (hz <= max_hz && max_chan < NUM_CHANNELS) {
		calibrate_freq(hz, max_chan);
		hz += spacing;
		for (i = 0; i < persistence; i++)
			chan_table[max_chan].ss[i] = 0;
		max_chan++;
	}

	/* calibrate lower channels */
	hz = freq * 1000000 - spacing;
	min_chan = NUM_CHANNELS / 2;
	while (hz >= min_hz && min_chan > 0) {
		min_chan--;
		calibrate_freq(hz, min_chan);
		for (i = 0; i < persistence; i++)
			chan_table[min_chan].ss[i] = 0;
		hz -= spacing;
	}

	center_freq = freq;
	//clear();
	//draw_ruler();
	//draw_freq();

	return freq;
}

/* tune the radio using stored calibration */
void tune(uint8_t ch) {

uint8_t tune_cfg[] = {
	CC1101_FREQ2,chan_table[ch].freq2,
	CC1101_FREQ1,chan_table[ch].freq1,
	CC1101_FREQ0,chan_table[ch].freq0,
	CC1101_FSCAL3,chan_table[ch].fscal3,
	CC1101_FSCAL2,chan_table[ch].fscal2,
	CC1101_FSCAL1,chan_table[ch].fscal1,
    0,0
	};

	cc1101.RadioConfigure(tune_cfg);
}

void set_width(uint8_t w) {
	width = w;
	set_filter();
	set_center_freq(center_freq);
}

void init_all(){

	center_freq = DEFAULT_FREQ;
	user_freq = DEFAULT_FREQ;
	band = BAND_400;
	width = WIDE;
	max_hold = 1;
	height = 0;
	sleepy = 0;
	vscroll = 0;
	min_chan = 0;
	max_chan = NUM_CHANNELS - 1;
	sweep = 0;
	persistence = PERSIST;

	radio_setup();

	set_width(WIDE);

}

