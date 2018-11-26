import time
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import requests
import re

topics = { 'move':'garage/move', 'temperatureHome':'home/temperature', 'temperatureGarage': 'garage/temperature', 'doorHome':'home/door', 'doorGarage':'garage/door'}

if __name__ == "__main__":
	
    mqtt_ip = "localhost"
    garage_ip = "192.168.1.25"
    home_ip = "192.168.1.24"
    
    while True:
        try:
            #Move
            move = requests.get("http://"+garage_ip+":8080/move").text
            p = re.compile('.*', re.IGNORECASE)
            move = p.search(move).group() 
            print(move)
            if move == "1":
                publish.single(topics['move'], int(move), hostname=mqqt_ip)
            else:
                publish.single(topics['move'], int(move), hostname=mqtt_ip)
        except:
            print("Move not found")
        try:
            #Temperature in Garage
            tempGarage = requests.get("http://"+garage_ip+":8080/temperature").text
            p = re.compile('.*', re.IGNORECASE)
            tempGarage = p.search(tempGarage).group() 
            print(tempGarage)
            publish.single(topics['temperatureGarage'], tempGarage, hostname=mqtt_ip)
        except:
            print("Temperature in garage not found")
        
        try:
            #Door in Garage
            door = requests.get("http://"+garage_ip+":8080/door").text
            p = re.compile('.*', re.IGNORECASE)
            door = p.search(door).group() 
            print("Garage: "+door)
            publish.single(topics['doorGarage'], int(door), hostname=mqtt_ip)
        except:
            print("Door in garage not found")

        try:
            #Temperature in Home
            tempHome = requests.get("http://"+home_ip+":8080/temperature").text
            p = re.compile('.*', re.IGNORECASE)
            tempHome = p.search(tempHome).group() 
            print("Home: "+tempHome)
            publish.single(topics['temperatureHome'], tempHome, hostname=mqtt_ip)
        except:
            print("Temperature in home not found")
        
        try:
            #Door in Home
            door = requests.get("http://"+home_ip+":8080/door").text
            p = re.compile('.*', re.IGNORECASE)
            door = p.search(door).group() 
            print("Home: "+door)
            # print(type(door))
            publish.single(topics['doorHome'], int(door), hostname=mqtt_ip) 
        except:
            print("Door in home not found")

        time.sleep(2)

        



    client = mqtt.Client()
    
    client.connect(mqtt_ip, 1883, 60)
    
client.loop_forever()
