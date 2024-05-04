from flask import Flask, request, jsonify, render_template
import json
from datetime import datetime, timedelta

app = Flask(__name__)

#-----------------------------------helper-----------------------------------------
# Function to read data from JSON file
def read_from_json(filepath):
    with open(filepath, 'r') as json_file:
        return json.load(json_file)

# Function to write data to JSON file
def write_to_json(data, filepath):
    with open(filepath, 'w') as json_file:
        json.dump(data, json_file, indent=4)


def average_rate_of_change(numbers):
    if len(numbers) < 2:
        return None  # Need at least two numbers to calculate rate of change
    
    differences = [numbers[i] - numbers[i-1] for i in range(1, len(numbers))]
    average_change = sum(differences) / len(differences)
    return average_change

def score_number(number, upper_limit, lower_limit):
    score = ((upper_limit - number) / (upper_limit - lower_limit)) * 100
    return score
#----------------------------------------------------------------------------


@app.route('/')
def index():
    return render_template('hometemp.html')



@app.route('/get_data')
def get_data():
    # Read data from data0.json
    with open('data0.json', 'r') as f:
        data = json.load(f)
    return jsonify(data)

#example endpoint
'''
@app.route('/bacon')
def bacon():
    return '511'
'''



@app.route('/Amount_to_water', methods=['GET'])
def Amount_to_water():
    water_amounts = [1, 2, 3, 4, 5]
    base_watering_amount = 100

    for json_index in range(5):

        json_file_path = f"static/data/data{json_index}.json"
        data = read_from_json(json_file_path)

        target = data['current_target'] # target ideal plant file to mimic

        json_file_path2 = f"static/data/Ideal{target}.json" 
        data2 = read_from_json(json_file_path2)

        current_time = datetime.utcnow() - timedelta(hours=4)  # UTC time minus 4 hours for EST
        # Format current_time as desired with date and time
        current_time_str = current_time.strftime('%I:%M%p %m/%d/%Y')


        if len(data['moisture_data']) > (data['readings_per_day'] * data['current_watering_period']):
            history_from_last_water=data['moisture_data'][-(data['readings_per_day'] * data['current_watering_period']):] #mv history from last watering
            mv_roc= average_rate_of_change(history_from_last_water) * data['readings_per_day'] # average rate of change of MV values
            target_mv = data2['lower_limit'] + mv_roc * data['current_watering_period'] # target MV to end at after watering

            score = score_number(history_from_last_water[-1],data2['lower_limit'],data2['upper_limit']) # how far off were we from where we want to be
            water_multiplier = data['water_multiplier']*score/100 # a learning multiplier
            water_amount = int(base_watering_amount * water_multiplier)

            if water_amount > 200:
                water_amount = 200

            # Update data json with new water_multiplier
            data['water_multiplier'] = water_multiplier
            data['last_water_amount'] = water_amount
            
            
            water_amounts[json_index] = water_amount

        else: #first watering case
            water_amounts[json_index] = base_watering_amount* data['current_watering_period']

        data['time_of_last_water'] = current_time_str
        write_to_json(data, json_file_path)
        

    return jsonify(water_amounts)

    
    




@app.route('/update_data', methods=['POST'])
def update_data():
    # Get data from request
    json_data = request.json
    number1 = json_data.get('number1')
    json_index = json_data.get('json_index')  # New parameter for specifying JSON file index

    if number1 is None or json_index is None:
        return jsonify({'error': 'Missing data in request'}), 400
    
    # Ensure json_index is an integer between 0 and 3
    if not isinstance(json_index, int) or json_index < 0 or json_index > 3:
        return jsonify({'error': 'Invalid JSON index'}), 400
    
    # Path to the JSON file based on json_index
    json_file_path = f"static/data/data{json_index}.json"
    
    # Get data from JSON file
    data = read_from_json(json_file_path)
    
    # Get current time in EST timezone
    current_time = datetime.utcnow() - timedelta(hours=4)  # UTC time minus 4 hours for EST

    # Format current_time as desired with date and time
    current_time_str = current_time.strftime('%Y-%m-%d %I:%M%p')

    # Append numbers to arrays
    data['moisture_data'].append(number1)
    data['time_labels'].append(current_time_str)

    # Write updated data to JSON file
    write_to_json(data, json_file_path)

    return jsonify({'success': True})


@app.route('/title-update', methods=['POST'])
def update_title():
    data = request.json
    index = data.get('index')
    new_title = data.get('newTitle')
    
    if index is None or new_title is None:
        return jsonify({'error': 'Invalid request. Both index and newTitle are required.'}), 400
    
    try:
        filename = f"static/data/data{index}.json"
        with open(filename, 'r') as file:
            json_data = json.load(file)
            json_data['title'] = new_title
        with open(filename, 'w') as file:
            json.dump(json_data, file, indent=4)
        return jsonify({'message': 'Title updated successfully!'}), 200
    except FileNotFoundError:
        return jsonify({'error': f'File data{index}.json not found.'}), 404
    except Exception as e:
        return jsonify({'error': f'Error updating title: {str(e)}'}), 500


@app.route('/graph-period-update', methods=['POST'])
def update_graph_period():
    data = request.json
    index = data.get('index')
    new_graph_period = data.get('newGraphPeriod')
    
    if index is None or new_graph_period is None:
        return jsonify({'error': 'Invalid request. Both index and newGraphPeriod are required.'}), 400
    
    try:
        filename = f"static/data/data{index}.json"
        with open(filename, 'r') as file:
            json_data = json.load(file)
            json_data['graph_period'] = new_graph_period
        with open(filename, 'w') as file:
            json.dump(json_data, file, indent=4)
        return jsonify({'message': 'Graph period updated successfully!'}), 200
    except FileNotFoundError:
        return jsonify({'error': f'File data{index}.json not found.'}), 404
    except Exception as e:
        return jsonify({'error': f'Error updating graph period: {str(e)}'}), 500

@app.route('/schedule-update', methods=['POST'])
def schedule_update():
    data = request.json
    index = data.get('index')
    new_watering_period = data.get('new_watering_period')
    
    if index is None or new_watering_period is None:
        return jsonify({'error': 'Invalid request. Both index and new_watering_period are required.'}), 400
    
    try:
        filename = f"static/data/data{index}.json"
        with open(filename, 'r') as file:
            json_data = json.load(file)
            json_data['current_watering_period'] = new_watering_period
        with open(filename, 'w') as file:
            json.dump(json_data, file, indent=4)
        return jsonify({'message': 'current_watering_period updated successfully!'}), 200
    except FileNotFoundError:
        return jsonify({'error': f'File data{index}.json not found.'}), 404
    except Exception as e:
        return jsonify({'error': f'Error updating current_watering_period period: {str(e)}'}), 500



if __name__ == '__main__':
    app.run(debug=True)
