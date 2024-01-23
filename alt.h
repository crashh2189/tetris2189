#include <iostream>

#include "discord.h"



#pragma once
void Alt_bruteforce_minecraft()
{


    int Alt_num[20] = { 0 };

    char Alt_chr[63] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    char Alt_code[24];

    int count = 0;

    std::string Alt_content;

    while (1) {

        if (count % 15 == 0) Sleep(3000);

        int k = 0;
        
        for (int i = 0; i < 20; i++) {
            if (i % 5 == 0 && i != 0) {
                Alt_code[k] = '-';
                k++;
            }
            Alt_code[k] = Alt_chr[Alt_num[i]];
            k++;
        }
        Alt_code[23] = '\0'; // Null terminator for proper string printing

        printf("%s\n", Alt_code);

        std::string Alt_content(Alt_code);
        
        Alt_content = "`" + Alt_content + "`";

        
        


        //Send_webhook(L"/api/webhooks/1197086126394069105/JDaoVFVmLxF1N6SX012iu5MQdiIwzKvEFswaOoKJnLjc3e8tNjy76K1e-2agz4RV32oP", "Minecraft Alt Generator",Alt_content);


        int i = 0;
        while (i < 23) {
            if (Alt_num[i] < 62) {
                Alt_num[i]++;
                break;
            }
            else {
                Alt_num[i] = 0;
                i++;
            }

            if (i == 23) {
                return;
            }
        }
        
        count++;
    }
}