//#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" {
#include "alloc.h"
#include "rtl_433_devices.h"
#include "pulse_detect.h"
#include "pulse_demod.h"
#include "data.h"
}

extern "C" char* convert(int* sequence, int length);
extern "C" int* buffer();

data_output_t* output;

void data_acquired_handler(r_device *r_dev, data_t *data)
{
    // prepend "description" if requested
    if (1) {
        data = data_prepend(data,
                "description", "Description", DATA_STRING, r_dev->name,
                NULL);
    }

    // prepend "protocol" if requested
    if (1) {
        data = data_prepend(data,
                "protocol", "Protocol", DATA_INT, r_dev->protocol_num,
                NULL);
    }
	if (output->file - output->fileBase > 1)
		*output->file++ = ',';
	data_output_print(output, data);
    data_free(data);
}



static int run_ook_demod(r_device* r_dev, pulse_data_t *pulse_data)
{
    int p_events = 0;
	switch (r_dev->modulation) {
	case OOK_PULSE_PCM_RZ:
		p_events += pulse_demod_pcm(pulse_data, r_dev);
		break;
	case OOK_PULSE_PPM:
		p_events += pulse_demod_ppm(pulse_data, r_dev);
		break;
	case OOK_PULSE_PWM:
		p_events += pulse_demod_pwm(pulse_data, r_dev);
		break;
	case OOK_PULSE_MANCHESTER_ZEROBIT:
		p_events += pulse_demod_manchester_zerobit(pulse_data, r_dev);
		break;
	case OOK_PULSE_PIWM_RAW:
		p_events += pulse_demod_piwm_raw(pulse_data, r_dev);
		break;
	case OOK_PULSE_PIWM_DC:
		p_events += pulse_demod_piwm_dc(pulse_data, r_dev);
		break;
	case OOK_PULSE_DMC:
		p_events += pulse_demod_dmc(pulse_data, r_dev);
		break;
	case OOK_PULSE_PWM_OSV1:
		p_events += pulse_demod_osv1(pulse_data, r_dev);
		break;
	// FSK decoders
	case FSK_PULSE_PCM:
	case FSK_PULSE_PWM:
		break;
	case FSK_PULSE_MANCHESTER_ZEROBIT:
		p_events += pulse_demod_manchester_zerobit(pulse_data, r_dev);
		break;
	default:
		fprintf(stderr, "Unknown modulation %d in protocol!\n", r_dev->modulation);
	}

    return p_events;
}

static void update_protocol(r_device *r_dev)
{
	int samp_rate = 1000000;
	int verbosity = -1;
	int verbose_bits = 0;
	
    float samples_per_us = samp_rate / 1.0e6;

    r_dev->f_short_width = 1.0 / (r_dev->short_width * samples_per_us);
    r_dev->f_long_width  = 1.0 / (r_dev->long_width * samples_per_us);
    r_dev->s_short_width = r_dev->short_width * samples_per_us;
    r_dev->s_long_width  = r_dev->long_width * samples_per_us;
    r_dev->s_reset_limit = r_dev->reset_limit * samples_per_us;
    r_dev->s_gap_limit   = r_dev->gap_limit * samples_per_us;
    r_dev->s_sync_width  = r_dev->sync_width * samples_per_us;
    r_dev->s_tolerance   = r_dev->tolerance * samples_per_us;

    r_dev->verbose      = verbosity > 0 ? verbosity - 1 : 0;
    r_dev->verbose_bits = verbose_bits;
	
	
	r_dev->s_tolerance = 200;
}


static void print_buffer_array(data_output_t *output, data_array_t *array, char *format)
{
	char buf[32];
	
    sprintf(buf, "[");
	strcat(output->file, buf); output->file += strlen(buf);
    for (int c = 0; c < array->num_values; ++c) {
        if (c)
		{
            sprintf(buf, ", ");
			strcat(output->file, buf); output->file += strlen(buf);
		}
        print_array_value(output, array, format, c);
    }
    sprintf(buf, "]");
	strcat(output->file, buf); output->file += strlen(buf);
}

static void print_buffer_data(data_output_t *output, data_t *data, char *format)
{
    bool separator = false;
	*output->file++ = '{';
	*output->file = 0;
    while (data) {
        if (separator)
		{
			strcat(output->file, ", ");
			output->file += strlen(output->file);
		}
        output->print_string(output, data->key, NULL);
		strcat(output->file, " : ");
		output->file += strlen(output->file);
        print_value(output, data->type, data->value, data->format);
        separator = true;
        data = data->next;
    }
	strcat(output->file, "}");
	output->file += strlen(output->file);
}

static void print_buffer_string(data_output_t *output, const char *str, char *format)
{
	*output->file++ = '\"';
    while (*str) {
        if (*str == '"')
			*output->file++ = '\\';
		*output->file++ = *str;
        ++str;
    }
	*output->file++ = '\"';
	*output->file = 0;
}

static void print_buffer_double(data_output_t *output, double data, char *format)
{
	char buf[128];
    sprintf(buf, "%.3f", data);
	strcat(output->file, buf); output->file += strlen(buf);
}

static void print_buffer_int(data_output_t *output, int data, char *format)
{
	char buf[128];
    sprintf(buf, "%d", data);
	strcat(output->file, buf); output->file += strlen(buf);
}

/*static*/ void data_output_buffer_free(data_output_t *output)
{
    if (!output)
        return;

    //free(output);
}

struct data_output *data_output_buffer_create()
{
	static char buffer[4096];
	buffer[0] = 0;
	
	static data_output_t singleton;
	data_output_t *output = &singleton;
    //data_output_t *output = (data_output_t *)calloc(1, sizeof(data_output_t));
    if (!output) {
        fprintf(stderr, "calloc() failed");
        return NULL;
    }

    output->print_data   = print_buffer_data;
    output->print_array  = print_buffer_array;
    output->print_string = print_buffer_string;
    output->print_double = print_buffer_double;
    output->print_int    = print_buffer_int;
    output->output_free  = data_output_buffer_free;
    output->fileBase     = buffer;
    output->file         = buffer;

    return output;
}

char* convert(int* sequence, int length)
{
	allocreset();
	pulse_data_t pulse_data;
	pulse_data.sample_rate = 1000000;

	pulse_data.num_pulses = 0;
	for (int i=0; i<length/2; i++)
	{
		pulse_data.pulse[pulse_data.num_pulses] = sequence[i*2];
		pulse_data.gap[pulse_data.num_pulses++] = sequence[i*2+1];
	}
	
	pulse_data.gap[pulse_data.num_pulses-1] = 10000;

	output = data_output_buffer_create();
	*output->file++ = '[';
	*output->file = 0;

	int nid = 0;
	#define DECL(name) \
		name.protocol_num = nid++; \
		update_protocol(&name); \
		name.output_fn  = data_acquired_handler; \
		run_ook_demod(&name, &pulse_data);

	DEVICES
	
	#undef DECL
	*output->file++ = ']';
	*output->file = 0; // TODO: ako sa tam dostalo cislo!?
	char* buf = output->fileBase;
	data_output_free(output);
	return buf;
}

int* buffer()
{
	static int singleton[1024];
	return singleton;
}

#ifndef EMSCRIPTEN

//void allocreset()
//{
//}

int main(int argc, const char * argv[])
{
	int sequence1[] =	{466,500,500,466,1000,966,966,966,500,466,533,466,500,500,966,966,966,966,533,466,466,500,500,466,1000,466,500,1000,966,500,466,466,533,466,500,966,500,466,500,466,533,433,500,500,1000,933,533,466,500,466,533,433,1000,966,500,466,500,500,500,433,533,466,500,466,500,500,500,466,500,500,966,966,1000,966,500,466,500,466,1000,966,500,466,500,466,500,466,500,500,500,500,966,966,500,466,500,466,533,466,966,966,1000,966,966,500,500,466,500,466,533,933,500,500,266};
	
	int sequence2[] = {466,466,500,500,466,533,466,466,500,500,500,466,500,466,466,533,466,500,500,466,466,533,466,500,500,466,500,466,500,500,500,466,466,533,466,500,500,466,466,500,500,500,500,433,500,500,500,1000,933,1000,966,966,966,1033,433,500,1000,1000,433,533,933,533,466,966,500,500,466,500,466,500,466,533,933,1000,466,533,466,466,500,500,466,500,966,1000,966,966,466,500,1000,966,966,1000,500,466,500,466,500,500,466,500,466,533,466,500,466,500,466,500,500,500,466,500,466,500,466,533,466,466,500,500,500,466,500,466,500,500,466,533,466,466,500,500,466,533,933,1000,966,966,466,533,966,966,466,500,533,466,466,500,966,1000,466,500,1000,966,933,1000,500,500,466,500,466,500,500,500,433,500,1000,500,466,466,500,1000,966,500,466,1000,500,500,433,500,966,1000,466,500,966,1033,933,1000,466};

	int sequence3[] ={466,533,433,500,466,533,466,533,433,500,500,500,466,533,433,500,466,533,466,533,433,500,466,533,466,500,466,500,466,533,466,500,466,500,466,533,466,500,433,533,466,500,500,500,433,533,466,1000,933,1000,1000,966,933,1033,466,500,966,1000,466,500,966,500,466,966,500,500,466,533,466,466,500,500,966,966,466,533,466,500,466,500,466,533,966,966,966,1033,433,500,966,1033,900,1000,500,500,433,533,466,500,500,500,433,533,466,533,466,500,433,533,466,533,433,533,433,533,466,500,500,500,433,533,433,533,466,533,433,533,433,533,466,533,433,500,500,500,966,966,966,1033,400,533,966,1000,433,533,466,533,466,500,933,1033,466,466,966,1000,966,1000,466,533,433,500,500,500,466,500,500,466,966,533,466,500,466,1000,933,533,433,1000,500,500,466,500,933,1033,466,500,966,1000,933,1000,466};
	
	int sequence4[] ={533,566,366,566,400,600,366,600,400,566,400,566,433,533,433,566,400,566,400,600,400,533,466,533,433,533,433,533,466,500,466,500,466,500,500,500,500,466,500,500,466,500,466,500,500,500,466,966,1000,966,966,500,500,466,466,533,466,500,500,933,533,466,500,466,1000,966,1000,933,533,466,466,500,500,466,966,1000,966,1000,466,466,533,466,500,466,1000,466,500,966,1000,466,500,500,466,500,500,966,500,466,500,466,500,466,500,500,966,1000,500,466,500,500,466,466,1000,966,500,466,500,500,500,466,500,466,500,500,466,500,500,466,500,500,966,966,1000,933,500,500,500,466,1000,966,466,500,500,466,533,433,500,500,500,466,1000,966,500,466,500,466,500,500,966,966,1000,966,966,500,500,466,500,466,533,933,500,500,266};

	
	int sequence0[] = {520, 500, 420, 520, 400, 520, 440, 520, 400, 520, 400, 540, 400, 520, 440, 500, 440, 480, 440, 500, 440, 500, 460, 440, 460, 500, 420, 500, 440, 520, 420, 520, 420, 500, 460, 460, 480, 440, 480, 440, 500, 460, 480, 460, 460, 460, 480, 920, 980, 940, 960, 460, 500, 440, 500, 440, 480, 460, 480, 920, 500, 460, 500, 420, 980, 920, 980, 940, 480, 440, 500, 460, 460, 460, 960, 940, 980, 920, 500, 460, 480, 440, 480, 440, 960, 460, 520, 920, 960, 480, 480, 420, 500, 460, 480, 440, 500, 920, 480, 440, 500, 440, 500, 420, 500, 440, 500, 440, 980, 960, 440, 460, 480, 460, 500, 420, 520, 440, 480, 460, 480, 420, 500, 460, 480, 440, 500, 440, 500, 440, 480, 440, 480, 460, 980, 440, 480, 960, 960, 440, 500, 460, 460, 960, 460, 460, 500, 420, 980, 440, 500, 440, 500, 920, 960, 960, 960, 940, 980, 920, 960, 460, 500, 940, 960, 960, 460, 460, 960, 460, 480};
	
	printf("%s\n", convert(sequence1, sizeof(sequence1)/sizeof(sequence1[0])));
	printf("%s\n", convert(sequence2, sizeof(sequence2)/sizeof(sequence2[0])));
	printf("%s\n", convert(sequence3, sizeof(sequence3)/sizeof(sequence3[0])));
	printf("%s\n", convert(sequence4, sizeof(sequence4)/sizeof(sequence4[0])));
	return 0;
}
#endif
