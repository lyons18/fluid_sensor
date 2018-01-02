# fluid_sensor
Code for Arduino Mini Pro with digital accelerometer to create a water leaks sensor.

Message type sended between arduino and mobile devices:
1. arduino -> mobile: 
 - sending only the status code in case of exceeding maximal leak  level - 200 (small leak), 300 (large leak), 400 (big leak)
 - sending data about sensor after GET request (frequency, small leak range, large leak range)

Message:
 - LEAK:leak_code (for example: LEAK:200)
 - INFO:freq:small:large (for example INFO:200:13:14)

2. mobile -> arduino:
 - message pattern: x:y:(z1):(z2)

 - x - type_of_message (PUT/GET) 
 - y - resource_to_change - passed as a device code (for example 600)
 - z1 - new first value - appears only if this is PUT message and y = 600
 - z2 - new first value - appears only if this is PUT message and y = 700

Message:
 - GET:100 - get info about sensor
 - PUT:600:freq - set new frequency (for example PUT:600:200)
 - PUT:700:small:large - set new leak ranges (for example PUT:700:11:16)
 - PUT:900 - reset all data to default



