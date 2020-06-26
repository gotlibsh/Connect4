import requests
import json


url = 'http://127.0.0.1:5000/evaluate'
req_content = \
        {'board': [ ['-','-','-','-','-','-','-'],
                    ['-','-','-','-','-','-','-'],
                    ['-','-','-','-','-','-','-'],
                    ['-','-','-','O','O','-','-'],
                    ['-','X','-','X','X','-','-'],
                    ['-','O','-','X','O','-','-']],
        'depth': 5,
        'turn': 'red'}

res = requests.post(url, json=req_content)
print(json.dumps(json.loads(res.text), indent=4, sort_keys=True))
