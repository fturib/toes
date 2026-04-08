const express = require('express');
const { google } = require('googleapis');
const cors = require('cors');
const app = express();

app.use(cors()); // Allows your Vue app to talk to this API
app.use(express.json());

// Load your Service Account credentials
const auth = new google.auth.GoogleAuth({
  keyFile: 'credentials.json', 
  scopes: ['https://www.googleapis.com/auth/spreadsheets.readonly'],
});

const sheets = google.sheets({ version: 'v4', auth });
const SPREADSHEET_ID = '1ZhK_DA8yjRrNYgK_bYiFMyRqSPJ9FJxpg6aOpctGqWA';

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

const PORT = process.env.PORT || 8080;
app.listen(PORT, () => console.log(`Server running on port ${PORT}`));