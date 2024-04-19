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

    # Format current_time as desired
    current_time_str = current_time.strftime('%I:%M%p')

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

if __name__ == '__main__':
    app.run(debug=True)
