from flask import Flask, request, render_template
from jinja2 import Template
import subprocess
import json


app = Flask(__name__)


# FRONT

@app.route("/")
def home():
    return render_template("home.html.jinja2")

@app.route("/editor")
def editor():
    with open("./arduino/examples/test_template.ino") as f:
        code = f.read()
    return render_template("editor.html.jinja2", code=code)


@app.route("/temperature")
def temperature():
    return render_template("temperature.html.jinja2")


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


def get_ips(filename):
    with open(filename) as json_file:
        ips = json.load(json_file)
    return ips

if __name__ == "__main__":
    app.run()