# Schema of Database

## Information collected

Measure

| Name | Description | Example |
|---|---|---|
| timestamp TZ | | |
| temperature | | |
| GPS localization | | |
| Turbidity | | |

Set of measures

- Title
- Type of set : TIMEFRAME, EMERGENCY, SESSION, CALIBRATION, SINGLE
- Options dependending the type
- Array of Measures

Device

- Identification
  - BuildID
  - BluesID
- Location
- Fleet
- Status : TEST, OPERATIONAL, CALIBRATION, RETIRED

Fleet (set of Devices)

- Owner
- start operations

Owners

- ID
- Name
- Address
- Status
