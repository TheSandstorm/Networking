#ifndef AUDIO_H
#define AUDIO_H

#include <map>
#include <fmod.hpp>

class CAudioManager
{
public:
	CAudioManager();
	~CAudioManager();

	bool AudioInit();
	const bool LoadAudio(std::string _AudioPath, int _KeyName, FMOD_MODE _SoundMode);
	void PlayAudioBackground(int _AudioKey);
	void PlayAudioSoundEffects(int _AudioKey);
	void StopAudio();

	void Update();
private:
	FMOD::System* audioSystem;
	FMOD::Channel* m_pBackground;
	FMOD::Channel* m_pSoundEffects;

	std::map<int, FMOD::Sound*> m_Sounds;
};

#endif
