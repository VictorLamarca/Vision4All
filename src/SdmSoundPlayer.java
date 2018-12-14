package com.example.julio.blindvoicer;

import android.annotation.SuppressLint;
import android.content.Context;
import android.media.MediaPlayer;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.accessibility.AccessibilityManager;


import java.util.HashMap;


@SuppressLint("UseSparseArrays")
public class SdmSoundPlayer {

    public static final int DEFAULT_WAIT_TIME_WITH_SFX_DISABLED = 2500;

    private static final String TAG = "SdmSoundPlayer";

    private static int mSFXidx = 1;


    private static final String PROP_SOUND_FX_ENABLED = "is_sound_fx_enabled";

    private Context mContext;

    private Handler mHandler = null;

    public static int PARE = 0;
    public static int CONTINUE = 1;

    private HashMap<Integer, int[]> mSoundHash = null;

    private MediaPlayer mMediaPlayer = null;
    private MediaPlayer mMediaPlayer2 = null;

    private boolean mIsAccessibiltyEnabled = false;

    private static SdmSoundPlayer mInstance;

    private SdmSoundPlayer(Context context) {
        mContext = context;
        mHandler = new Handler();
        initSoundHash();

        AccessibilityManager am = (AccessibilityManager) context.getSystemService(Context.ACCESSIBILITY_SERVICE);
        mIsAccessibiltyEnabled = am.isEnabled();
    }

    public static SdmSoundPlayer getInstance(Context context) {
        if (mInstance == null) {
            mInstance = new SdmSoundPlayer(context);
        }
        // Update context
        mInstance.mContext = context;
        return mInstance;
    }

    private void initSoundHash() {

        if (mSoundHash == null) {
            mSoundHash = new HashMap<Integer, int[]>(50);
        }

        mSoundHash.put(PARE, new int[] {R.raw.pare}); //pare
        mSoundHash.put(CONTINUE, new int[] {R.raw.continuando}); //pare

    }

    public boolean isSoundEnabled() {
        return PreferenceManager.getDefaultSharedPreferences(mContext).getBoolean(PROP_SOUND_FX_ENABLED, true);
    }

    public void playSound(int key, boolean stopCurrent, boolean loop, final MediaPlayer.OnCompletionListener listener) {

        if (!isSoundEnabled()) {
            if (listener != null) {
                // If sound is disabled but UI requested to play a song with a callback
                // that means that the UI is waiting the sound to finish in order to do
                // some action. We must guarantee that behavior and the listener must
                // called.
                mHandler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        listener.onCompletion(mMediaPlayer);
                    }
                }, DEFAULT_WAIT_TIME_WITH_SFX_DISABLED);
            }
            Log.i(TAG, "Sound FX is disabled. Skipping sound " + key);
            return;
        }

        MediaPlayer player = null;

        // Get the target array of sounds
        int[] sfxArray = mSoundHash.get(key);

        // Get the sfx resource id
        int sfxResID = -1;
        if (sfxArray != null) {
            // Get the current selected SFX set
            int sfxSetIdx = 1; //PreferenceManager.getDefaultSharedPreferences(mContext).getInt(SdmSharedPreferences.PREF_SFX_SET, 0);
            // If there are variation according to the selected set, get it. Otherwise, get the default sound (idx == 0)
            sfxResID = sfxArray[(sfxArray.length > sfxSetIdx) ? sfxSetIdx : 0];
        }

        if (sfxResID > 0) {

            if (mMediaPlayer == null || stopCurrent) {
                // No sound playing or the current one must be stopped. Use the main player anyway.
                if (mMediaPlayer != null) {
                    mMediaPlayer.reset();
                }
                mMediaPlayer = MediaPlayer.create(mContext, sfxResID);
                player = mMediaPlayer;
            }
            else {
                if (mMediaPlayer2 != null) {
                    mMediaPlayer2.reset();
                }
                // Should use the secondary player to do not stop the current one.
                mMediaPlayer2 = MediaPlayer.create(mContext, sfxResID);
                player = mMediaPlayer2;
            }

            try {
                player.setLooping(loop);
                player.start();
                if (mIsAccessibiltyEnabled) {
                    // Set game volume to 80% to focus more on talkback voice
                    float log1=(float)(Math.log(100-80)/Math.log(100));
                    player.setVolume(1-log1, 1-log1);
                }
                if (listener != null) {
                    player.setOnCompletionListener(listener);
                }
            }
            catch (Exception ex) {
                Log.w(TAG, "Could not play sound: " + ex.getMessage());
            }
        }
    }

    public void playSound(int key, MediaPlayer.OnCompletionListener listener) {
        playSound(key, true, false, listener);
    }

    public void playSound(int key, boolean stopCurrent) {
        playSound(key, stopCurrent, false, null);
    }

    public void playSound(int key) {
        playSound(key, null);
    }

    public void stopAllSounds() {
        if ((mMediaPlayer != null) && (mMediaPlayer.isPlaying())) {
            mMediaPlayer.stop();
        }
        if ((mMediaPlayer2 != null) && (mMediaPlayer2.isPlaying())) {
            mMediaPlayer2.stop();
        }
    }

    public void release() {
        if (mMediaPlayer != null) {
            mMediaPlayer.release();
        }
        if (mMediaPlayer2 != null) {
            mMediaPlayer2.release();
        }
    }

    public void switchSoundOnOff(boolean isEnabled) {
        PreferenceManager.getDefaultSharedPreferences(mContext)
                .edit()
                .putBoolean(PROP_SOUND_FX_ENABLED, isEnabled).commit();
    }
}