/* Copyright (C) 2021 Gautam Gupta
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <ao/ao.h>
#include <math.h>
#include <sndfile.h>
#include <gsound.h>

int sound_out(char *sound_name)
{
	SNDFILE *inputFile;
	SF_INFO inputFileInfo;
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	int *out;
	int buf_size;
	int out_size;
	int sample;
	int i;

	/* -- Reading the .wav file -- */
	inputFileInfo.format = 0;
	inputFile = sf_open(sound_name, SFM_READ, &inputFileInfo);

	/* -- Initialize ao player -- */

	fprintf(stderr, "Playing audio...\n");

	ao_initialize();

	/* -- Setup for default driver(in this case, AUDIO JACK - MUST BE MANUALLY SET USING sudo raspi-config) -- */

	default_driver = ao_default_driver_id();

        memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = inputFileInfo.channels;
	//printf("Number of channels -> %i\n", format.channels);
	format.rate = inputFileInfo.samplerate;
	format.byte_format = AO_FMT_LITTLE;
	int num_frames = inputFileInfo.frames;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL); /* no options */
	if (device == NULL) {
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}

	/* -- Play some stuff -- */
	//buf_size = format.bits/8 * format.channels * format.rate;
	buf_size = num_frames * inputFileInfo.channels;
	out = (int*) calloc(buf_size, sizeof(int));
	buffer = (char*) calloc(buf_size, sizeof(int));
	//buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
	//out = (int*) calloc(BUFFER_SIZE, sizeof(int));

	sf_readf_int(inputFile, out, num_frames);
	//sf_readf_int(inputFile, out, BUFFER_SIZE);
	//int read = sf_read_short(inputFile, buffer, BUFFER_SIZE);

	/* -- Copying the contents of the out array into buffer to get it into libao playable format --*/
	for (i = 0; i < buf_size; i++) {
		sprintf(&buffer[i], "%d", out[i]);
		//printf("buffer -> %s, out -> %i\n", &buffer[i], out[i]);
	}
	//printf("buffer size -> %i, output buffer size -> %i\n", buf_size, out_size);
	ao_play(device, buffer, buf_size); //Actually plays the sound here

	/* -- Close everything -- */
	ao_close(device);
	sf_close(inputFile);
	ao_shutdown();
	free(buffer);
	free(out);
  	return (0);
}

int sndcon(char *sound_name){ //Uncomment and edit to make it run in the background
	if (strlen(sound_name) < 290){
		char command[300] = "omxplayer "; //--no-keys ";
		strcat(command, sound_name);
		strcat(command, "&");
		system(command);
		return (0);
	} else {
		fprintf(stderr, "ERROR! MP3 File name too long! Please rectify and try again!");
		return (1);
	}
}

#ifdef SOUND_STANDALONE
int main(){
	char *sound = "/home/pi/Downloads/Wav_868kb.wav";
	//char *sound = "/home/pi/Downloads/Soft Piano Music_16000_mono.wav";
	//sound_out(sound);
	sndcon(sound);
}
#endif
