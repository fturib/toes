## format of the data send from NOTE to Cloud backend

Webhook information: `https://webhook.site/#!/view/46794803-bf61-48f0-b035-fb8d0f0c9f3c/247331b8-d03f-411e-9359-38d9565072b9/1`

```json
{
  "event": "a75d45f5-e62b-4a35-8401-bd4d7197b939",
  "file": "data.qo",
  "body": {
    "turbidity": {
      "adc_raw": 1300,
      "reading": 167,
      "value": 2.345
    }
  },
  "session": "891e6ef2-a367-4d12-b912-d59424ce8696",
  "transport": "wifi",
  "best_id": "dev:4827e2cc7260",
  "device": "dev:4827e2cc7260",
  "product": "product:com.gmail.fturib:toes",
  "app": "app:56cb6a9e-f02c-4e2a-88a6-85c4aa3efca2",
  "received": 1776718322.259774,
  "req": "note.add",
  "best_location_type": "triangulated",
  "best_location_when": 1776718317,
  "best_lat": 38.934672,
  "best_lon": -76.547087,
  "best_location": "Edgewater Maryland",
  "best_country": "US",
  "best_timezone": "America/New_York",
  "tri_when": 1776718317,
  "tri_lat": 38.934672,
  "tri_lon": -76.547087,
  "tri_location": "Edgewater Maryland",
  "tri_country": "US",
  "tri_timezone": "America/New_York",
  "fleets": [
    "fleet:7835b534-ca9c-43af-80c8-c5e345a96b29"
  ]
}
```

Tools to test JSONata transformation : [JSONata Exerciser](https://try.jsonata.org/)
JSON Transformation:

```json
{
    "turbidity": body.turbidity,
    "station": "from-notecard"
}
```

Raw content sent by Note to the sample-webhook route

```json
{
  "station": "from-notecard",
  "turbidity": {
    "adc_raw": 1300,
    "reading": 167,
    "value": 3.245
  }
}
```

## conversation with NoteCard

We need to send:
1- a data to add - req = note.add
2- ask for sync with NoteHub - (it not done, I believe it will happen after some time)

**Send the data**:

```json
{"req":"note.add","body":{"turbidity": { "value": 3.245, "adc_raw": 1300, "reading": 167 } } }
```

**Ask for sync with Cloud**:

```json
{"req":"hub.sync"}
```

**Configuration**:
1- send the access to Wifi (can be done manually)
2- send the ProductUUID

```json
{ "req": "hub.set", "product": "com.gmail.fturib:toes"}
```

**verifying the card version**:

```json
{"req":"card.version"}
```

```json
{
 "version": "notecard-11.2.1.17592",
 "device": "dev:4827e2cc7260",
 "name": "Blues Wireless Notecard",
 "sku": "NOTE-ESP",
 "ordering_code": "WZ0XV1N0ZZAV",
 "board": "4.0",
 "wifi": true,
 "body": {
  "org": "Blues Wireless",
  "product": "Notecard",
  "target": "s3",
  "version": "notecard-s3-11.2.1",
  "ver_major": 11,
  "ver_minor": 2,
  "ver_patch": 1,
  "ver_build": 17592,
  "built": "Mar 8 2026 18:54:58"
 }
}
```

## wiring between Cygnet and Notecard, through NoteCarrier-F

[Notecarrier F v1.3 Datasheet](https://dev.blues.io/datasheets/notecarrier-datasheet/notecarrier-f-v1-3/)

[Cygnet Datasheet](https://dev.blues.io/datasheets/cygnet-datasheet/cygnet-v1-2/?_gl=1*1up5uu0*_gcl_au*OTkyMzA1MjYzLjE3NzYzMzM1MjU.*_ga*MTQzMjY5MjAwOC4xNzc2MzMzNTI1*_ga_PJ7RGMWWBX*czE3NzY3ODU5NDIkbzQkZzEkdDE3NzY3ODU5ODEkajIxJGwwJGgxOTEzNDgyNTg.&_ga=2.122753346.877290756.1776709501-1432692008.1776333525)
Guide and tutorial from **Blues** : [Host Wiring Guide](https://dev.blues.io/guides-and-tutorials/host-wiring-guide/)

SCL - PB6 - SDA1 - TX1 - connected to Feather SCL
SDA - PB7 - SCL1 - RX1 - connected to Feather SDA

Feather SDA/SCL are connected to the NoteCard connector SDA / SCL

USER_BTN - PC13
LED_BUILTIN - PA8

[NoteWIFI Datasheet](https://dev.blues.io/datasheets/notecard-datasheet/note-wifi/)

## using libraries to communicte with NoteWIFI

[Global link to Tools & SDK](https://dev.blues.io/tools-and-sdks/)

[Notecard API -0 c library implementation](https://blues.github.io/note-c/library_initialization.html)

[Notecard API documentation](https://dev.blues.io/api-reference/notecard-api/introduction/)

NOTE: there is a sample application for communication eetwen an STM32L$ and Notecard : [note-stm32l4](https://github.com/blues/note-stm32l4)

[Here is the Github repo for the note-c API implementation for c](https://github.com/blues/note-c)
