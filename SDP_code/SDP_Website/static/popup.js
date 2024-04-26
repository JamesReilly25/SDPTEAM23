// Load JSON data from files
const loadData = async () => {
  const promises = [];
  for (let i = 0; i <= 3; i++) { // Load data0.json to data3.json
    promises.push(fetch(`static/data/data${i}.json`)
      .then(response => response.json())
      .catch(error => {
        console.error(`Error loading data${i}.json:`, error);
        return null; // Return null for failed fetch
      })
    );
  }
  return Promise.all(promises);
};

// Populate dropdown menu with titles and choose JSON file to edit
const populateDropdown = async () => {
  const data = await loadData();
  const dropdown = document.getElementById('jsonDropdown');
  
  // Clear previous options
  dropdown.innerHTML = '';
  
  data.forEach((item, index) => {
    if (item !== null && item.title) { // Check if data is not null and has a title
      const option = document.createElement('option');
      option.value = index;
      option.textContent = item.title;
      dropdown.appendChild(option);
    }
  });
};


// Update title in the selected JSON file
const updateTitle = async () => {
  const selectedIndex = document.getElementById('jsonDropdown').value;
  const newTitle = document.getElementById('newTitle').value;
  
  if (!newTitle) {
    alert('Please enter a new title.');
    return;
  }
  
  const data = await loadData();
  const selectedData = data[selectedIndex];
  if (selectedData !== null) {
    selectedData.title = newTitle;
  
    // Send updated data to the server
    const response = await fetch('/title-update', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ index: selectedIndex, newTitle: newTitle }),
    });

    if (response.ok) {
      console.log('Title updated successfully!');
    } else {
      console.error('Failed to update title:', response.status);
    }

    // Close the popup after updating the title
    closePopup('popup');
  } else {
    console.error(`No data available for data${selectedIndex}.json`);
  }
};


// Function to open the popup
function openPopup(popupId) {
  var popup = document.getElementById(popupId);
  if (popup) {
    popup.style.display = "block";
    populateDropdown(); // Populate dropdown when the popup is opened
  } else {
    console.error("Popup element with ID " + popupId + " not found.");
  }
}

// Function to close the popup
function closePopup(popupId) {
  var popup = document.getElementById(popupId);
  if (popup) {
    popup.style.display = "none";
  } else {
    console.error("Popup element with ID " + popupId + " not found.");
  }
}





const updateMaxDataPoints = async () => {
  // Get the new value from the input field
  var newMaxDataPoints = document.getElementById("maxDataPointsInput").value;

  // Load data
  const data = await loadData();

  // Loop over each index 0-4
  for (let i = 0; i < 5; i++) {
    const selectedData = data[i];

    // Check if data exists for this index
    if (selectedData !== null) {
      // Update the graph_period
      selectedData.graph_period = newMaxDataPoints;

      // Send updated data to the server
      const response = await fetch('/graph-period-update', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ index: i, newGraphPeriod: newMaxDataPoints }),
      });

      if (response.ok) {
        console.log(`Graph period updated successfully for index ${i}!`);
      } else {
        console.error(`Failed to update graph period for index ${i}:`, response.status);
      }
    } else {
      console.error(`No data available for index ${i}`);
    }
  }
};


const updateSchedule = async () => {
  // Get the new value from the input field
  var newSchedule = document.getElementById("scheduleInput").value;

  // Load data
  const data = await loadData();

  // Loop over each index 0-4
  for (let i = 0; i < 5; i++) {
    const selectedData = data[i];

    // Check if data exists for this index
    if (selectedData !== null) {
      // Update the graph_period
      selectedData.current_watering_period = newSchedule;

      // Send updated data to the server
      const response = await fetch('/schedule-update', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ index: i, new_watering_period: newSchedule }),
      });

      if (response.ok) {
        console.log(`schedule updated successfully for index ${i}!`);
      } else {
        console.error(`Failed to update schedule for index ${i}:`, response.status);
      }
    } else {
      console.error(`No data available for index ${i}`);
    }
  }
};