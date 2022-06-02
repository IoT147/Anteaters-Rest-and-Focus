from flask import Flask, jsonify
import aws_controller
import processing


app = Flask(__name__)


@app.route('/')
def index():
    return "This is the main page."
    

@app.route('/arf')
def arf():
    response = aws_controller.get_items()
    avg_result = processing.get_average(response)
    return avg_result


if __name__ == '__main__':
    app.run()