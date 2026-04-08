# TOES - backend API

## High level

Run an Express HTTP server on nodejs.
Reply inqueries from anywhere internet (for now)
Usage:

- User Interfaces backend (webapp, phone app)
  - Data requests
  - Interactions with the device (in, out)
- Processing data events forwarded by Blues (send to DB and other processes)

### How to

#### Build

- **npm install** : Ensure that all depdnencies are downloaded
- **npm run build** : Build the full application

#### Test

- **npm run start** : run a local server (testing purpose)

#### Deploy to GCP

On the first install, ensure to have firebase tooling:

- **npm install -g firebase-tools**
- **firebase login**
- **firebase init hosting**

For the deployment itself:

- **gcloud run deploy toes-backend --source . --region us-central1 --allow-unauthenticated --min-instances 0 --max-instances 1 --memory 512Mi**

project:toes-492610
