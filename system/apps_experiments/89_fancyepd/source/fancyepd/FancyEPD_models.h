#ifndef FANCYEPD_MODELS_H
#define FANCYEPD_MODELS_H

#include <stdbool.h>
#include <stdint.h>

epd_driver_t modelDriver(epd_model_t model) {
	switch (model) {
		case k_epd_CFAP122250A00213:
		case k_epd_E2215CS062:
			return k_driver_IL3895;

		case k_epd_CFAP104212D00213:	// ??? correct? This display is monochrome
		case k_epd_CFAP152152A00154:
		case k_epd_CFAP152152B00154:
		case k_epd_CFAP128296C00290:
		case k_epd_CFAP128296D00290:
			return k_driver_CFAP128296;

		default: break;

	}

	return k_driver_unknown;
}

int16_t epdWidth(epd_model_t model)
{
	switch (model) {

		// Crystalfontz
		case k_epd_CFAP152152A00154:    return 152;
		case k_epd_CFAP152152B00154:    return 152;
		case k_epd_CFAP104212D00213:    return 104;
		case k_epd_CFAP122250A00213:    return 122;
		case k_epd_CFAP128296C00290:    return 128;
		case k_epd_CFAP128296D00290:    return 128;

		// Pervasive Displays
		case k_epd_E2215CS062:          return 112;

		default:                        break;
	}

	return 0;	// not found
}

int16_t epdHeight(epd_model_t model)
{
	switch (model) {

		// Crystalfontz
		case k_epd_CFAP152152A00154:    return 152;
		case k_epd_CFAP152152B00154:    return 152;
		case k_epd_CFAP104212D00213:    return 212;
		case k_epd_CFAP122250A00213:    return 250;
		case k_epd_CFAP128296C00290:    return 296;
		case k_epd_CFAP128296D00290:    return 296;

		// Pervasive Displays
		case k_epd_E2215CS062:          return 208;

		default:                        break;
	}

	return 0;	// not found
}

uint8_t colorChannelsForModel(epd_model_t model)
{
	switch (model) {
		case k_epd_CFAP152152A00154:	// blk+red
		case k_epd_CFAP152152B00154:  // blk+ylw
		case k_epd_CFAP128296D00290:	// blk+red
		{
			return 2;
		}
		break;

		default: {} break;
	}

	return 1;
}

bool isWaveformColorInverted(epd_driver_t driver, epd_model_t model, epd_update_t update_type)
{
	switch (driver) {
		// Crystalfontz flexible panel, and others: The builtin
		// waveforms draw an inverted image?! Oh, OK.
		case k_driver_CFAP128296:
		{
			if (colorChannelsForModel(model) == 1) {
				return (update_type == k_update_builtin_refresh);
			}
		}
		break;

		default: {} break;
	}

	return false;
}

#endif
