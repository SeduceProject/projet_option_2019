from flask import Flask, request, render_template
from jinja2 import Template
import subprocess
import json
from pysnmp.hlapi import *


app = Flask(__name__)


# FRONT

@app.route("/")
def home():
    return render_template("home.html.jinja2", page="home")

@app.route("/editor")
def editor():
    with open("./arduino/examples/demo.ino") as f:
        code = f.read()
    return render_template("editor.html.jinja2", code=code, page="editor")


@app.route("/temperature")
def temperature():
    sensors = ["Sensor"+str(i) for i in range(1,17)]
    ips = get_ips("ips.json")
    cards = [card for card in ips]
    return render_template("temperature.html.jinja2", sensors=sensors, cards=cards, page="temperature")


# API

@app.route("/upload", methods=["POST"])
def upload():
    response = {}
    tm = Template(request.get_json()["template"])
    ips = get_ips('./ips.json')
    for card_id in ips:
        code = tm.render(card_id=card_id)
        with open("./arduino/src/main.ino", "w") as f:
            f.write(code)
        args = ('pio run --target upload --upload-port '+ips[card_id]).split(" ")
        update = subprocess.Popen(args, cwd="./arduino")
        update.wait()
        response[card_id] = 1 - update.returncode 
    return json.dumps(response)

@app.route("/get_temperature", methods=["POST"])
def get_temperature():
    data = request.get_json()
    card, sensor = data["card"], data["sensor"].strip("Sensor")
    ips = get_ips('./ips.json')
    ip = ips[card]
    oid = '.1.3.6.1.4.1.5.'+sensor
    temp = round(float(snmpget(ip, oid)),1)
    #if(type(temp)!=float):
    #    temp = -127
    print(temp)
    return json.dumps({"temp": temp})

# Other Functions

def get_ips(filename):
    with open(filename) as json_file:
        ips = json.load(json_file)
    return ips

def snmpget(ip, oid):
    '''snmpget -v 1 -c public ip oid'''
    g = getCmd(SnmpEngine(),
               CommunityData('public', mpModel=0),
               UdpTransportTarget((ip, 161)),
               ContextData(),
               ObjectType(ObjectIdentity(oid)))
    errorIndication, errorStatus, errorIndex, varBinds = next(g)
    if errorIndication:
        print(errorIndication)
    else:
        if errorStatus:
            print('%s at %s' % (
                errorStatus.prettyPrint(),
                errorIndex and varBinds[int(errorIndex)-1] or '?'))
        else:
            for name, val in varBinds:
                return val/100  # int->float


if __name__ == "__main__":
    app.run(debug=True)