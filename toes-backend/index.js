const express = require('express');
const { google } = require('googleapis');
const cors = require('cors');
const app = express();

app.use(cors()); // Allows your Vue app to talk to this API
app.use(express.json());

// Load your Service Account credentials
const auth = new google.auth.GoogleAuth({
  keyFile: 'credentials.json', 
  scopes: ['https://www.googleapis.com/auth/spreadsheets'],
});
  
const sheets = google.sheets({ version: 'v4', auth });
const SPREADSHEET_ID = '1ZhK_DA8yjRrNYgK_bYiFMyRqSPJ9FJxpg6aOpctGqWA';

// Helper function to format date as yyyy-mm-dd hh:mm:ss
function formatDate(date) {
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, '0');
  const day = String(date.getDate()).padStart(2, '0');
  const hours = String(date.getHours()).padStart(2, '0');
  const minutes = String(date.getMinutes()).padStart(2, '0');
  const seconds = String(date.getSeconds()).padStart(2, '0');
  return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

app.get('/data', async (req, res) => {
  try {
    const range = 'DATA!B:D'; // Adjust based on your sheet
    const response = await sheets.spreadsheets.values.get({
      spreadsheetId: SPREADSHEET_ID,
      range: range,
    });
    res.send(response.data.values);
  } catch (error) {
    res.status(500).send(error.message);
  }
});

app.post('/turbidity', async (req, res) => {
  try {
    // Validate the request body
    if (!req.body.station || !req.body.turbidity || typeof req.body.turbidity.value !== 'number') {
      return res.status(400).send('Invalid request body: missing station or turbidity.value');
    }

    const station = req.body.station;
    const turbidityValue = req.body.turbidity.value;
    const currentDateTime = formatDate(new Date());
    const range = 'DATA!B:D'; // Append station, date/time, turbidity value
    const values = [[station, currentDateTime, turbidityValue]];
    const response = await sheets.spreadsheets.values.append({
      spreadsheetId: SPREADSHEET_ID,
      range: range,
      valueInputOption: 'USER_ENTERED', 
      // USER_ENTERED will allow us to use the date format in the sheet and it will be converted to a date type in the sheet.
      // alternatively we can use RAW as an option but all data will be kept as-is, as Strings. 
      resource: { values },
    });
    res.send({ success: true });
  } catch (error) {
    res.status(500).send(error.message);
  }
});

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));