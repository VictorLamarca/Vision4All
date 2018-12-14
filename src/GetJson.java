package com.example.julio.blindvoicer;

/**
 * Created by julio on 30/10/2016.
 */

import java.io.IOException;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class GetJson {

    private static String URL = "https://data.sparkfun.com/output/7JKYyK4Zw1FLpaX20JyN.json";

    OkHttpClient client = new OkHttpClient();

    String run(String url) throws IOException {
        Request request = new Request.Builder()
                .url(url)
                .build();

        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
    }

    public static void main(String[] args) throws IOException {
        GetJson example = new GetJson();
        String response = example.run("https://data.sparkfun.com/output/7JKYyK4Zw1FLpaX20JyN.json");
        System.out.println(response);
    }
}