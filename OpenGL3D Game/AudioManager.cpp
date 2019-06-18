#include "AudioManager.h"

CAudioManager::CAudioManager()
{

}

CAudioManager::~CAudioManager()
{
}
//Init the Audio at the start of the game
bool CAudioManager::AudioInit()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&audioSystem);
	if (result != FMOD_OK)
	return false;

	result = audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
	return false;

	return true;
}
//Takes in the Audiopath and Puts a Key to that path and sets the sound mode for that audio.
const bool CAudioManager::LoadAudio(std::string _AudioPath, int _KeyNum, FMOD_MODE _SoundMode)
{
	FMOD_RESULT result;

	FMOD::Sound* newSound;
	result = audioSystem->createSound(_AudioPath.c_str(), FMOD_DEFAULT, 0, &newSound);

	newSound->setMode(_SoundMode);
	m_Sounds[_KeyNum] = newSound;

	return (true);
}
//Play's the audio key and runs it multiple times
void CAudioManager::PlayAudioBackground(int _AudioKey)
{
	audioSystem->playSound(m_Sounds[_AudioKey], 0, false, &m_pBackground);
}
//Play's the audio key and only runs it once
void CAudioManager::PlayAudioSoundEffects(int _AudioKey)
{
	audioSystem->playSound(m_Sounds[_AudioKey], 0, false, &m_pSoundEffects);
}
//Stops and audio coming from the channels
void CAudioManager::StopAudio()
{
	m_pBackground->stop();
	m_pSoundEffects->stop();
}
//Updates the Audio each loop
void CAudioManager::Update()
{
	audioSystem->update();
}