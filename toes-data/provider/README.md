# TOES - Doigts-de-pieds Project

## Block Schema

Currently available [in this Google presentation](https://docs.google.com/presentation/d/1t1adQOfSyYh1Vk9-PvZom8S7t6_nnRyFWVspny9nIpk/edit?slide=id.g3870771aecb_3_6#slide=id.g3870771aecb_3_6)

## Current Implementation

### Embeded on the Turpidity Device : toes-embedded

The device include the micro-controller STM32, and several periphericals:

- the Turbidity probe itself - communication with UART (rs232 ? - maybe in 0-5v)
- the Blue NoteCard for Wifi / Data cell to Cloud BlueNote - communication with UART (rs232 ? - maybe in 0-5v)
- optional user In/Out at least for test purposes (true ?)

### Database DB: toes-data

It is a simple [google sheet](https://docs.google.com/spreadsheets/d/1ZhK_DA8yjRrNYgK_bYiFMyRqSPJ9FJxpg6aOpctGqWA/edit?gid=0#gid=0) hosted in Google Drive of fturib

- Access through a GCP ServiceAccount (need to get credential.json by asking <fturib@gmail.com>) that have an access Editor to the spreadsheet.

To be transformed into **Firestone** noSQL DB under GCP

#### Data Provider : uploading data to the DB

The final client is the Probe Device itself (including SRM32 + Blue NoteCard)
for testing purpose we need some client simulation to fill the DB

### Backend API : toes-backend

A Express HTTP server run under Nodejs, hosted on GCP Google Run

- Used as an interface between DB and Blues, DB and WebApps
- Also used to admin the application (customers, stations, dedicated calibration sessions ..etc)

### WebApp UI : toes-webapp

A Single Page Application based on Vue framework, hosted in Google's CDN : Firebase

- use toes-backend to query data from DB
- display basic information on the client navigator.

## Usage

See the README doc of each folders.
Some of the components need communication with GCP or Google Account

- That usually requires a credentials.json file that contains ServiceAccount credentials.
- Ask <fturib@gmail.com> to get credentials.
