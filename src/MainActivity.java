package com.example.julio.blindvoicer;

import android.content.Context;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;

public class MainActivity extends AppCompatActivity implements RequestListener {

    private boolean keepGoing = true;



    private double lastComand = 0.0;

    private Object[] params = null;

    private GetJson getJson = new GetJson();

    JsonDownloader jsonDownloader = new JsonDownloader(this,getContext());

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        jsonDownloader.execute();


    }

    public void Quit(View view){
        this.jsonDownloader.setVerifica(false);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    private Context getContext(){
        return this;
    }

    @Override
    public void onPositive() {
        if(jsonDownloader.getVerifica()) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    SdmSoundPlayer sdm = SdmSoundPlayer.getInstance(getContext());
                    sdm.playSound(SdmSoundPlayer.CONTINUE);
                }
            });
        }
    }

    @Override
    public void onNegative() {
        if (jsonDownloader.getVerifica()){
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    SdmSoundPlayer sdm = SdmSoundPlayer.getInstance(getContext());
                    sdm.playSound(SdmSoundPlayer.PARE);
                }
            });
        }
    }

    public void setLastComand(double lastComand){
        this.lastComand = lastComand;
    }

    public double getLastComand(){
        return this.lastComand;
    }

    public void startAgain(){
        jsonDownloader = new JsonDownloader(this, getContext());
        jsonDownloader.execute();
    }
}
