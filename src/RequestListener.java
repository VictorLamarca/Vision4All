package com.example.julio.blindvoicer;

/**
 * Created by julio on 30/10/2016.
 */

public interface RequestListener {
    void onPositive();
    void onNegative();
    double getLastComand();
    void setLastComand(double lastComand);
    void startAgain();
}
