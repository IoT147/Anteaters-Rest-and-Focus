from flask import Flask, jsonify
import aws_controller
import processing
import analyse


app = Flask(__name__)



@app.route('/')
def index():
    return "This is the main page."
    

@app.route('/arf')
def arf():
    response = aws_controller.get_items()
    avg_result, analyse_data = processing.get_average(response)
    analysed_result = analyse.append_analyse(avg_result, analyse_data)
    return avg_result


if __name__ == '__main__':
    app.run()