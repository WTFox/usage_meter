import json
import os


def read_json_file(path):
    json_data = None
    if os.path.isfile(path):
        with open(path, 'r') as f:
            json_data = json.loads(f.read())
    return json_data


def write_json_file(path, json_data):
    with open(path, 'w') as f:
        f.write(json.dumps(json_data))
    return json_data
