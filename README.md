# Interactive Tarot Diviner

A fun and mystical device for tarot card reading! Users can touch the sensor to “draw” a random tarot card, which is revealed on an OLED screen. Dynamic effects like pointer rotation and RGB lighting enhance the experience.

---

## Sensor Device
<img width="1470" alt="image" src="https://github.com/user-attachments/assets/a603ad4c-f37b-4f77-8156-546604f982f0" />

---

## Display Device
<img width="1470" alt="image" src="https://github.com/user-attachments/assets/8e7f2d8f-3585-4daf-b496-b9395d606d1c" />

---

## System Architecture and Workflow

### System Architecture:
![image](https://github.com/user-attachments/assets/8ea1e4e7-5279-41e3-8396-5b19fa50aefb)

[Touch Sensor (TTP223)] --> [ESP32 BLE] --> [Stepper Motor (Pointer Rotation)] --> [OLED Display (Card Info)] --> [RGB Lights (Visual Feedback)]
### Function Workflow:
![output](https://github.com/user-attachments/assets/977ac4f8-1b2f-47e6-bfdc-6c3c95f58752)

1. User touches the sensor.
2. ESP32 records touch duration.
3. Stepper motor rotates pointer dynamically.
4. User releases touch; ESP32 determines the card.
5. OLED displays card info and RGB lights change color.

