#!/bin/bash
# Check if arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <station_name> <number_of_measures>"
    exit 1
fi

STATION_NAME=$1
NUM_MEASURES=$2


# 1. Grab a fresh token using the Python script
TOKEN=$(/usr/local/opt/python@3.9/bin/python3.9 google_token.py)

# 2. Configuration
SHEET_ID="1ZhK_DA8yjRrNYgK_bYiFMyRqSPJ9FJxpg6aOpctGqWA"
RANGE="DATA!B2"
IDDATA="SouthRiver-1"

# 3. Generate the data rows
# We build a JSON-formatted list of rows
ROWS=""
for (( i=1; i<=$NUM_MEASURES; i++ ))
do
    OFFSET=$(( NUM_MEASURES - i ))
    # Note: Using -u for UTC consistency
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS version
        TIMESTAMP=$(date -u -v-"${OFFSET}"S +"%Y-%m-%d %H:%M:%S")
    else
        # Linux/Ubuntu version
        TIMESTAMP=$(date -u -d "$OFFSET seconds ago" +"%Y-%m-%d %H:%M:%S")
    fi
    VALUE=$(( RANDOM % 101 )) # Generates 0-100
    
    # Format the row as a JSON array
    ROW="[\"$STATION_NAME\", \"$TIMESTAMP\", $VALUE]"
    
    # Add a comma if it's not the last row
    if [ $i -lt $NUM_MEASURES ]; then
        ROWS="$ROWS$ROW,"
    else
        ROWS="$ROWS$ROW"
    fi
done
# ["'$IDDATA'","'$(date "+%Y-%m-%d %H:%M:%S")'", "3.56"]
# 4. Construct the full payload
DATA_PAYLOAD="{
  \"values\": [ $ROWS ]
}"

# 4. The Raw API Call
curl --request POST \
  "https://sheets.googleapis.com/v4/spreadsheets/$SHEET_ID/values/$RANGE:append?valueInputOption=USER_ENTERED" \
  --header "Authorization: Bearer $TOKEN" \
  --header "Content-Type: application/json" \
  --data "$DATA_PAYLOAD"

echo -e "\nUpload complete."