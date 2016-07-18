#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

void SDLAudioCallback(
		void *UserData, Uint8 *AudioData, int Length) {
	// Clear our audio buffer to silence.
	memset(AudioData, 0, Length);
}

int main(int argc, char **argv) {
	int SamplesPerSecond = 48000;
	int ToneHz = 256;
	int ToneVolume = 3000;
	int SquareWavePeriod = SamplesPerSecond / ToneHz;
	Uint32 RunningSampleIndex = 0;
	Uint32 BufferSize = 4096;

	SDL_AudioSpec AudioSettings = {0};
	AudioSettings.freq = SamplesPerSecond;
	AudioSettings.format = AUDIO_S16LSB;
	AudioSettings.channels = 2;
	AudioSettings.samples = BufferSize;
	AudioSettings.callback = SDLAudioCallback;

	SDL_OpenAudio(&AudioSettings, NULL);

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
