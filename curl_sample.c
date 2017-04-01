#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct Buffer {
    char *data;
    int data_size;
};

size_t buffer_writer(char *ptr, size_t size, size_t nmemb, void *stream) {
    struct Buffer *buf = (struct Buffer *)stream;
    int block = size * nmemb;
    if (!buf) {
        return block;
    }

    if (!buf->data) {
        buf->data = (char *)malloc(block);
    }
    else {
        buf->data = (char *)realloc(buf->data, buf->data_size + block);
    }

    if (buf->data) {
        memcpy(buf->data + buf->data_size, ptr, block);
        buf->data_size += block;
    }

    return block;
}

int main(int argc, char **argv) {

    CURL *curl;
    struct Buffer *buf;
    char *url = "http://www.goole.com/";
    char *proxy_url = NULL;
    CURLcode res;

    if ( argc > 1 )
    {
        url = argv[1];
    }
    if ( argc > 2 )
    {
        proxy_url = argv[2];
    }

    buf = (struct Buffer *)malloc(sizeof(struct Buffer));
    buf->data = NULL;
    buf->data_size = 0;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buffer_writer);
    if ( proxy_url )
    {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy_url);
    }
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if ( CURLE_OK != res )
    {
        printf("ERROR:%d\n", res);
    }
    printf("%s\n", buf->data);

    free(buf->data);
    free(buf);

    return EXIT_SUCCESS;
}

