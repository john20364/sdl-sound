#include <iostream>
#include <SDL2/SDL.h>
#include <string.h>

using namespace std;

void SDLAudioCallback(
		void *UserData, Uint8 *AudioData, int Length) {
	// Clear our audio buffer to silence.
	memset(AudioData, 0, Length);
}

int main(int argc, char **argv) {
	int SamplesPerSecond = 48000; // the sampling frequency of our audio device.
	int ToneHz = 256; // The frequency of the square wave.
	int ToneVolume = 3000; // The volume of our square wave: 32767 (int16) is loudest, 0 is silent.
	int SquareWavePeriod = SamplesPerSecond / ToneHz; // The period of our squarewave in samples.
	int HalfSquareWavePeriod = SquareWavePeriod / 2; // The number of samples for which the square wave 
													 // is held high or low.
	int BytesPerSample = sizeof(int16_t) * 2; // The number of bytes per stereo sample. (one sample for left
											  // one sample for right).

	Uint32 RunningSampleIndex = 0; // The index of the next sample we are going to generate. We need to 
	                               // preserve this across writes to our buffer, else the wave mightn't
								   // be consistent across write boundaries.

	// If out game is running at 60 frames per second, that's 800 samples (at 48000 Hz) per frame.
//	int BytesToWrite = 800 * BytesPerSample;
	int BytesToWrite = 48000 * BytesPerSample;

//	Uint32 BufferSize = 4096;

	SDL_AudioSpec AudioSettings = {0};
	AudioSettings.freq = SamplesPerSecond;
	AudioSettings.format = AUDIO_S16LSB;
	AudioSettings.channels = 2;
//	AudioSettings.samples = BufferSize / 2;
	//AudioSettings.callback = SDLAudioCallback;
	AudioSettings.callback = NULL;

	void *SoundBuffer = malloc(BytesToWrite);
	int16_t *SampleOut = (int16_t *)SoundBuffer;
	int SampleCount = BytesToWrite / BytesPerSample;

	for (int SampleIndex = 0; SampleIndex < SampleCount; ++SampleIndex) {
		int16_t SampleValue = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
		*SampleOut++ = SampleValue;
		*SampleOut++ = SampleValue;
	}

	SDL_OpenAudio(&AudioSettings, NULL);

	cout << "samples: " << AudioSettings.samples << endl;

	SDL_QueueAudio(1, SoundBuffer, BytesToWrite);

	free(SoundBuffer);

	if (AudioSettings.format != AUDIO_S16LSB) {
		cout << "Didn't get AUDIO_S16LSB" << endl;
	}

	SDL_PauseAudio(0);
	SDL_Delay(5000);
	SDL_CloseAudio();

	cout << "sdlsound" << endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Quit();
	return 0;
}
