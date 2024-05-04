// Function to fetch data from JSON files for multiple plants
function fetchPlantData() {
    const numberOfPlants = 5; // Assuming you have data0.json to data4.json

    // Array to store fetched data along with index
    const fetchedData = [];

    // Fetch data from each JSON file
    for (let i = 0; i < numberOfPlants; i++) {
        fetch(`/static/data/data${i}.json`) // Fetch the JSON file for each plant
            .then(response => response.json()) // Parse the JSON response
            .then(data => {
                fetchedData.push({ index: i, value: data.last_water_amount }); // Store index and last water amount value
                // Update numbers if all data is fetched
                if (fetchedData.length === numberOfPlants) {
                    updateNumbers(fetchedData);
                }
            })
            .catch(error => console.error(`Error fetching data for plant ${i}:`, error)); // Log any errors
    }
}

// Function to update the numbers in the dark gray box
function updateNumbers(fetchedData) {
    // Sort the fetched data based on index
    fetchedData.sort((a, b) => a.index - b.index);

    const numbers = document.querySelectorAll('.number'); // Select all elements with class 'number'
    fetchedData.forEach((item, index) => {
        if (index < numbers.length) {
            numbers[index].textContent = item.value; // Set the text content of each number element
        }
    });
}


  // Function to fetch last water time from data0.json
function fetchLastWaterTime() {
    fetch('/static/data/data0.json') // Fetch data0.json
      .then(response => response.json()) // Parse the JSON response
      .then(data => {
        const lastWaterTime = data.time_of_last_water || "Unknown"; // Get the time of last water or set as "Unknown" if not available
        document.getElementById('lastWaterTime').textContent = lastWaterTime; // Update the text content with last water time
      })
      .catch(error => console.error('Error fetching last water time:', error)); // Log any errors
}

  // Call the fetchLastWaterTime function when the page loads
window.addEventListener('load', fetchLastWaterTime);

// Call the fetchPlantData function when the page loads
window.addEventListener('load', fetchPlantData);
