import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time

# GPIO Setup
LED = 18

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED, GPIO.OUT)

# MQTT Broker Details
BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "home/light"

# Callback when connected
def on_connect(client, userdata, flags, rc):
    print("Connected to Broker")
    client.subscribe(TOPIC)

# Callback when message received
def on_message(client, userdata, msg):
    message = msg.payload.decode()
    print("Message Received:", message)

    if message == "ON":
        GPIO.output(LED, GPIO.HIGH)
        print("Light ON")

    elif message == "OFF":
        GPIO.output(LED, GPIO.LOW)
        print("Light OFF")

# Create MQTT Client
client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message

# Connect to Broker
client.connect(BROKER, PORT, 60)

# Start Listening
client.loop_forever()
