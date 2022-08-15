#pragma once

namespace Burst {
	typedef irrklang::ISound* Sound;

	namespace Audio {
		irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

		std::map<std::string, Sound> soundEvents;

		Sound Play(std::string filepath, bool loop=false) {
			return SoundEngine->play2D(filepath.c_str(), loop, false, true);
		}

		void Stop(Sound sound) {
			sound->stop();
			sound->drop();
		}

		void SetSpeed(Sound sound, float speed=1.0f) {
			sound->setPlaybackSpeed(speed);
		}

		Sound PlaySoundEvent(std::string filepath, std::string evt, bool loop = false) {
			if (soundEvents.contains(evt)) {
				soundEvents[evt]->stop();
				soundEvents[evt]->drop();
			}
			soundEvents[evt] = SoundEngine->play2D(filepath.c_str(), loop, false, true);
			return soundEvents[evt];
		}

		void StopSoundEvent(std::string evt) {
			if (soundEvents.contains(evt)) {
				soundEvents[evt]->stop();
				soundEvents[evt]->drop();
			}
		}

		void SetSoundEventSpeed(std::string evt, float speed = 1.0f) {
			if (soundEvents.contains(evt)) {
				soundEvents[evt]->setPlaybackSpeed(speed);
			}
		}

		void StopAll() {
			SoundEngine->stopAllSounds();
		}
	}
}