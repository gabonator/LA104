#include <library.h>
#include "../adafruitgfx/Adafruit_GFX.h"
#include "../spi/spi.h"
#include "FancyEPD.h"
#include "FancyEPD_models.h"

CSpi SPI;

#define AUTO_REFRESH_AFTER_UPDATES        (5)

// Dev: error codes for: updateWithCompressedImage()
#define NO_ERROR                          (0)
#define ERROR_INVALID_VERSION             (1)
#define ERROR_UNKNOWN_HEADER_PROPERTY     (2)
#define ERROR_BPC_NOT_SUPPORTED           (3)
#define ERROR_CHANNELS_NOT_SUPPORTED      (4)
#define ERROR_INVALID_WIDTH               (5)
#define ERROR_INVALID_HEIGHT              (6)
#define ERROR_IMAGE_DATA_NOT_FOUND        (7)
#define ERROR_UNKNOWN_COMPRESSION_FORMAT  (8)
#define ERROR_CAN_UPDATE_IS_FALSE         (9)

// IL3895.pdf : Voltages, used in waveform phases.
// See the _sendWaveforms() for working examples.
const uint8_t _SOURCE = 0b00;	// Same, no change (actually the pixel drifts towards grey after enough updates)
const uint8_t _HIGH = 0b01;	// High (white)
const uint8_t _LOW  = 0b10;	// Low (black)

// Generate a byte describing one step of a waveform.
// Used for command: Write LUT register (0x32)
// hh = pixel transition is high-to-high,  hl = high-to-low, etc.
static uint8_t waveformByte(uint8_t hh, uint8_t hl, uint8_t lh, uint8_t ll)
{
	return (hh << 6) | (hl << 4) | (lh << 2) | ll;
}

FancyEPD::FancyEPD(
	epd_model_t model,
	uint32_t cs,	// CS: chip select
	uint32_t dc,	// DC: data/command
	uint32_t rs,	// RS: register select
	uint32_t bs,	// BS: busy signal
	uint32_t d0,	// D0: SPI clock (optional software SPI)
	uint32_t d1	// D1: SPI data	(optional software SPI)
) : Adafruit_GFX(epdWidth(model), epdHeight(model))
{
	_model = model;
	_driver = modelDriver(model);
	_colorChannels = colorChannelsForModel(model);
	_cs = cs;	// Chip select
	_dc = dc;	// Data/command
	_rs = rs;	// Register select
	_bs = bs;	// Busy signal
	_d0 = d0;	// SPI clock
	_d1 = d1;	// SPI data
	_hardwareSPI = (d0 == 0xffff);
	_temperature = 0x1A;
	_borderColor = 0x0;
	_borderBit = 0x0;
	_updatesSinceRefresh = 0xFF;
	_obeyWaitPin = true;
	_isAnimationMode = false;

	// Reset waveform timings
	for (uint8_t i = 0; i < k_update_builtin_refresh; i++) {
		restoreDefaultTiming((epd_update_t)i);
	}

	markDisplayDirty();
	_prevWindow = _window;
}

bool FancyEPD::init(uint8_t * optionalBuffer, epd_image_format_t bufferFormat)
{
	_buffer = optionalBuffer;
	_bufferFormat = bufferFormat;

	// SPI
	SPI.begin();
	SPI.reset();

	// One-time commands on startup
	switch (_model) {
		case k_epd_CFAP122250A00213:
		{
			// Software start (not documented in IL3895.pdf? Hmm)
			uint8_t soft_start[] = {0xd7, 0xd6, 0x9d};
			_sendData(0x0c, soft_start, 3);
		}
		break;

		case k_epd_CFAP104212D00213:	// ??? correct? This display is monochrome
		case k_epd_CFAP152152A00154:	// ??? correct?
		case k_epd_CFAP152152B00154:	// ??? correct?
		case k_epd_CFAP128296C00290:
		case k_epd_CFAP128296D00290:
		{
			// Kludge: Initial data is not being sent in the expected order,
			// causes the wait pin to remain blocked. Unset on draw.
			_obeyWaitPin = false;

			uint8_t data[] = {0, 0, 0};

			// Power on
			_sendData(0x04, NULL, 0);

			// Resolution setting: 1:1 please
			data[0] = _width;
			data[1] = _height >> 8;
			data[2] = _height & 0xff;
			_sendData(0x61, data, 3);
		}
		break;

		default: break;
	}

	return true;
}

uint8_t * FancyEPD::getBuffer()
{
	return _buffer;
}

uint32_t FancyEPD::getBufferSize()
{
	return getColorChannelSize() * _colorChannels;
}

uint32_t FancyEPD::getColorChannelSize()
{
	// Assumes 1-bit buffer.
	int16_t WIDTH_BYTES = ((WIDTH + 7) >> 3);
	return (WIDTH_BYTES * HEIGHT);
}

void FancyEPD::clearBuffer(uint8_t color)
{
	memset(_buffer, color, getBufferSize());
	markDisplayDirty();
}

void FancyEPD::invertBuffer()
{
	uint32_t sz = getBufferSize();
	for (uint32_t b = 0; b < sz; b++) {
		_buffer[b] ^= 0xff;
	}
}

void FancyEPD::invertFirstColorChannel()
{
	uint32_t sz = getColorChannelSize();
	for (uint32_t b = 0; b < sz; b++) {
		_buffer[b] ^= 0xff;
	}
}

bool FancyEPD::getAnimationMode()
{
	return _isAnimationMode;
}

void FancyEPD::setAnimationMode(bool b)
{
	_isAnimationMode = b;
}

// Default behavior: Only push data for changed pixels.
// Use this method to mark the entire display dirty, and
// send the entire _buffer to the EPD. (It's slower.)
void FancyEPD::markDisplayDirty()
{
	int16_t xMax = (int16_t)(WIDTH - 1);
	int16_t yMax = (int16_t)(HEIGHT - 1);

	_window = (window16){
		.xMin = 0, .yMin = 0,
		.xMax = xMax, .yMax = yMax
	};
}

void FancyEPD::markDisplayClean()
{
	int16_t xMin = (int16_t)(WIDTH - 1);
	int16_t yMin = (int16_t)(HEIGHT - 1);

	_window = (window16){
		.xMin = xMin, .yMin = yMin,
		.xMax = 0, .yMax = 0
	};
}

uint16_t FancyEPD::getPixel(int16_t x, int16_t y)
{
	_applyRotationForBuffer(&x, &y);

	uint16_t xByte = x >> 3;
	uint16_t bytesPerRow = (WIDTH + 7) >> 3;
	uint16_t posOffset = y * bytesPerRow + xByte;

	uint16_t out = 0x0;
	for (uint8_t c = 0; c < _colorChannels; c++) {
		// 2-channel board: red data is listed after black data.
		uint32_t channelOffset = getColorChannelSize() * c;

		// Pointer to the byte containing the pixel,
		// within the correct color channel.
		uint8_t *ptr = &_buffer[channelOffset + posOffset];

		if ((*ptr) & (0x80 >> (x & 0x7))) {
			out |= (0x1 << c);
		}
	}

	return out;
}

// Override Adafruit_GFX basic function for setting pixels
void FancyEPD::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

	_applyRotationForBuffer(&x, &y);

	int16_t xByte = x >> 3;
	int16_t bytesPerRow = (WIDTH + 7) >> 3;

	// Set the bit color
	for (uint8_t c = 0; c < _colorChannels; c++) {
		uint16_t channelOffset = getColorChannelSize() * c;
		uint8_t *ptr = &_buffer[channelOffset + y * bytesPerRow + xByte];

		// Default for 1 color channel: 0 == white, !0 == black
		bool inkOn = (bool)(color);

		// Multiple color channels use bitmask for colors:
		// 0x1 -> color channel #0, 0x2 -> #1, 0x4 -> #2, etc
		if (_colorChannels >= 2) {
			inkOn = (0x1 << c) & color;
		}

		if (inkOn) {
			*ptr |= 0x80 >> (x & 0x7);
		} else {
			*ptr &= ~(0x80 >> (x & 0x7));
		}
	}

	_window.xMin = min(x, _window.xMin);
	_window.yMin = min(y, _window.yMin);
	_window.xMax = max(x, _window.xMax);
	_window.yMax = max(y, _window.yMax);
}

void FancyEPD::setBorderColor(uint8_t color)
{
	_borderColor = color;
}

uint8_t FancyEPD::getTiming(epd_update_t update_type)
{
	if (update_type >= k_update_builtin_refresh) return 0;

	return _timingNormal[update_type];
}

uint8_t FancyEPD::getTiming_inverse(epd_update_t update_type)
{
	if (update_type >= k_update_builtin_refresh) return 0;

	return _timingInverse[update_type];
}

void FancyEPD::setCustomTiming(epd_update_t update_type, uint8_t time_normal, uint8_t time_inverse)
{
	if (update_type >= k_update_builtin_refresh) return;

	_timingNormal[update_type] = time_normal;
	_timingInverse[update_type] = time_inverse;
}

void FancyEPD::restoreDefaultTiming(epd_update_t update_type)
{
	if (update_type >= k_update_builtin_refresh) return;

	_timingNormal[update_type] = 20;
	_timingInverse[update_type] = 12;
}

void FancyEPD::waitUntilNotBusy()
{
	if (!_obeyWaitPin) {
		return;
	}

	SPI.wait();	
}

void FancyEPD::update(epd_update_t update_type)
{
	if (!_canUpdate()) return;

	if (update_type == k_update_auto) {
		if (_updatesSinceRefresh < (AUTO_REFRESH_AFTER_UPDATES - 1)) {
			update_type = k_update_no_blink;
		} else {
			update_type = k_update_quick_refresh;
		}
	}

	_screenWillUpdate(update_type);
	_sendBufferData(update_type);
	_sendUpdateActivation(update_type);
}

void FancyEPD::updateWithImage(const uint8_t * data, epd_image_format_t format, epd_update_t update_type)
{
	if (!_canUpdate()) return;

	// 1 bit (black & white)? Fall back on update()
	if (format == k_image_1bit) {
		memcpy(_buffer, data, getBufferSize());
		update(update_type);
		return;
	}

	_willUpdateWithImage(update_type);

	// Multiple draws will be required, for each bit.
	switch(format) {
		case k_image_2bit_monochrome:
		{
			for (uint8_t b = 0; b < 2; b++) {	// least-significant bits first
				uint16_t offset = 0;
				uint8_t mask0 = 0x1 << (b + 6);
				uint8_t mask1 = 0x1 << (b + 4);
				uint8_t mask2 = 0x1 << (b + 2);
				uint8_t mask3 = 0x1 << (b);

				markDisplayClean();

				for (int16_t y = 0; y < _height; y++) {
					for (int16_t x = 0; x < _width; x += 4) {
						uint8_t _byte = data[offset++];

						drawPixel(x, y, _byte & mask0);
						drawPixel(x + 1, y, _byte & mask1);
						drawPixel(x + 2, y, _byte & mask2);
						drawPixel(x + 3, y, _byte & mask3);
					}
				}

				_sendImageLayer(b, 2, (_borderColor & mask0));
			}
		}
		break;

		case k_image_4bit_monochrome:
		{
			for (uint8_t b = 0; b < 4; b++) {	// least-significant bits first
				uint16_t offset = 0;
				uint8_t mask_hi = 0x1 << (b + 4);
				uint8_t mask_lo = 0x1 << b;

				markDisplayClean();

				for (int16_t y = 0; y < _height; y++) {
					for (int16_t x = 0; x < _width; x += 2) {
						uint8_t _byte = data[offset++];

						// First pixel: Stored in highest 4 bits
						drawPixel(x, y, _byte & mask_hi);

						// Second pixel: Stored in lowest 4 bits
						drawPixel(x + 1, y, _byte & mask_lo);
					}
				}

				_sendImageLayer(b, 4, (_borderColor & mask_hi));
			}
		}
		break;

		case k_image_8bit_monochrome:
		{
			// For the sake of brevity: We're only displaying
			// the most-significant 4 bits of each pixel.
			for (uint8_t b = 0; b < 4; b++) {	// least-significant bits first
				uint16_t offset = 0;
				uint8_t mask = 0x10 << b;

				markDisplayClean();

				for (int16_t y = 0; y < _height; y++) {
					for (int16_t x = 0; x < _width; x++) {
						uint8_t _byte = data[offset++];

						drawPixel(x, y, _byte & mask);
					}
				}

				_sendImageLayer(b, 4, (_borderColor & mask) ? 1 : 0);
			}
		}
		break;

		default: break;
	}

	// Next `_auto` update: Trigger a refresh.
	_updatesSinceRefresh = 0xFF;
}

uint8_t FancyEPD::updateWithCompressedImage(const uint8_t * data, epd_update_t update_type)
{
	if (!_canUpdate()) return ERROR_CAN_UPDATE_IS_FALSE;

	uint8_t version = *data;
	data++;

	if (version != 1) {
		// Invalid version
		return ERROR_INVALID_VERSION;
	}

	// Read header
	uint8_t bpc = 0;	// bits per channel
	uint8_t channels = 0;
	int16_t width = 0;
	int16_t height = 0;
	const uint8_t * img_data = NULL;

	vlq_decoder decode_bytes = (vlq_decoder){.data = NULL, .mask = 0x80, .word_size = 8};

	// Read header. Fail after 10 reads and no img_data.
	for (uint8_t i = 0; i < 10; i++) {

		if ((*data) == 0x1) {	// Bits per channel
			bpc = data[1];
			data += 2;

		} else if ((*data) == 0x2) {	// Color channels
			// (monochrome -> 1, black+red -> 2)
			channels = data[1];
			data += 2;

		} else if ((*data) == 0x3) {	// width, height
			decode_bytes.data = &data[1];
			width = (int16_t)(_vlqDecode(&decode_bytes));
			height = (int16_t)(_vlqDecode(&decode_bytes));
			data = decode_bytes.data;

		} else if ((*data) == 0x4) {	// img_data starts here
			img_data = &data[1];
			break;

		} else {
			// Unknown header command! Fail.
			return ERROR_UNKNOWN_HEADER_PROPERTY;
		}

	}

	// Bail on garbage data
	if ((bpc == 0) || (bpc > 4)) return ERROR_BPC_NOT_SUPPORTED;
	if ((channels < 1) || (2 < channels)) {
		return ERROR_CHANNELS_NOT_SUPPORTED;
	}
	if (width <= 0) return ERROR_INVALID_WIDTH;
	if (height <= 0) return ERROR_INVALID_HEIGHT;
	if (!img_data) return ERROR_IMAGE_DATA_NOT_FOUND;

	// Not sure how else to handle this, but:
	// If the bit-depth matches the model's native
	// bit-depth, AND we're drawing with default
	// refresh, then only send update at end.
	bool is_native_bit_depth = (bpc == 1);

	if (!is_native_bit_depth) {
		_willUpdateWithImage(update_type);
	}

	// Decode & send each layer of image data
	const uint8_t * layer_start = img_data;

	for (uint8_t layer = 0; layer < bpc * channels; layer++) {
		markDisplayClean();

		decode_bytes.data = layer_start;
		uint16_t sz = (uint16_t)(_vlqDecode(&decode_bytes));

		const uint8_t * img_data_start = decode_bytes.data;
		const uint8_t * read = img_data_start;
		uint8_t cmpr = *read;	// header byte == compression type
		read++;

		// Compression: must be a known format
		if (cmpr > 0x2) return ERROR_UNKNOWN_COMPRESSION_FORMAT;

		uint8_t c = layer % channels;	// channel (0==black, 1==red)
		uint16_t color = (0x1) << c;
		uint8_t drawBit = layer / channels;

		int16_t x = 0, y = 0;

		if (c == 0) {
			clearBuffer(0x0);
		}

		// cmpr == 0: Raw, not compressed
		if (cmpr == 0) {
			// Just memcpy the image
			// Nope! This fails when pixel width isn't an
			// integer multiple of 8.
			//memcpy(_buffer, read, (width * height) >> 3);

			uint8_t mask = 0x80;

			while (y < height) {
				x = 0;

				while (x < width) {
					drawPixel(x, y, ((*read) & mask) ? color : 0x0);

					if (mask == 0x01) {
						mask = 0x80;
						read++;

					} else {
						mask >>= 1;
					}

					x++;
				}

				y++;
			}

		} else if ((cmpr == 1) || (cmpr == 2)) {	// RLE
			bool isOn = false;

			vlq_decoder rle = (vlq_decoder){.data = &read[1], .mask = 0x80, .word_size = *read};

			while (y < height) {
				uint32_t run = _vlqDecode(&rle);

				if (cmpr == 1) {	// 0x1: RLE, white vs black
					while (run--) {
						if (isOn) drawPixel(x, y, color);

						x++;
						if (x >= width) {
							x = 0;
							y++;
						}
					}

				} else {	// 0x2: RLE, same vs XOR
					while (run--) {
						if (y == 0) {
							// First row: Draw black & white
							if (isOn) drawPixel(x, y, color);

						} else {
							// Subsequent rows: same or XOR of row above
							bool pxAbove = getPixel(x, y - 1);
							bool pxAboveIsOn = (bool)(pxAbove & color);

							if (pxAboveIsOn != isOn) {
								drawPixel(x, y, color);
							}
						}

						x++;
						if (x >= width) {
							x = 0;
							y++;
						}

					}
				}

				isOn = !isOn;
			}

		}

		// All channels decoded for this layer?
		// Then send the image layer.
		if (c == (channels - 1)) {

			if (is_native_bit_depth) {
				// 1-bit image: It's all drawn.
				// Use the builtin refresh or whatever.
				update(update_type);

			} else {
				uint8_t border_mask = (0x80 >> (bpc - 1)) << drawBit;

				_sendImageLayer(drawBit, bpc, (_borderColor & border_mask));
			}
		}

		// Advance to next layer
		layer_start = &img_data_start[sz];
	}

	return NO_ERROR;
}

void FancyEPD::setTemperature(uint8_t temperature)
{
	// TODO: Temperature is a 12-bit value, so we're
	//       losing some resolution here.
	_temperature = temperature;
}

//
//  PRIVATE
//

void FancyEPD::_sendData(uint8_t command, uint8_t * data, uint16_t len) {
	const bool DEBUG_BUSY = false;

	// Ensure the busy pin is LOW
	waitUntilNotBusy();
	SPI.select();
	SPI.transfer9(command);

	for (uint16_t i = 0; i < len; i++) {
		SPI.transfer9(data[i] | 0x100);
	}
	SPI.deselect();
}

bool FancyEPD::_canUpdate(void)
{
	// FIXME ZKA
	return true;

	// _window must be valid. (Something must have been drawn.)
	if ((_window.xMin > _window.xMax) || (_window.yMin > _window.yMax)) {
		return false;
	}

	return true;
}

void FancyEPD::_sendImageLayer(uint8_t layer_num, uint8_t layer_count, uint8_t newBorderBit)
{
	const epd_update_t draw_scheme = k_update_INTERNAL_image_layer;

	// FIXME ZKA: support different image timings, based on layer_num and layer_count
	uint8_t timing = 3;

	if (_model == k_epd_CFAP122250A00213) {
		timing = 4;
	} else if (_model == k_epd_CFAP128296C00290) {
		timing = 7;
	} else if (_model == k_epd_CFAP104212D00213) {
		//                         MSB, , ,LSB
		//const uint8_t TIMINGS[] = {5, 6, 6, 6};
		const uint8_t TIMINGS[] = {4, 5, 6, 6};
		timing = TIMINGS[(layer_count - 1) - layer_num];

	} else if (colorChannelsForModel(_model) == 2) {
		timing = 10;	// FIXME this looks washed out
	}

	_sendWaveforms(draw_scheme, timing);

	_sendBorderBit(draw_scheme, (newBorderBit) ? 1 : 0);
	_sendWindow();
	_sendBufferData(draw_scheme);
	_sendUpdateActivation(draw_scheme);
}

void FancyEPD::_willUpdateWithImage(epd_update_t update_type)
{
	if (update_type == k_update_auto) {
		update_type = k_update_quick_refresh;
	}

	bool do_blink = (update_type == k_update_quick_refresh) || (update_type == k_update_builtin_refresh);

	if (do_blink) {
		clearBuffer();
		_screenWillUpdate(update_type);
		_sendBorderBit(update_type, 0);	// white border
		_sendBufferData(update_type);
		_sendUpdateActivation(update_type);

	} else {
		_screenWillUpdate(k_update_none);	// Don't apply waveforms here
	}
}

void FancyEPD::_screenWillUpdate(epd_update_t update_type)
{
	if ((update_type == k_update_quick_refresh) || (update_type == k_update_builtin_refresh)) {
		_updatesSinceRefresh = 0;

	} else if (_updatesSinceRefresh < 0xFF) {
		_updatesSinceRefresh++;
	}

	_sendDriverOutput();
	_sendGateScanStart();
	_sendDataEntryMode();
	_sendGateDrivingVoltage();
	_sendAnalogMode();
	_sendTemperatureSensor();
	_sendWaveforms(update_type);
	_sendVcomVoltage();

	uint8_t bc = _borderColor;
	if (_colorChannels == 1) {
		bc = (_borderColor & 0x80) ? 1 : 0;
	}
	_sendBorderBit(update_type, bc);

	_sendWindow();
}

void FancyEPD::_sendDriverOutput()
{
	if (_driver == k_driver_IL3895) {
		uint8_t data[] = {0xCF, 0x00};
		_sendData(0x01, data, 2);
	}
}

void FancyEPD::_sendGateScanStart()
{
	if (_driver == k_driver_IL3895) {
		uint8_t data[] = {0x0};
		_sendData(0x0F, data, 1);
	}
}

void FancyEPD::_sendDataEntryMode()
{
	if (_driver == k_driver_IL3895) {
		uint8_t data[] = {0x03};
		_sendData(0x11, data, 1);
	}
}

void FancyEPD::_sendGateDrivingVoltage()
{
	if (_model == k_epd_E2215CS062) {
		uint8_t data[] = {0x10, 0x0A};
		_sendData(0x03, data, 2);
	}
}

void FancyEPD::_sendAnalogMode()
{
	if (_model == k_epd_E2215CS062) {
		uint8_t data[] = {0, 0, 0};
		_sendData(0x05, data, 1);
		_sendData(0x75, data, 3);
	}
}

void FancyEPD::_sendTemperatureSensor()
{
	if (_model == k_epd_E2215CS062) {
		uint8_t data[] = {_temperature, 0x0};
		_sendData(0x1A, data, 2);
	}
}

// time_normal, time_inverse: optional overrides
void FancyEPD::_sendWaveforms(epd_update_t update_type, uint8_t time_normal, uint8_t time_inverse)
{
	// If the waveform timing is not being sent with overrides:
	// Apply the default or custom timing (whatever is in
	// _timingNormal and _timingInverse).
	if (update_type < k_update_builtin_refresh) {
		if (time_normal == 0) time_normal = _timingNormal[update_type];
		if (time_inverse == 0) time_inverse = _timingInverse[update_type];
	}

	// Crystalfontz 128x296
	if (_driver == k_driver_CFAP128296) {
		uint8_t lut_size = 42;

		// Flexible panel: Supports a larger LUT,
		// needs this data zeroed out, otherwise
		// drawing each layer is slow.
		if (_model == k_epd_CFAP104212D00213) {
			lut_size = 61;
		}

		uint8_t data[lut_size];

		// Lookup table from OTP or register?
		data[0] = 0b10111111;

		if (update_type == k_update_builtin_refresh) {
			data[0] &= 0b11011111;	// Flip the bit, use LUT OTP
		}

		// CrystalFontz color
		if (colorChannelsForModel(_model) == 2) {
			data[0] &= 0b11101111;	// blk+red+white
		}

		// FIXME: Testing grayscale on the blk+red device
		//data[0] |= 0b00010000;

		_sendData(0x00, data, 1);

		// Builtin refresh? It's all set to go!
		if (update_type == k_update_builtin_refresh) return;

		if (update_type == k_update_INTERNAL_image_layer) {
			if (time_normal == 0) time_normal = 3;
		}

		// LUT is 7 consecutive structures, 6 bytes each.
		//
		// struct[0]  : 0b11223344, each bit pair is a color:
		// 	00==no change, 01==white, 10==black, 11==grey (VCOM?)
		// struct[1-4]: Time (in cycles?) each color is applied
		// struct[5]  : Number of times to repeat this structure
		//
		// This is interesting, it implies that each
		// pixel transition can have different timing(s)
		// relative to other transitions. Test this? :)

		memset(data, 0, lut_size);

		// FIXME; Confirm that flexible monochrome display works here:
		if (colorChannelsForModel(_model) == 1) {
			data[0] = 0b10010000;	// [1]:black [2]:white
			data[1] = (update_type == k_update_quick_refresh) ? time_inverse : 0;
			data[2] = time_normal;
			data[5] = 1;	// repeat this structure 1 time

			// Special case: Flexible panel needs more
			// dark ink, otherwise image is too light.
			bool make_darker = (_model == k_epd_CFAP104212D00213) && (update_type == k_update_INTERNAL_image_layer);

			if (make_darker) {
				/*
				if (data[2] < 170) {
					data[2] += (data[2] >> 1);
					*/
				if (data[2] < 128) {
					data[2] *= 2;
				} else {
					data[2] = 0xff;
				}
			}

			// Black -> white
			_sendData(0x22, data, lut_size);

			// White -> white
			if (update_type == k_update_partial) {
				data[0] = 0x0;	// same color: don't change (VCOM)
			}
			_sendData(0x21, data, lut_size);

			data[2] = time_normal;	// reset

			// Set color to black:
			data[0] = 0b01100000;	// [1]:white [2]:black

			// White -> black
			_sendData(0x23, data, lut_size);

			// Black -> black
			if (update_type == k_update_partial) {
				data[0] = 0x0;	// same color: don't change (VCOM)
			}
			_sendData(0x24, data, lut_size);

		} else if (colorChannelsForModel(_model) == 2) {
			// black+red: This is where the rules go out the window.
			// Black and red particles have the same charge, but
			// different speeds. The red particle is physically
			// larger (or has a reduced charge?), and moves more slowly.
			// The trick to producing high-quality images is manipulating
			// the relative positions of the red and black particles.
			// Spoilers: It's tricky.

			// Color byte is 4 sets of 2 bits:
			//     00 => GND
			//     01 => _black_ (VDH)
			//     10 => _white_ (VDL)
			//     11 => red (VDHR)

			// the timing is extremely finicky.
			// It's a balance between update speed, image
			// cohesion (less ghosting, unbroken solid regions)
			// and color saturation (nice vivid red, not
			// blackish-red).

			const uint8_t PUSH_BLK_DOWN_TIME = 9;

			const uint8_t ALL_BLACK = 0b01010101;
			const uint8_t ALL_WHITE = 0b10101010;
			const uint8_t ALL_RED = 0b11111111;

			switch (update_type) {
				case k_update_quick_refresh:
				case k_update_no_blink:
				case k_update_partial:
				{
					bool do_blink = (update_type == k_update_quick_refresh);
					bool is_partial = (update_type == k_update_partial);

					// To create a cohesive, high-saturation red:
					// Alternate white and red voltage to red pixels.
					// This pushes black particles down, lets red rise.
					uint8_t RW_COUNT = (do_blink ? 5 : 1);

					// LUTR, red
					if (do_blink) {
						data[0] = 0b10010000;
						data[1] = 5;
						data[2] = time_inverse;
						data[5] = 2;

					} else {
						data[0] = ALL_WHITE;
						data[1] = time_normal;
						data[5] = 1;
					}

					data[6] = 0b10110000;	// white, red
					data[7] = PUSH_BLK_DOWN_TIME;
					data[8] = time_normal;
					data[11] = RW_COUNT;

					// Burn in the red at the end.
					if (do_blink) {
						data[12] = ALL_RED;
						data[13] = 100;
						data[17] = 1;
					}

					_sendData(0x22, data, lut_size);

					// Red needs burnin, other channels
					// definitely need less!
					//data[17] = 20;

					// LUTWW, white -> white
					data[0] = (do_blink ? ALL_BLACK : ALL_WHITE);
					data[6] = (is_partial ? 0b0 : ALL_WHITE);
					data[11] = (do_blink ? 1 : 0);
					data[12] = 0x0;//data[6];
					_sendData(0x21, data, lut_size);

					// LUTW, to white
					data[6] = ALL_WHITE;
					//data[12] = data[6];
					_sendData(0x23, data, lut_size);

					// LUTB, black
					data[0] = (do_blink ? ALL_WHITE : ALL_BLACK);
					data[6] = ALL_BLACK;
					//data[12] = data[6];
					_sendData(0x24, data, lut_size);
				};
				break;

				case k_update_INTERNAL_blink_like_crazy:
				{
					const uint8_t BLINK_CRAZY_TIMING = 12;
					const uint8_t BLINK_CRAZY_REPS = 4;

					data[0] = 0b10010000;  // white, black
					data[1] = BLINK_CRAZY_TIMING;
					data[2] = data[1];
					data[5] = BLINK_CRAZY_REPS;

					_sendData(0x21, data, lut_size);  // LUTWW
					_sendData(0x23, data, lut_size);  // LUTW, all white

					data[0] = 0b01100000;  // black, white
					_sendData(0x24, data, lut_size);  // LUTB (black)

					data[0] = 0b0;
					data[1] = 0;
					data[2] = 0;
					data[5] = 0;
					_sendData(0x22, data, lut_size);  // LUTR (red/color)
				}
				break;

				case k_update_INTERNAL_then_show_coherent_image:
				{
					// Phase 1
					const uint8_t BW_FLASH_TIME = 12;
					const uint8_t BW_FLASH_REPS_BLACK = 3;
          const uint8_t BW_FLASH_REPS_RED = 3;
          //const uint8_t BW_FLASH_REPS_WHITE = 1;
					const uint8_t BURN_TIME = 16;
					const uint8_t WR_WHITE_TIME = 2; // Phase 3: coarse red
					const uint8_t WR_RED_TIME = 15;
					const uint8_t WR_REPS = 4;
					const uint8_t WR2_WHITE_TIME = 2; // Phase 4: detailed vivid red
					const uint8_t WR2_RED_TIME = 60;
					const uint8_t WR2_REPS = 1;

					// BLACK AND RED
					// Phase 1: BW flashing
					data[0] = 0b01100000; // black, white
					data[1] = BW_FLASH_TIME;
					data[2] = BW_FLASH_TIME;
					data[5] = BW_FLASH_REPS_BLACK;

					// Phase 2: Big burn-in of solid color
					data[6] = 0b01000000;  // black
					data[7] = BURN_TIME;
					data[11] = 1;

					// Black is ready, so send it.
					_sendData(0x24, data, lut_size);  // LUTB (black)

					// For red: Phase 1 is inactive (don't blink with black)
					data[0] = 0;

					// Phase 2: Blink
					data[6] = 0b10010000; // white, black
					data[7] = BW_FLASH_TIME;
					data[8] = BW_FLASH_TIME;
					data[11] = BW_FLASH_REPS_RED;

					// Phase 3: white/red
					data[12] = 0b10110000;  // white, red
					data[13] = WR_WHITE_TIME;
					data[14] = WR_RED_TIME;
					data[17] = WR_REPS;

					data[18] = data[12];  // white, red
					data[19] = WR2_WHITE_TIME;
					data[20] = WR2_RED_TIME;
					data[23] = WR2_REPS;

					// Red is ready
					_sendData(0x22, data, lut_size);  // LUTR (red/color)

					// White and WW are different:
          /*
					data[0] = 0b10010000;  // Phase 1: Blink 1 time: white, black
          data[5] = BW_FLASH_REPS_WHITE;
          */
          data[0] = 0;  // no action

					// Phase 2: burn-in white, or no action for WW
					data[6] = 0b10000000; // white
					data[7] = BURN_TIME;
					data[8] = 0;
					data[11] = 1;

          data[12] = 0; // no action, phase 2
          data[18] = 0; // no action, phase 4

					_sendData(0x23, data, lut_size);  // LUTW, all white

					data[6] = 0;  // no action for LUTWW
					_sendData(0x21, data, lut_size);  // LUTWW
				}
				break;

				case k_update_INTERNAL_fast_color_redraw:
				{
					// Phase 1: Set W to white
					// Phase 2: B & R: Set to black
					// Phase 3: R -> red
					const uint8_t W_BLACK_TIME = 20;
					const uint8_t W_WHITE_TIME = 10;
					const uint8_t WB_WHITE_TIME = 18;
					const uint8_t WB_BLACK_TIME = 7;
					const uint8_t WB_BLACK_TIME_FOR_RED = 15;
					const uint8_t WB_WHITE_TIME_FOR_RED = 10;
					const uint8_t WB_REPS = 3;
					const uint8_t WB_REPS_FOR_RED = 3;
					const uint8_t WR_WHITE_TIME = 10;
					const uint8_t WR_BLACK_TIME = 40;
					const uint8_t WR_WHITE_TIME_FOR_RED = 7;
					const uint8_t WR_RED_TIME = 120;
					const uint8_t WR_RED_REPS = 1;

					// WW: No changes are needed. Send waveform now.
					_sendData(0x21, data, lut_size);  // LUTWW

					// W
					data[0] = 0b01100000;	// black, white
					data[1] = W_BLACK_TIME;
					data[2] = W_WHITE_TIME;
					data[5] = 1;

					// W phase 2: white pulses
					data[6] = 0b10000000;	// white
					data[7] = WB_WHITE_TIME;
					data[8] = WB_BLACK_TIME;
					data[11] = WB_REPS;

					_sendData(0x23, data, lut_size);  // LUTW, all white

					// Phase 2: WB has no action in phase 1
					data[0] = 0;
					data[6] = 0b10010000;	// white, black
					data[12] = 0b10010000;	// white, black
					data[13] = WR_WHITE_TIME;
					data[14] = WR_BLACK_TIME;
					data[17] = 1;
					_sendData(0x24, data, lut_size);  // LUTB (black)

					// Red: Fix WB timing, and add WR phase
					data[6] = 0b01100000;	// black, then white? hmm. Why so effective?
					data[7] = WB_BLACK_TIME_FOR_RED;
					data[8] = WB_WHITE_TIME_FOR_RED;
					data[11] = WB_REPS_FOR_RED;
					data[12] = 0b10110000;	// white, red
					data[13] = WR_WHITE_TIME_FOR_RED;
					data[14] = WR_RED_TIME;
					data[17] = WR_RED_REPS;
					_sendData(0x22, data, lut_size);  // LUTR (red/color)
				}
				break;

				default: break;
			}

		}

		return;	// exit Crystalfontz 128x296
	}

	uint8_t lut_size = 29;
	if (_model == k_epd_CFAP122250A00213) {
		lut_size = 30;
	}

	uint8_t data[lut_size];
	memset(data, 0, lut_size);

	switch (update_type) {
		case k_update_partial:
		{
			// Apply voltage only to pixels which change.
			data[0] = waveformByte(_SOURCE, _LOW, _HIGH, _SOURCE);
			data[16] = time_normal;
		}
		break;

		case k_update_no_blink:
		{
			// Apply voltage to all pixels, whether they change or not.
			data[0] = waveformByte(_HIGH, _LOW, _HIGH, _LOW);
			data[16] = time_normal;	// timing
		}
		break;

		case k_update_quick_refresh:
		{
			data[0] = waveformByte(_LOW, _HIGH, _LOW, _HIGH);	// inverted image
			data[1] = waveformByte(_HIGH, _LOW, _HIGH, _LOW);	// normal image

			data[16] = time_inverse;	// Inverted image: short flash
			data[17] = time_normal;	// Normal image: apply longer
		}
		break;

		case k_update_INTERNAL_image_layer:
		{
			if (time_normal == 0) time_normal = 3;

			data[0] = waveformByte(_HIGH, _LOW, _HIGH, _LOW);
			data[16] = time_normal;	// Short pulse
		}
		break;

		case k_update_builtin_refresh:
		default:
		{
			// builtin_refresh: Waveforms are reset in _sendUpdateActivation()

			return;
		}
	}

	_sendData(0x32, data, lut_size);

	// ZKA: Zero dummy line period? It's a tiny bit faster,
	// but I'm not sure what dummy line period is used for.
	// Gonna leave it alone, for now.
	/*
	uint8_t zero = 0;
	_sendData(0x3A, &zero, 1);
	*/

	if (_model == k_epd_CFAP122250A00213) {
		// "4 dummy line per gate"
		uint8_t dummy = 0x1a;
		_sendData(0x3a, &dummy, 1);

		// "2us per line"
		uint8_t gate_time = 0x08;
		_sendData(0x3b, &gate_time, 1);
	}
}

void FancyEPD::_sendBorderBit(epd_update_t update_type, uint8_t newBit)
{
	if (_driver == k_driver_IL3895) {
		uint8_t borderByte = 0x80 | (_borderBit << 1) | newBit;

		// _partial update: Looks bad if the border bit doesn't
		// change. So apply voltage in that case.
		if (update_type == k_update_partial) {
			// Force a change. Make A1 the opposite of A0.
			borderByte = (borderByte & (~0b10)) | (((~newBit) << 1) & 0b10);
		}

		_sendData(0x3C, &borderByte, 1);

	} else if (_driver == k_driver_CFAP128296) {
		// FIXME: Test border colors, make sure the
		//        resultant colors are what's expected.

		// From Crystalfontz code:
		//
		// 1000 0111 = Crystalfontz
		// BBRB CCCC
		// |||| ||||-- CDI: 0011 = default
		// ||||------- DDX[0]: Black Data Polarity 1=ink, 0=white
		// |||-------- DDX[1]: Red Data Polarity   1=ink, 0=white
		// ||--------- VBD: Border Data Selection
		//             (by experiment for DDX = 00)
		//               00=really light grey
		//               01=muddy red
		//               10=white <<<<
		//               11=black

		// VCOM and data interval settings
		uint8_t data[] = {0x97};

		if (colorChannelsForModel(_model) == 2) {
			data[0] = 0b10000111;
		}

		// Border byte
		data[0] = (data[0] & 0x3f) | (_borderBit << 6);

		_sendData(0x50, data, 1);
	}

	_borderBit = newBit;
}

void FancyEPD::_sendVcomVoltage()
{
	if (_model == k_epd_CFAP122250A00213) {
		uint8_t vcom[] = {0xa8};
		_sendData(0x2c, vcom, 1);
	}
}

void FancyEPD::_sendBufferData(epd_update_t update_type)
{
	if (isWaveformColorInverted(_driver, _model, update_type)) {
		invertFirstColorChannel();
	}

	// To defeat double-buffering artifacts on the device:
	// Send enough pixels to cover _prevWindow and _window.
	int16_t xMinByte = min(_prevWindow.xMin, _window.xMin) >> 3;
	int16_t xMaxByte = max(_prevWindow.xMax, _window.xMax) >> 3;
	int16_t yMin = min(_prevWindow.yMin, _window.yMin);
	int16_t yMax = max(_prevWindow.yMax, _window.yMax);

	// Send this many bytes of image data:
	uint16_t len = ((xMaxByte - xMinByte) + 1) * ((yMax - yMin) + 1);

	// Window: Do not allocate another buffer. We may
	// not have enough RAM. (Using Teensy LC, for example.)
	// Instead: Arrange pixel bytes so stream starts at [0].
	bool doArrange = (xMinByte > 0) ||
									 (xMaxByte < ((WIDTH - 1) >> 3)) ||
									 (yMin > 0) ||
									 (yMax < (HEIGHT - 1));

	if (doArrange) {
		_swapBufferBytes(xMinByte, yMin, xMaxByte, yMax, true);
	}

	if (_driver == k_driver_IL3895) {
		_sendData(0x24, &_buffer[0], len);

	} else if (_driver == k_driver_CFAP128296) {
		if (colorChannelsForModel(_model) == 1) {
			_sendData(0x13, &_buffer[0], len);	// Set NEW data

		} else if (colorChannelsForModel(_model) == 2) {
			uint32_t channelSize = getColorChannelSize();

			_sendData(0x10, &_buffer[0], len);	// Black
			_sendData(0x13, &_buffer[channelSize], len);	// Red
		}

		_sendData(0x11, NULL, 0);	// DATA STOP command
	}

	// After sending: Swap everything back.
	if (doArrange) {
		_swapBufferBytes(xMinByte, yMin, xMaxByte, yMax, false);
	}

	// Undo inverted buffer, so we can continue
	// drawing on it.
	if (isWaveformColorInverted(_driver, _model, update_type)) {
		invertFirstColorChannel();
	}
}

void FancyEPD::_sendUpdateActivation(epd_update_t update_type)
{
	if (_driver == k_driver_IL3895) {
		uint8_t sequence = 0xC7;
		if (update_type == k_update_builtin_refresh) {
			sequence = 0xF7;
		}

		_sendData(0x22, &sequence, 1);	// Display Update type

		_sendData(0x20, NULL, 0);	// Master activation

	} else if (_driver == k_driver_CFAP128296) {
		_sendData(0x12, NULL, 0);	// Display refresh

	}

	// To defeat double-buffering artifacts: Cache _window
	// as _prevWindow.
	_prevWindow = _window;

	// The screen pixels now match _buffer. All clean!
	markDisplayClean();

	// Now that an update has started:
	// Fix the CrystalFontz boot kludge: Now we can obey the wait pin
	_obeyWaitPin = true;
}

void FancyEPD::_sendWindow()
{
	// When not in animation mode: Always send a full
	// screen of data. The large transfer makes this
	// a tiny bit slower, but images will look cleaner
	// (less drift towards VCOM grey).
	if (!_isAnimationMode) {
		markDisplayDirty();
	}

	// FIXME: Implement window for 128x296 displays,
	//        then remove this.
	if (_driver == k_driver_CFAP128296) {
		markDisplayDirty();
	}

	// Window for image data: Send enough pixels to cover
	// both _prevWindow and _window, defeat double-buffering
	// artifacts on the device
	int16_t xMin = min(_prevWindow.xMin, _window.xMin);
	int16_t xMax = max(_prevWindow.xMax, _window.xMax);
	int16_t yMin = min(_prevWindow.yMin, _window.yMin);
	int16_t yMax = max(_prevWindow.yMax, _window.yMax);

	// Actual redraw area (gate multiplexing):
	// Only need to redraw the changed area.
	// FIXME ZKA: Why max(16) ?
	int16_t muxLines = max(16, _window.yMax - _window.yMin + 1);

	if (_driver == k_driver_IL3895) {
		// Multiplexing: Only MUX the rows which have changed
		if (_model == k_epd_E2215CS062) {
			uint8_t data_mux[] = {(uint8_t)muxLines, 0x0};
			_sendData(0x01, data_mux, 2);

		} else if (_model == k_epd_CFAP122250A00213) {
			uint8_t data_mux[] = {(uint8_t)((muxLines-1)&0x00FF), (uint8_t)((muxLines-1)>>8)};
			_sendData(0x01, data_mux, 2);
		}

		uint8_t gateStartY = min(_window.yMin, HEIGHT - (muxLines - 1));
		_sendData(0x0F, &gateStartY, 1);

		// Window coordinates
		uint8_t data_x[] = {
			(uint8_t)(xMin >> 3),
			(uint8_t)(xMax >> 3)
		};
		_sendData(0x44, data_x, 2);

		uint8_t data_y[] = {
			(uint8_t)(yMin),
			(uint8_t)(yMax)
		};
		_sendData(0x45, data_y, 2);

		// XY counter
		_sendData(0x4E, &data_x[0], 1);
		_sendData(0x4F, &data_y[0], 1);

	} else if (_driver == k_driver_CFAP128296) {
		// TODO
		// use commands:  0x90, 0x91, 0x92
		/*
		uint8_t data = {
			()
		};
		_sendData(0x90, data, 7);
		*/
	}
}

void FancyEPD::_applyRotationForBuffer(int16_t * x, int16_t * y)
{
	switch (getRotation()) {
		case 1:
		{
			int16_t temp = *x;
			*x = (WIDTH - 1) - *y;
			*y = temp;
		}
		break;

		case 2:
		{
			*x = (WIDTH - 1) - *x;
			*y = (HEIGHT - 1) - *y;
		}
		break;

		case 3:
		{
			int16_t temp = *x;
			*x = *y;
			*y = (HEIGHT - 1) - temp;
		}
		break;

		default:
			break;
	}
}

// For streaming windowed region, without allocating another
// buffer: Arrange pixel bytes so stream starts at &_buffer[0].
void FancyEPD::_swapBufferBytes(int16_t xMinByte, int16_t yMin, int16_t xMaxByte, int16_t yMax, bool ascending)
{
	uint16_t len = ((xMaxByte - xMinByte) + 1) * ((yMax - yMin) + 1);

	if (ascending) {
		// Arrange windowed bytes starting at [0]
		uint16_t b = 0;

		for (int16_t y = yMin; y <= yMax; y++) {
			uint16_t win = y * (WIDTH >> 3) + xMinByte;	// optimization

			for (int16_t x = xMinByte; x <= xMaxByte; x++) {
				uint8_t temp = _buffer[b];
				_buffer[b] = _buffer[win];
				_buffer[win] = temp;

				b++;
				win++;
			}
		}

	} else {	// descending
		uint16_t b = len - 1;

		for (int16_t y = yMax; y >= yMin; y--) {
			uint16_t win = y * (WIDTH >> 3) + xMaxByte;

			for (int16_t x = xMaxByte; x >= xMinByte; x--) {
				uint8_t temp = _buffer[b];
				_buffer[b] = _buffer[win];
				_buffer[win] = temp;

				b--;
				win--;
			}
		}
	}
}

// Simple state machine: Pull bits from ->data,
// in ->word_size chunks. The leading bit indicates
// whether the value continues in subsequent words;
// otherwise this is the final word in the value.
uint32_t FancyEPD::_vlqDecode(vlq_decoder * decoder) {
	uint32_t out = 0;
	bool doesContinue = false;

	for (uint8_t zz = 0; zz < 20; zz++) {	// safer than while()?

		for (uint8_t r = 0; r < decoder->word_size; r++) {
			uint8_t bit = ((*decoder->data) & decoder->mask) ? 1 : 0;

			// First bit: Sets whether word continues
			if (r == 0) {
				doesContinue = (bool)(bit);

			} else {
				// Shift this bit onto the right-hand side of value.
				out = (out << 1) | bit;
			}

			// Move the decoder mask to the next bit, advancing
			// to the next byte if needed.
			if (decoder->mask == 0x1) {	// Final bit?
				decoder->data++;	// Next byte
				decoder->mask = 0x80;	// Reset mask

			} else {
				decoder->mask = (decoder->mask >> 1);
			}
		}

		if (!doesContinue) break;
	}

	return out;
}

void FancyEPD::reset()
{
	SPI.reset();
}
