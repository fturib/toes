#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

// --- Configuration ---
#define CLIENT_EMAIL "your-service-account@project.iam.gserviceaccount.com"
#define SHEET_ID "your_spreadsheet_id"
#define PEM_FILE "private.pem"

// Helper to Base64URL encode (Simplified for POC)
void base64url_encode(const unsigned char *input, int length, char *output) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *mem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, mem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);
    
    memcpy(output, bptr->data, bptr->length);
    output[bptr->length] = '\0';

    // Convert Base64 to Base64URL
    for (int i = 0; output[i]; i++) {
        if (output[i] == '+') output[i] = '-';
        if (output[i] == '/') output[i] = '_';
        if (output[i] == '=') { output[i] = '\0'; break; }
    }
    BIO_free_all(b64);
}

// Function to get Access Token
char* get_access_token() {
    // 1. Create JWT Header and Payload
    char header[] = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
    long now = time(NULL);
    char payload[512];
    snprintf(payload, sizeof(payload), 
        "{\"iss\":\"%s\",\"scope\":\"https://www.googleapis.com/auth/spreadsheets\",\"aud\":\"https://oauth2.googleapis.com/token\",\"exp\":%ld,\"iat\":%ld}",
        CLIENT_EMAIL, now + 3600, now);

    char h_enc[100], p_enc[1024];
    base64url_encode((unsigned char*)header, strlen(header), h_enc);
    base64url_encode((unsigned char*)payload, strlen(payload), p_enc);

    char unsigned_jwt[2048];
    snprintf(unsigned_jwt, sizeof(unsigned_jwt), "%s.%s", h_enc, p_enc);

    // 2. Sign with Private Key
    FILE *fp = fopen(PEM_FILE, "r");
    EVP_PKEY *privKey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);

    unsigned char sig[512];
    unsigned int sig_len;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_SignInit(mdctx, EVP_sha256());
    EVP_SignUpdate(mdctx, unsigned_jwt, strlen(unsigned_jwt));
    EVP_SignFinal(mdctx, sig, &sig_len, privKey);
    
    char s_enc[1024];
    base64url_encode(sig, sig_len, s_enc);

    char assertion[4096];
    snprintf(assertion, sizeof(assertion), "%s.%s", unsigned_jwt, s_enc);

    // 3. Exchange for token via libcurl
    CURL *curl = curl_easy_init();
    char *response = malloc(4096);
    char post_fields[5000];
    snprintf(post_fields, sizeof(post_fields), "grant_type=urn:ietf:params:oauth:grant-type:jwt-bearer&assertion=%s", assertion);

    curl_easy_setopt(curl, CURLOPT_URL, "https://oauth2.googleapis.com/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    // Note: In a real app, you'd parse the JSON response here to extract "access_token"
    // For this POC, we'll return the raw response string.
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    return assertion; // This returns the JWT; usually you'd perform the POST here.
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <station> <count>\n", argv[0]);
        return 1;
    }

    const char *station = argv[1];
    int count = atoi(argv[2]);

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Build the "Values" JSON string
    char rows[8192] = "";
    time_t now = time(NULL);
    for (int i = 1; i <= count; i++) {
        char timestamp[30];
        time_t offset_time = now - (count + i);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&offset_time));
        
        char row[256];
        int val = rand() % 101;
        snprintf(row, sizeof(row), "[\"%s\", \"%s\", %d]%s", 
                 station, timestamp, val, (i < count ? "," : ""));
        strcat(rows, row);
    }

    char payload[9000];
    snprintf(payload, sizeof(payload), "{\"values\": [%s]}", rows);

    // --- EXECUTION ---
    // 1. Get Token (Placeholder: in C, you'd chain the CURL calls)
    // 2. POST to Google Sheets API
    printf("Payload generated:\n%s\n", payload);

    curl_global_cleanup();
    return 0;
}