import time
import jwt
import json
import requests

def get_access_token():
    # Load your service account key
    with open("credentials.json") as f:
        config = json.load(f)

    private_key = config["private_key"]
    client_email = config["client_email"]
    token_url = config["token_uri"]

    now = int(time.time())

    payload = {
        "iss": client_email,
        "scope": "https://www.googleapis.com/auth/spreadsheets",
        "aud": token_url,
        "exp": now + 3600,
        "iat": now
    }

    # Sign and encode the JWT
    signed_jwt = jwt.encode(payload, private_key, algorithm="RS256")

    # Exchange for the actual Access Token
    response = requests.post(token_url, data={
        "grant_type": "urn:ietf:params:oauth:grant-type:jwt-bearer",
        "assertion": signed_jwt
    })
    
    return response.json().get("access_token")

if __name__ == "__main__":
    print(get_access_token())