#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <curl/curl.h>

#define BOT 3
#define USER 2
#define WEBHOOK 1

char* resdata;


struct ResponseData {
	char* data;
	size_t size;
};

size_t write_data(void* ptr, size_t size, size_t nmemb, struct ResponseData* response) {
	size_t total_size = size * nmemb;
	response->data = (char*)realloc(response->data, response->size + total_size + 1);
	if (response->data == NULL) {
		fprintf(stderr, "Failed to allocate memory for response data.\n");
		return 0;
	}
	memcpy(response->data + response->size, ptr, total_size);
	response->size += total_size;
	response->data[response->size] = '\0';
	return total_size;
}

void Send_message(int bot, char* channelID = NULL, char* content = NULL, char* username = NULL, char* token= NULL)
{

    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL;
    struct ResponseData response_data = { NULL, 0 };

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    char useragent[] = "User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 121.0.0.0 Safari / 537.36";
    char data[256];
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // 기본 헤더
    headers = curl_slist_append(headers, "'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.7',");
    headers = curl_slist_append(headers, "Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7");
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: none");
    headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");

    headers = curl_slist_append(headers, useragent);

    char url[256];

    if (bot == BOT)
    {

        char temp[500];
        sprintf(temp, "authorization: Bot %s", token); // token을 포함한 새로운 문자열 생성
        sprintf(data, "{\"content\":\"%s\"}", content); // JSON 만들기
        strcpy(url,"https://ptb.discord.com/api/v9/channels/");
        strcat(url, channelID);
        strcat(url, "/messages");
        headers = curl_slist_append(headers, temp); // 새로운 문자열을 headers에 추가

    }

    else if (bot == USER)
    {
        char temp[500];
        sprintf(data, "{\"content\":\"%s\"}", content); // JSON 만들기
        strcpy(url, "https://ptb.discord.com/api/v9/channels/");
        strcat(url, channelID);
        strcat(url, "/messages");

        sprintf(temp, "authorization: %s", token); // token을 포함한 새로운 문자열 생성
        headers = curl_slist_append(headers, temp);
        

    }

    else if (bot == WEBHOOK)
    {
        strcpy(url, channelID);
        sprintf(data, "{\"username\":\"%s\",\"content\":\"%s\"}", username, content); // JSON 만들기
    }

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl); // POST 요청 보내기

        // 응답 코드가 200번대가 아니라면,
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); // 오류 코드 출력
        
        resdata = (char*)malloc(response_data.size + 1);
        if (resdata == NULL) {
            fprintf(stderr, "Failed to allocate memory for resdata.\n");
            return;
        }

        resdata = response_data.data;

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    printf("%s\n", resdata);
    return;
}

void get_token(char* email, char* password) {

	CURL* curl;
	CURLcode res;
	struct curl_slist* headers = NULL;
	struct ResponseData response_data = { NULL, 0 };

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();

	char useragent[] = "User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 121.0.0.0 Safari / 537.36";
	char data[256];

	const char url[] = "https://discord.com/api/v9/auth/login";
	if (curl) {
		sprintf(data, "{\"login\":\"%s\",\"password\":\"%s\"}", email, password); // JSON 만들기

		// 헤더 추가
		headers = curl_slist_append(headers, "Content-Type: application/json");

		// 기본 헤더
		headers = curl_slist_append(headers, "'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/;q=0.8,application/signed-exchange;v=b3;q=0.7',");
		headers = curl_slist_append(headers, "Accept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7");
		headers = curl_slist_append(headers, "Cache-Control: max-age=0");
		headers = curl_slist_append(headers, "Connection: keep-alive");
		headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
		headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
		headers = curl_slist_append(headers, "Sec-Fetch-Site: none");
		headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
		headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
		headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");

		headers = curl_slist_append(headers, useragent);

		// 헤더 설정
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		/* POST 데이터로 JSON 데이터를 사용합니다 */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		/* 요청할 URL을 설정합니다 */
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// 응답 데이터 처리를 위한 콜백 함수 등록
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

		res = curl_easy_perform(curl); // POST 요청 보내기

		// 응답 코드가 200번대가 아니라면,
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); // 오류 코드 출력

		resdata = (char*)malloc(response_data.size + 1);
		if (resdata == NULL) {
			fprintf(stderr, "Failed to allocate memory for resdata.\n");
			return;
		}

        resdata = response_data.data;

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

    curl_global_cleanup();

    printf("%s\n", resdata);

    return;
}
