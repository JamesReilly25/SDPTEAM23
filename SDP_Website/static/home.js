// main.js
fetch('static/data/data0.json')
  .then(response => response.json())
  .then(data => {
    // Check if the JSON data is not empty
    if (data && data.time_labels && data.moisture_data && data.time_labels.length > 0 && data.moisture_data.length > 0) {
      const timeLabels = data.time_labels;
      const moistureData = data.moisture_data;

      // Limit the number of data points if it exceeds the maximum allowed
      const maxDataPoints = Math.min(data.graph_period, timeLabels.length);
      const truncatedTimeLabels = timeLabels.slice(-maxDataPoints);
      const truncatedMoistureData = moistureData.slice(-maxDataPoints);

      // Create a new Chart instance for the first canvas
      const ctx = document.getElementById('moistureChart0').getContext('2d');
      const moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: truncatedTimeLabels,
          datasets: [{
            label: data.title,
            data: truncatedMoistureData,
            backgroundColor: 'rgba(75, 192, 192, 0.2)', // Fill color
            borderColor: 'rgba(75, 192, 192, 1)', // Line color
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: {
              title: {
                display: true,
                text: 'Time'
              }
            },
            y: {
              title: {
                display: true,
                text: 'Moisture Level'
              }
            }
          }
        }
      });
    } else {
      console.error('JSON data is empty or invalid.');
    }
  })
  .catch(error => console.error('Error fetching data:', error));


// Fetch JSON data for the second graph ---------------------------------------------------------------------
fetch('static/data/data1.json')
  .then(response => response.json())
  .then(data => {
    // Check if the JSON data is not empty
    if (data && data.time_labels && data.moisture_data && data.time_labels.length > 0 && data.moisture_data.length > 0) {
      const timeLabels = data.time_labels;
      const moistureData = data.moisture_data;

      // Limit the number of data points if it exceeds the maximum allowed
      const maxDataPoints = Math.min(data.graph_period, timeLabels.length);
      const truncatedTimeLabels = timeLabels.slice(-maxDataPoints);
      const truncatedMoistureData = moistureData.slice(-maxDataPoints);

      // Create a new Chart instance for the first canvas
      const ctx = document.getElementById('moistureChart1').getContext('2d');
      const moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: truncatedTimeLabels,
          datasets: [{
            label: data.title,
            data: truncatedMoistureData,
            backgroundColor: 'rgba(75, 192, 192, 0.2)', // Fill color
            borderColor: 'rgba(75, 192, 192, 1)', // Line color
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: {
              title: {
                display: true,
                text: 'Time'
              }
            },
            y: {
              title: {
                display: true,
                text: 'Moisture Level'
              }
            }
          }
        }
      });
    } else {
      console.error('JSON data is empty or invalid.');
    }
  })
  .catch(error => console.error('Error fetching data:', error));



  // Fetch JSON data for the third graph---------------------------------------------------------------------
fetch('static/data/data2.json')
  .then(response => response.json())
  .then(data => {
    // Check if the JSON data is not empty
    if (data && data.time_labels && data.moisture_data && data.time_labels.length > 0 && data.moisture_data.length > 0) {
      const timeLabels = data.time_labels;
      const moistureData = data.moisture_data;

      // Create a new Chart instance for the first canvas
      const ctx = document.getElementById('moistureChart2').getContext('2d');
      const moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: timeLabels,
          datasets: [{
            label: data.title,
            data: moistureData,
            backgroundColor: 'rgba(75, 192, 192, 0.2)', // Fill color
            borderColor: 'rgba(75, 192, 192, 1)', // Line color
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: {
              title: {
                display: true,
                text: 'Time'
              }
            },
            y: {
              title: {
                display: true,
                text: 'Moisture Level'
              }
            }
          }
        }
      });
    } else {
      console.error('JSON data is empty or invalid.');
    }
  })
  .catch(error => console.error('Error fetching data:', error));



// Fetch JSON data for the fourth graph -----------------------------------------------------------------------
fetch('static/data/data3.json')
  .then(response => response.json())
  .then(data => {
    // Check if the JSON data is not empty
    if (data && data.time_labels && data.moisture_data && data.time_labels.length > 0 && data.moisture_data.length > 0) {
      const timeLabels = data.time_labels;
      const moistureData = data.moisture_data;

      // Create a new Chart instance for the first canvas
      const ctx = document.getElementById('moistureChart3').getContext('2d');
      const moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: timeLabels,
          datasets: [{
            label: data.title,
            data: moistureData,
            backgroundColor: 'rgba(75, 192, 192, 0.2)', // Fill color
            borderColor: 'rgba(75, 192, 192, 1)', // Line color
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: {
              title: {
                display: true,
                text: 'Time'
              }
            },
            y: {
              title: {
                display: true,
                text: 'Moisture Level'
              }
            }
          }
        }
      });
    } else {
      console.error('JSON data is empty or invalid.');
    }
  })
  .catch(error => console.error('Error fetching data:', error));


// Fetch JSON data for the fourth graph -----------------------------------------------------------------------
fetch('static/data/data4.json')
  .then(response => response.json())
  .then(data => {
    // Check if the JSON data is not empty
    if (data && data.time_labels && data.moisture_data && data.time_labels.length > 0 && data.moisture_data.length > 0) {
      const timeLabels = data.time_labels;
      const moistureData = data.moisture_data;

      // Create a new Chart instance for the first canvas
      const ctx = document.getElementById('moistureChart4').getContext('2d');
      const moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: timeLabels,
          datasets: [{
            label: data.title,
            data: moistureData,
            backgroundColor: 'rgba(75, 192, 192, 0.2)', // Fill color
            borderColor: 'rgba(75, 192, 192, 1)', // Line color
            borderWidth: 1
          }]
        },
        options: {
          scales: {
            x: {
              title: {
                display: true,
                text: 'Time'
              }
            },
            y: {
              title: {
                display: true,
                text: 'Moisture Level'
              }
            }
          }
        }
      });
    } else {
      console.error('JSON data is empty or invalid.');
    }
  })
  .catch(error => console.error('Error fetching data:', error));
