# sound 
class DecibelLevel:
    faint = 40
    soft = 50
    normal = 60
    loud = 70


class LuxLevel:
    very_dark = 20
    sleep = 100
    normal = 2000


# distance to determine if a place has been occupied:
OCCUPIED_LENGTH = 2000


def append_analyse(avg_result, analyse_data):
    avg_result['status'] = {}
    for device_id in analyse_data:
        avg_result['status'][device_id] = {}
        avg_result['status'][device_id]['Noise'] = ""

        # Noise detection
        if analyse_data[device_id]['decibel'] < DecibelLevel.faint:
            avg_result['status'][device_id]['Noise'] = "Very Quite"
        elif analyse_data[device_id]['decibel'] < DecibelLevel.soft:
            avg_result['status'][device_id]['Noise'] = "Soft"
        elif analyse_data[device_id]['decibel'] < DecibelLevel.normal:
            avg_result['status'][device_id]['Noise'] = "Normal"
        else:
            avg_result['status'][device_id]['Noise'] = "IMPOSSIBLE TO SLEEP!!"

        # In use detection
        if analyse_data[device_id]['distance'] < OCCUPIED_LENGTH:
            avg_result['status'][device_id]['In Use'] = "Yes"
        else:
            avg_result['status'][device_id]['In Use'] = "No"

        # Brightness detection
        if analyse_data[device_id]['lux'] < LuxLevel.very_dark:
            avg_result['status'][device_id]['Brightness'] = "Ideal for sleeping"
        elif analyse_data[device_id]['lux'] < LuxLevel.sleep:
            avg_result['status'][device_id]['Brightness'] = "Ideal for sleeping & study"
        elif analyse_data[device_id]['lux'] < LuxLevel.normal:
            avg_result['status'][device_id]['Brightness'] = "Ideal for study"
        else:
            avg_result['status'][device_id]['Brightness'] = "Too bright"

    return avg_result