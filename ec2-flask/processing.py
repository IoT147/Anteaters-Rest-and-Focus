from flask import Flask, jsonify
import aws_controller
from collections import defaultdict


def get_average(response):
    count = response["device_one"]["Count"]
    result_avg = {}
    analyse_data = {}
    
    for device in response:
        if device not in result_avg:
            result_avg[device] = {}

        sensor_sum = defaultdict(int)
        for item in range(count):
            payload = response[device]['Items'][item]['payload']
            for sensor_name, data in payload.items():
                sensor_sum[sensor_name] += data
        
        for sensor_name, data in sensor_sum.items():
            if sensor_name not in result_avg[device]:
                result_avg[device][sensor_name] = 0
            result_avg[device][sensor_name] = sensor_sum[sensor_name] / count

            if sensor_name == 'decibel':
                if device not in analyse_data:
                    analyse_data[str(device)] = {}
                analyse_data[str(device)]['decibel'] = int(result_avg[device][sensor_name])
            if sensor_name == 'lux':
                if device not in analyse_data:
                    analyse_data[str(device)] = {}
                analyse_data[str(device)]['lux'] = int(result_avg[device][sensor_name])
            if sensor_name == 'distance':
                if device not in analyse_data:
                    analyse_data[str(device)] = {}
                analyse_data[str(device)]['distance'] = int(result_avg[device][sensor_name])

    return result_avg, analyse_data

