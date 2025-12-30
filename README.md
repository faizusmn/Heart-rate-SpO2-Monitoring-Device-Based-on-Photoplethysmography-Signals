# HR & SpO₂ Monitoring System Using MAX30102

---

## Project Summary

This project implements a **non-invasive biomedical monitoring system** to measure heart rate and blood oxygen saturation using optical PPG signals. The system focuses not only on data acquisition but also on **signal conditioning and filtering** to improve measurement stability and accuracy.

The project emphasizes practical application of **biomedical sensors**, **instrumentation principles**, and **embedded signal processing**.

---

## System Components

- **MAX30102 Pulse Oximeter Sensor**  
  Generates red and infrared PPG signals from blood flow.

- **Microcontroller (Arduino Uno)**  
  Performs sensor interfacing, signal processing, and physiological parameter calculation.

- **OLED / LCD Display**  
  Displays heart rate (BPM) and SpO₂ values in real time.

---

## Signal Processing Workflow

1. Acquire raw red and IR PPG signals via I²C  
2. Perform hardware calibration to prevent optical saturation  
3. Reduce noise through sample rate adjustment  
4. Apply moving average filter to stabilize BPM readings  
5. Calculate heart rate (BPM) and SpO₂ values  
6. Display processed results continuously  

---

## Technologies Used

- **Sensor:** MAX30102  
- **Microcontroller:** Arduino Uno
- **Display:** OLED SSD1306  
- **Communication:** I²C  
- **Programming Language:** C/C++ (Arduino)  
- **Concepts:** PPG, Biomedical Instrumentation, Signal Filtering, Embedded Systems  

---

## Notes and Limitations

- Measurement accuracy depends on stable finger placement
- Motion artifacts significantly affect PPG signal quality
- Intended for **educational and experimental use only**
- Not designed for medical diagnosis or clinical application

---

## License

This project was developed for academic purposes as part of the  
**Biomedical Sensor and Instrumentation** course.
