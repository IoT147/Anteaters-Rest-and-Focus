import boto3
from boto3.dynamodb.conditions import Key


# for demo, set average period to 10 seconds
AVERAGE_ITEMS = 10
table = boto3.resource('dynamodb').Table('iotfinal')


def get_items():
    """
    Retrieve sensor data from AWS DynamoDB
    This is only a temp solution
    It has N-query, could cause performance issue if more devices are added
    Add GSI would fix the issue
    """
    response = {}
    response['device_one'] = table.query(
        KeyConditionExpression=Key('device_id').eq('1'),
        Limit=AVERAGE_ITEMS,
        ScanIndexForward=False
    )
    response['device_two'] = table.query(
        KeyConditionExpression=Key('device_id').eq('2'),
        Limit=AVERAGE_ITEMS,
        ScanIndexForward=False
    )

    return response