import json
import os
import pathlib
from datetime import datetime


def expired_file(path, ttl_hours=3):
    now = datetime.now()
    file_created_ts = pathlib.Path(path).stat().st_mtime
    created_time = datetime.fromtimestamp(file_created_ts)
    age = now - created_time
    age_in_hours = age.seconds / 60 / 60
    if age_in_hours > ttl_hours:
        return True
    else:
        return False


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
