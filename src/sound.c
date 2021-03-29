/*
 *
 * ao_example.c
 *
 *     Written by Stan Seibert - July 2001, Modified by Gautam Gupta - March 2021
 *
 * Legal Terms:
 *
 *     This source file is released into the public domain.  It is
 *     distributed without any warranty; without even the implied
 *     warranty * of merchantability or fitness for a particular
 *     purpose.
 *
 * Function:
 *
 *     This program opens the default driver and plays a 440 Hz tone for
 *     one second.
 *
 * Compilation command line (for Linux systems):
 *
 *     gcc -o ao_example ao_example.c -lao -ldl -lm
 *
 */

#include <stdio.h>
#include <string.h>
#include <ao/ao.h>
#include <math.h>
#include <sndfile.h>
//#include <gsound.h> - NEED TO ADD HEADER FILE

#define BUFFER_SIZE 8192

int sound_out(char *sound_name)
{
	SNDFILE *inputFile;
	SF_INFO inputFileInfo;
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	//short *buffer;
	char *buffer;
	int *out;
	int buf_size;
	int out_size;
	int sample;
	int i;

	/* -- Reading the .wav file -- */
	inputFile = sf_open(sound_name, SFM_READ, &inputFileInfo);

	/* -- Initialize ao player -- */

	fprintf(stderr, "Playing audio...\n");

	ao_initialize();

	/* -- Setup for default driver(in this case, AUDIO JACK - MUST BE MANUALLY SET USING sudo raspi-config) -- */

	default_driver = ao_default_driver_id();

        memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = inputFileInfo.channels;
	format.rate = inputFileInfo.samplerate;
	format.byte_format = AO_FMT_LITTLE;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}

	/* -- Play some stuff -- */
	buf_size = format.bits/8 * format.channels * format.rate;
	out_size = inputFileInfo.frames * inputFileInfo.channels;
	out = (int*) calloc(out_size, sizeof(int));
	buffer = (char*) calloc(buf_size, sizeof(char));
	//buffer = calloc(BUFFER_SIZE, sizeof(short));

	sf_readf_int(inputFile, out, inputFileInfo.frames);
	//int read = sf_read_short(inputFile, buffer, BUFFER_SIZE);

	for (i = 0; i < format.rate; i++) {
		//sample = (int)(0.75 * 32768.0 * sin(2 * M_PI * freq * ((float) i/format.rate)));
		sprintf(&buffer[i], "%d", out[i]);
		printf("buffer -> %s, out -> %i\n", &buffer[i], out[i]);
	}

	//sf_readf_int(inputFile, buffer, inputFileInfo.frames);
	ao_play(device, buffer, buf_size);

	/* -- Close everything -- */
	ao_close(device);
	sf_close(inputFile);
	ao_shutdown();
	free(buffer);
	free(out);

  return (0);
}
#ifdef SOUND_STANDALONE
int main(){
	char *sound = "/home/pi/Downloads/Wav_868kb.wav";
	sound_out(sound);
}
#endif
