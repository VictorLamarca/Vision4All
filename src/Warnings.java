package com.example.julio.blindvoicer;

import com.google.gson.Gson;
import com.google.gson.annotations.SerializedName;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;

/**
 * Created by julio on 30/10/2016.
 */
public class Warnings {

    @SerializedName("comands")
    Double warning_value;

    @SerializedName("timestamp")
    String timestamp;

    public static Warnings[] getWarnings(String json) throws IOException {
        Gson gson = new Gson();
        Warnings[] warnings = gson.fromJson(json, Warnings[].class);
        return warnings;
    }


}