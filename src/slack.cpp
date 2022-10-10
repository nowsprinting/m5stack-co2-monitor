//
// Created by Koji Hasegawa on 2022/10/10.
//

#include "slack.h"
#include <Arduino.h>
#include <HTTPClient.h>

void notifySlack(String message, bool here) {
    String body;
    if (here) {
        body = "{\"text\":\"<!here> " + message + "\"}";
    } else {
        body = "{\"text\":\"" + message + "\"}";
    }

    HTTPClient httpClient;
    httpClient.begin(SLACK_WEBHOOK_URL);
    httpClient.addHeader("Content-Type", "application/json");

    int status = httpClient.POST(body);
    if (status == 200) {
        String response = httpClient.getString();
        Serial.printf("Slack notification send\n");
    } else {
        Serial.printf("Slack send error! status code: %d\n", status);
    }
    httpClient.end();
}
