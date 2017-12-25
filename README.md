# fluid_sensor
Code for Arduino Mini Pro with digital accelerometer to create a water leaks sensor.

Message type sended between arduino and mobile devices:
1. arduino -> mobile: 
 - sending only the status code in case of exceeding maximal leak  level - 200 (small leak), 300 (large leak), 400 (big leak)
 
2. mobile -> arduino:
- message pattern: x:y:(z):

- x - type_of_message (PUT/GET) - passed as String
- y - resource_to_change (LED/Button status) - passed as String but as a device code (for example 600)
- z - new value - passed as Integer, but appears only if this is PUT message

For example:
put:600:120 
get:400
