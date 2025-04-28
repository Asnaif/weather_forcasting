const mongoose = require('mongoose');

const sensorDataSchema = new mongoose.Schema({
    temperature: {
        type: Number,
        required: true,
        min: -50,  // Prevents invalid values
        max: 100
    },
    humidity: {
        type: Number,
        required: true,
        min: 0,
        max: 100
    },
    air_quality: {  // ✅ Add this if missing
        type: Number,
        required: true, 
    },
    timestamp: {
        type: Date,
        default: Date.now
    },

});

const SensorData = mongoose.model('SensorData', sensorDataSchema);
module.exports = SensorData;
