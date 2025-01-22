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

