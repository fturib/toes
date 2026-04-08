#include "mbedtls/base64.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include <string>
#include <vector>

// --- Hardcoded Credentials (Extracted from your JSON) ---
const char *private_key_pem = "-----BEGIN PRIVATE KEY-----\n"
                              "MIIEvAIBADANBgkqhkiG9w0BAQEFAASC...\n"
                              "-----END PRIVATE KEY-----";

const char *client_email = "robot@project.iam.gserviceaccount.com";

// Helper: Base64 to Base64URL
void to_base64url(std::string &s) {
  size_t pos;
  while ((pos = s.find('+')) != std::string::npos)
    s.replace(pos, 1, "-");
  while ((pos = s.find('/')) != std::string::npos)
    s.replace(pos, 1, "_");
  while ((pos = s.find('=')) != std::string::npos)
    s.erase(pos);
}

// 3. Generate Signed JWT
std::string generate_jwt(uint32_t now) {
  mbedtls_pk_context pk;
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;

  mbedtls_pk_init(&pk);
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  // 1. Seed RNG and Load Key
  mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
  mbedtls_pk_parse_key(&pk, (const unsigned char *)private_key_pem,
                       strlen(private_key_pem) + 1, NULL, 0);

  // 2. Create Header.Payload
  std::string header = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
  char payload[256];
  snprintf(payload, sizeof(payload),
           "{\"iss\":\"%s\",\"scope\":\"https://www.googleapis.com/auth/"
           "spreadsheets\","
           "\"aud\":\"https://oauth2.googleapis.com/"
           "token\",\"exp\":%lu,\"iat\":%lu}",
           client_email, now + 3600, now);

  // Encode Parts
  unsigned char buf[1024];
  size_t out_len;

  mbedtls_base64_encode(buf, sizeof(buf), &out_len,
                        (unsigned char *)header.c_str(), header.length());
  std::string h_enc((char *)buf, out_len);
  to_base64url(h_enc);

  mbedtls_base64_encode(buf, sizeof(buf), &out_len, (unsigned char *)payload,
                        strlen(payload));
  std::string p_enc((char *)buf, out_len);
  to_base64url(p_enc);

  std::string unsigned_jwt = h_enc + "." + p_enc;

  // 3. Sign with RSA-SHA256
  unsigned char hash[32];
  mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
             (unsigned char *)unsigned_jwt.c_str(), unsigned_jwt.length(),
             hash);

  unsigned char sig[256];
  mbedtls_pk_sign(&pk, MBEDTLS_MD_SHA256, hash, 0, sig, &sig_len,
                  mbedtls_ctr_drbg_random, &ctr_drbg);

  mbedtls_base64_encode(buf, sizeof(buf), &out_len, sig, sig_len);
  std::string s_enc((char *)buf, out_len);
  to_base64url(s_enc);

  // Cleanup
  mbedtls_pk_free(&pk);
  return unsigned_jwt + "." + s_enc;
}

// 4. Main Loop/Task Logic
void run_upload_task(const char *station, int num_measures) {
  uint32_t current_time = get_sntp_time(); // You need an SNTP client running!

  std::string jwt = generate_jwt(current_time);

  // Build JSON Values
  std::string values = "{\"values\": [";
  for (int i = 1; i <= num_measures; i++) {
    int val = rand() % 101;
    uint32_t ts = current_time - (num_measures + i);
    char entry[128];
    snprintf(entry, sizeof(entry), "[\"%s\", %lu, %d]%s", station, ts, val,
             (i == num_measures ? "" : ","));
    values += entry;
  }
  values += "]}";

  // Perform HTTPS POST
  // Use your LwIP/mbedTLS HTTPS Client here to send 'values' to Google
}