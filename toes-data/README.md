# TOES - database

## High level

It is a simple [google sheet](https://docs.google.com/spreadsheets/d/1ZhK_DA8yjRrNYgK_bYiFMyRqSPJ9FJxpg6aOpctGqWA/edit?gid=0#gid=0) hosted in Google Drive of fturib

- Access through a GCP ServiceAccount (need to get credential.json by asking <fturib@gmail.com>) that have an access Editor to the spreadsheet.

To be transformed into **Firestone** noSQL DB under GCP

## Send random data to fill-up the database (spreadsheet)

crontab -e
crontab -l

```:crontab
0,30 * * * * cd /Users/francois/toes/toes-data/provider && ./upload.sh "South River" 1 >> cron_toes.log 2>&1
10,40 * * * * cd /Users/francois/toes/toes-data/provider && ./upload.sh "West River" 1 >> cron_toes.log 2>&1
0,15,30,45 * * * * cd /Users/francois/toes/toes-data/provider && ./upload.sh "Rhode River" 1 >> cron_toes.log 2>&1
```
