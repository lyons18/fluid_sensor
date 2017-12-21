# fluid_sensor
Code for Arduino Mini Pro with digital accelerometer to create a water leaks sensor.

Message type sended between arduino and mobile devices:
- arduino -> mobile - sending status code in case of exceeding maximal leak  level
- mobile  -> arduino:

Message:
- "type":"what resource to change":value

List and model of messaages:
- put:600:value - string, string, int
