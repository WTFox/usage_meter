from datetime import datetime

import requests

from .utils import write_json_file


def write_usage_json_to_file(username, password, path):
    usage_json = fetch_usage_json(username, password)
    write_json_file(path, usage_json)


def format_usage_message(usage_json):
    return """
     --   Data Usage for the Month   --
    Date:       {today} ({period_percentage}%)
    Days Left:  {days_remaining}
    Data Left:  {data_remaining} GB
    Data Used:  {data_used} GB / {data_limit} GB ({data_used_percentage}%)
    Period:     {start_date} - {end_date}
    """.format(**usage_json)


def fetch_usage_json(username, password):
    login_post_url = 'https://idm.east.cox.net/idm/coxnetlogin'
    usage_endpoint = 'https://www.cox.com/internet/ajaxDataUsageJSON.ajax'
    with requests.Session() as sesh:
        # Authenticate with Cox
        sesh.post(login_post_url, data={
            'username': username,
            'password': password,
            'targetFN': 'COX.net',
            'emaildomain': '@cox.net',
            'onsuccess': 'https://www.cox.com/resaccount/home.cox',
            'onfailure': 'http://www.cox.com/resaccount/sign-in.cox'
        })

        # We have to GET this URL to get some extra cookies
        sesh.get('https://www.cox.com/internet/mydatausage.cox')

        # Now we can get the JSON
        response = sesh.get(usage_endpoint, data={
            'usagePeriodType': 'monthly',
            '_': str(datetime.utcnow().timetz()).replace(':', '').replace('.', '')
        })
        response.raise_for_status()
        return _transform_usage_json(response.json())


def _transform_usage_json(usage_json):
    modem_details = usage_json.get('modemDetails', [])[0]

    # Calculate usage data
    data_used = int(modem_details['dataUsed']['monthly'][-1]['data'])
    data_limit = int(modem_details['dataPlan'].split('&')[0])
    data_remaining = data_limit - data_used

    # Calculate monthly data
    today = datetime.now()
    start_date_str, end_date_str = modem_details['dataUsed']['monthly'][-1]['month'].split('-')
    start_date = datetime.strptime(start_date_str, "%m/%d/%y")
    end_date = datetime.strptime(end_date_str, "%m/%d/%y")
    days_in_period = (end_date - start_date).days
    days_since_start_date = (today - start_date).days

    return {
        'today': today.strftime('%m/%d/%y'),
        'start_date': start_date.strftime('%m/%d/%y'),
        'end_date': end_date.strftime('%m/%d/%y'),
        'days_remaining': (end_date - today).days,
        'period_percentage': round((days_since_start_date / float(days_in_period)) * 100),
        'data_remaining': data_remaining,
        'data_limit': data_limit,
        'data_used': data_used,
        'data_used_percentage': int(modem_details['dataUsed']['actualPercentage'])
    }


if __name__ == '__main__':
    pass
