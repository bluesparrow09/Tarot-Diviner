# Interactive Tarot Diviner

A fun and mystical device for tarot card reading! Users can touch the sensor to “draw” a random tarot card, which is revealed on an OLED screen. Dynamic effects like pointer rotation and RGB lighting enhance the experience.

---

## Sensor Device

- **Component:** Touch Sensor (TTP223)
- **Function:** Detects user touch and sends the touch duration to the ESP32.
- **How it Works:**
  1. Records touch duration as input.
  2. Sends data to ESP32 for stepper motor control.
  3. Determines the selected card when the touch ends.

---

## Display Device

- **Components:**
  - OLED Display (SSD1306): Displays card name, upright/reversed state, and meaning.
  - Stepper Motor (28BYJ-48): Rotates a pointer dynamically based on touch duration.
  - RGB Lights (WS2812): Provides visual feedback based on card state (e.g., green for upright, red for reversed).
- **How it Works:**
  1. Receives data from ESP32.
  2. Updates OLED with card details.
  3. Rotates pointer to the selected card.
  4. Activates RGB lights for dynamic feedback.

---

## System Architecture and Workflow

### System Architecture:
![image](https://github.com/user-attachments/assets/8ea1e4e7-5279-41e3-8396-5b19fa50aefb)

[Touch Sensor (TTP223)] --> [ESP32 BLE] --> [Stepper Motor (Pointer Rotation)] --> [OLED Display (Card Info)] --> [RGB Lights (Visual Feedback)]
### Function Workflow:
1. User touches the sensor.
2. ESP32 records touch duration.
3. Stepper motor rotates pointer dynamically.
4. User releases touch; ESP32 determines the card.
5. OLED displays card info and RGB lights change color.

---

## Datasheets

Datasheets for all components are located in the `datasheets/` folder of this repository:
- `TTP223_datasheet.pdf`
- `28BYJ48_datasheet.pdf`
- `ESP32_datasheet.pdf`
- `SSD1306_datasheet.pdf`
- `WS2812_datasheet.pdf`

