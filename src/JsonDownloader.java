package com.example.julio.blindvoicer;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.AsyncTask;

import java.io.IOException;

import static java.security.AccessController.getContext;

/**
 * Created by julio on 30/10/2016.
 */

public class JsonDownloader extends AsyncTask {

    private RequestListener requestListener;

    private String ultimo = "ultimo";

    //private SharedPreferences preferences;

    private boolean verifica = true;
    private Context mContext;

    JsonDownloader(RequestListener requestListener, Context context){
        this.requestListener = requestListener;
        mContext = context;
    }

    @Override
    protected Object doInBackground(Object[] params) {
        final GetJson example = new GetJson();
        try {
            SharedPreferences preferences = mContext.getSharedPreferences("my_prefs", Context.MODE_PRIVATE);
            double lastReadValue = preferences.getFloat(ultimo, 0);
            String response = example.run("https://data.sparkfun.com/output/7JKYyK4Zw1FLpaX20JyN.json");
            Warnings[] warnings = Warnings.getWarnings(response);
            float lastValue = warnings[0].warning_value.floatValue();
            System.out.println(warnings.toString());
            SharedPreferences.Editor editor = preferences.edit();
            editor.putFloat(ultimo, lastValue);
            editor.commit();


            if(lastReadValue < lastValue){
                this.requestListener.onNegative();
                this.requestListener.setLastComand(warnings[0].warning_value);

            }else{
                this.requestListener.onPositive();

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    protected void onPostExecute(Object o) {
        super.onPostExecute(o);
        if(this.verifica) {
            requestListener.startAgain();
        }
    }

    protected void execute(){
        if (this.verifica) {
            super.execute();
        }else{

        }
    }

    public void setVerifica(boolean verifica){
        this.verifica = verifica;
    }

    public boolean getVerifica(){
        return verifica;
    }


}
