from flask import request, jsonify

from . import app
from .usage import format_usage_message, write_usage_json_to_file
from .utils import read_json_file


@app.route('/', methods=['GET'])
def index():
    path = app.config.get('USAGE_JSON_FILE_PATH')
    usage_json = read_json_file(path)
    if not usage_json:
        write_usage_json_to_file(
            username=app.config.get('COX_USERNAME'),
            password=app.config.get('COX_PASSWORD'),
            path=path
        )
        usage_json = read_json_file(path)

    if request.args.get('as_string', False):
        return format_usage_message(usage_json)

    return jsonify(usage_json)


if __name__ == '__main__':
    app.run(debug=app.config.get('DEBUG'))
