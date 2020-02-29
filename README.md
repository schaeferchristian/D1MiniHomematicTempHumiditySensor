# Measuring Temperature and Humidity the cheap way

Buying preassembled products is not always the most fun. That's why I built this humidity temperature device myself and let it post to Homematic.

To give you an idea about the total costs and which parts you need:

* The Wemos D1 Mini Arduino compatible board that brings WIFI out of the box    ~5€ e.g. at [BerryBase.de](https://www.berrybase.de/raspberry-pi-co/esp8266-esp32/d1-mini-esp8266-entwicklungsboard)
* DHT22 Digital Humidity and Temperature-Sensor                                 ~4€ e.g at [BerryBase.de](https://www.berrybase.de/raspberry-pi-co/raspberry-pi/bauelemente/dht22-digitaler-temperatur-und-luftfeuchtesensor)
* 10 kOhm resistor

You see in total it is less then 10€

# Layout Sketch

![Layout sketch](https://github.com/schaeferchristian/D1MiniHomematicTempHumiditySensor/raw/master/Sketch.png "Layout Sketch")

# Prepare Homematic CCU or RaspberryMatic to receive Temperature and Humidity Values

* Create a SystemVariable for Temperature in your ControlCenter under "Settings" > "System Variables" > "New"
 * Set a descriptive name, description, Variable Type "Number", Unit of Measurement "°C"
* Create another one for the Humidity (Unit of Measurement "%")
* You need to be able to write the Variables via HTTP-Calls, therefore install the "XML-API" Plugin from Jens Maus
 * under "Settings" > "Control Panel" > "Additional Software", get the plugin from https://github.com/jens-maus/XML-API
* Now you need to get find out the "ise_id" for both of your variables
 * Simply paste the following Link incl. your Homematic IP into your Browser-URL-Field: http://YOUR_HOMEMATIC_IP/addons/xmlapi/sysvarlist.cgi
 * It shows you a list of your custom variables and you should also find the ones you defined previously. Write down the "ise_id"s of your Temparature and Humidity Variables (you will need to add them to the script, as well as the ip of the Homematic CCU)
