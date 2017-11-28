# Garage Door

This project uses and Wemos D1 (Arduino) and HC-SR04 Ultrasonic Detector to detect if the garage door is open or closed. The Arduino device senses if anything is below the detector. If the detector measuresures anything more than 25 inches, the detector assumes that the garage door is closed. If the detectors measure anything 25 inches or less, then the detector assumes that the garage door is open. The detector then tranmits the data to the SmartThings hub. 

The SmartThings device handler displays the garage door status.
