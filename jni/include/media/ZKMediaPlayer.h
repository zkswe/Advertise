#ifndef _MEDIA_ZKMEDIAPLAYER_H_
#define _MEDIA_ZKMEDIAPLAYER_H_

#include "system/Thread.h"
#include "utils/MessageQueue.h"
#include "control/Common.h"

namespace android {
class CedarMediaPlayer;
class CedarDisplay;
}

class InternalPlayerListener;

class ZKMediaPlayer {
	friend class InternalPlayerListener;
public:
	typedef enum {
		E_MEDIA_TYPE_AUDIO,
		E_MEDIA_TYPE_VIDEO
	} EMediaType;

	typedef enum {
		E_MSGTYPE_PLAY_FILE = 1,
		E_MSGTYPE_PAUSE_FILE,
		E_MSGTYPE_RESUME_FILE,
		E_MSGTYPE_SEEK_FILE,
		E_MSGTYPE_STOP_FILE,
		E_MSGTYPE_PLAY_STARTED,
		E_MSGTYPE_PLAY_COMPLETED,
		E_MSGTYPE_MEDIA_SET_VIDEO_SIZE,
		E_MSGTYPE_MEDIA_INFO, 			// MEDIA_INFO_UNKNOWN
		E_MSGTYPE_MEDIA_SEEK_COMPLETE,
		E_MSGTYPE_ERROR_MEDIA_ERROR,  	// MEDIA_ERROR_UNKNOWN, ...
		E_MSGTYPE_ERROR_UNKNOWN,
		E_MSGTYPE_ERROR_INVALID_FILEPATH,
		E_MSGTYPE_ERROR_PREPARE_FAIL,
		E_MSGTYPE_ERROR_PLAY_FAIL,
		E_MSGTYPE_QUIT_THREAD,
	} EMessageType;

	typedef struct {			// for MSGTYPE_ERROR_MEDIA_ERROR
		int mediaErrorType;		// enum media_error_type
		int extra;
	} SMediaErrorInfo;

public:
	ZKMediaPlayer(EMediaType mediaType);
	virtual ~ZKMediaPlayer();

	void play(const char *pFilePath, int msec = 0);
	void pause();
	void resume();
	void seekTo(int msec);
	void stop();

	bool isPlaying();

	void setVolume(float leftVolume, float rightVolume);
	void setPreviewPos(const LayoutPosition &pos);

	int getDuration();
	int getCurrentPosition();

	class IPlayerMessageListener {
	public:
		virtual ~IPlayerMessageListener() { }
		virtual void onPlayerMessage(ZKMediaPlayer *pMediaPlayer, int msg, void *pMsgData) = 0;
	};

	void setPlayerMessageListener(IPlayerMessageListener *pListener) {
		mPlayerMessageListenerPtr = pListener;
	}

private:
	bool playerThreadLoop();
	bool executePlayFile(const char *pFilePath, int msec = 0);
	void resetPlayer();

	enum EPlayState {
		E_PLAY_STATE_ERROR = -1,
		E_PLAY_STATE_IDLE = 0,
		E_PLAY_STATE_INITIALIZED = 1,
		E_PLAY_STATE_STARTED = 4,
		E_PLAY_STATE_PAUSED = 5,
	};

	class PlayerThread : public Thread {
	public:
		PlayerThread(ZKMediaPlayer &mp) : mPlayer(mp) { }

		void start();
		void stop();
		virtual bool threadLoop();

	private:
		ZKMediaPlayer &mPlayer;
	};

private:
	PlayerThread mPlayerThread;
	InternalPlayerListener *mInternalPlayerListenerPtr;

	EMediaType mMediaType;

	android::CedarMediaPlayer *mMediaPlayerPtr;
	android::CedarDisplay *mDisplayPtr;
	int mHLay;

	MessageQueue mMsgQueue;

	mutable Mutex mLock;
	mutable Mutex mPlayerListenerLock;

	Condition mExecuteDoneCondition;

	EPlayState mCurrentPlayState;

	IPlayerMessageListener *mPlayerMessageListenerPtr;
};

#endif  /* _MEDIA_ZKMEDIAPLAYER_H_ */
