# Serial Communication Variables Reference

This document lists all serial communication variable names used in the FICES automation system, organized by station.

---

## Control Room Station

**Station Code:** `CR`  
**File:** `stations/control_room.py`

| Variable Name         | Type    | Description               |
| --------------------- | ------- | ------------------------- |
| `CR_BUFFER_VALVE`     | Boolean | Buffer tank valve control |
| `CR_GREENHOUSE_VALVE` | Boolean | Greenhouse valve control  |
| `CR_MAIN_PUMP`        | Boolean | Main water pump control   |

---

## Buffer Mixer Station

**Station Code:** `BM`  
**File:** `stations/buffer_mixer.py`

| Variable Name  | Type    | Description              |
| -------------- | ------- | ------------------------ |
| `BM_VALVE_A`   | Boolean | Solution A valve control |
| `BM_VALVE_B`   | Boolean | Solution B valve control |
| `BM_VALVE_C`   | Boolean | Solution C valve control |
| `BM_SOL_PUMP`  | Boolean | Solution pump control    |
| `BM_MIX_PUMP`  | Boolean | Mixer pump control       |
| `BM_MIX_VALVE` | Boolean | Mixer valve control      |

---

## Mixer Module Station

**Station Code:** `MM`  
**File:** `stations/mixer_module.py`

| Variable Name | Type    | Description                 |
| ------------- | ------- | --------------------------- |
| `MM_SOL_PUMP` | Boolean | Solution pump control       |
| `MM_T1_IN`    | Boolean | Tank 1 input valve control  |
| `MM_T1_OUT`   | Boolean | Tank 1 output valve control |
| `MM_T2_IN`    | Boolean | Tank 2 input valve control  |
| `MM_T2_OUT`   | Boolean | Tank 2 output valve control |
| `MM_T3_IN`    | Boolean | Tank 3 input valve control  |
| `MM_T3_OUT`   | Boolean | Tank 3 output valve control |

---

## Substation 1

**Station Code:** `SUB1`  
**File:** `stations/substation_1.py`

| Variable Name | Type    | Description                |
| ------------- | ------- | -------------------------- |
| `SUB1_VALVE`  | Boolean | Substation 1 valve control |

---

## Substation 2

**Station Code:** `SUB2`  
**File:** `stations/substation_2.py`

| Variable Name | Type    | Description                |
| ------------- | ------- | -------------------------- |
| `SUB2_VALVE`  | Boolean | Substation 2 valve control |

---

## Summary

**Total Variables:** 18

| Station      | Variable Count | Station Code |
| ------------ | -------------- | ------------ |
| Control Room | 3              | CR           |
| Buffer Mixer | 6              | BM           |
| Mixer Module | 7              | MM           |
| Substation 1 | 1              | SUB1         |
| Substation 2 | 1              | SUB2         |

---

## Serial Command Format

All commands are sent via serial in the format:

```
<VARIABLE_NAME>:<VALUE>
```

**Examples:**

- `CR_MAIN_PUMP:1` - Turn on main pump
- `CR_MAIN_PUMP:0` - Turn off main pump
- `MM_T1_IN:1` - Open Tank 1 input valve
- `BM_VALVE_A:0` - Close Buffer Mixer Valve A

---

## Event Feedback Format

Arduino sends event feedback with `EVT_` prefix:

```
EVT_<VARIABLE_NAME>:<VALUE>
```

**Examples:**

- `EVT_CR_MAIN_PUMP:1` - Main pump turned on (confirmed)
- `EVT_MM_T1_IN:0` - Tank 1 input valve closed (confirmed)

---

## Mixer Recipe Data Structure

**Endpoint:** `/mixer_module/recipe`  
**File:** `stations/mixer_module.py` → `process_mixer_recipe()`

The mixer recipe uses a **complex data structure** instead of simple boolean commands. This allows for precise control of nutrient mixing ratios.

### Recipe Data Format

| Field         | Type   | Description                           |
| ------------- | ------ | ------------------------------------- |
| `recipe`      | Object | Solution ratios (A, B, C)             |
| `recipe.A`    | Number | Solution A ratio (0-100)              |
| `recipe.B`    | Number | Solution B ratio (0-100)              |
| `recipe.C`    | Number | Solution C ratio (0-100)              |
| `target_tank` | String | Target tank number ("1", "2", or "3") |
| `total_ec`    | Number | Total EC target value                 |
| `water_level` | Number | Water level percentage (1-100)        |

### Example Recipe JSON

```json
{
  "recipe": {
    "A": 40,
    "B": 35,
    "C": 25
  },
  "target_tank": "1",
  "total_ec": 1.5,
  "water_level": 75
}
```

### Serial Command Format for Recipe

**Option 1: Single Compound Command**

```
RECIPE:<target_tank>,<ratio_A>,<ratio_B>,<ratio_C>,<total_ec>,<water_level>
```

Example:

```
RECIPE:1,40,35,25,1.5,75
```

**Option 2: Multiple Commands**

```
RECIPE_TANK:<tank_number>
RECIPE_A:<ratio>
RECIPE_B:<ratio>
RECIPE_C:<ratio>
RECIPE_EC:<ec_value>
RECIPE_WATER:<water_level_percentage>
RECIPE_START:1
```

Example:

```
RECIPE_TANK:1
RECIPE_A:40
RECIPE_B:35
RECIPE_C:25
RECIPE_EC:1.5
RECIPE_WATER:75
RECIPE_START:1
```

### Cancel All Recipes

**Endpoint:** `/mixer_module/cancel_all`

Cancels all active mixing recipes. This sends a stop command to Arduino.

**Serial Command:**

```
RECIPE_CANCEL:1
```

---

## Notes

- All control variables are **Boolean** type (0 = OFF, 1 = ON)
- Variable names use **station prefix** for easy identification
- Commands are sent from automation system to Arduino
- Events are sent from Arduino back to automation system for state confirmation
