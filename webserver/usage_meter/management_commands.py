from . import app

from .usage import fetch_usage_json, write_usage_json_to_file, format_usage_message


@app.cli.command()
def check_usage():
    usage_json = fetch_usage_json(
        username=app.config['COX_USERNAME'],
        password=app.config['COX_PASSWORD']
    )
    print(format_usage_message(usage_json))


@app.cli.command()
def write_usage():
    write_usage_json_to_file(
        username=app.config['COX_USERNAME'],
        password=app.config['COX_PASSWORD'],
        path=app.config['USAGE_JSON_FILE_PATH']
    )
