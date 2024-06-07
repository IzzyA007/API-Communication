#include <iostream>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void get_int_P(const char* endpoint, int value)
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        std::string data = std::to_string(value);

        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, endpoint);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}


int get_int_G(const char* endpoint)
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, endpoint);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return -1;
        }

        curl_easy_cleanup(curl);

        return std::stoi(buffer);
    }

    curl_global_cleanup();
    return -1;
}

int main() {
    std::cout << "Israel Akpati" << std::endl;
    std::cout << "R11712482" << std::endl;


    get_int_P("http://localhost:5000/initialize", 3360);


    get_int_P("http://localhost:5000/modify", 4);


    int val_initialize = get_int_G("http://localhost:5000/initialize");
    if (val_initialize != -1) {

        std::cout << "Value from initialize: " << val_initialize << std::endl;


        int val_mod = get_int_G("http://localhost:5000/modify");
        if (val_mod != -1) {

            std::cout << "Value from modify: " << val_mod << std::endl;

            //this sends the value received from modify #4 to initialize to the PUT
            get_int_P("http://localhost:5000/initialize", val_mod);

            //then I send the value received from initialize #3 to modify to the PUT
            get_int_P("http://localhost:5000/modify", val_initialize);

            //the data is then retrieved in initialize to a GET command and this will store as an integer
            int N_val_initialize = get_int_G("http://localhost:5000/initialize");
            if (N_val_initialize != -1) {
                //print this value to standard out stdout on its own line
                std::cout << "New value from initialize: " << N_val_initialize << std::endl;

                //this gets the data in modify to a GET command which will be an integer
                int N_val_mod = get_int_G("http://localhost:5000/modify");
                if (N_val_mod != -1) {
                  //Then the value to standard out the stdout to is going to be its own line
                    std::cout << "New value from modify: " << N_val_mod << std::endl;
                }
            }
        }
    }

    return 0;
}
